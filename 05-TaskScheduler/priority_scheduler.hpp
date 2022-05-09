#ifndef PRIORITY_SCHEDULER_HPP
#define PRIORITY_SCHEDULER_HPP

#include <cassert>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <semaphore>
#include <chrono>
#include <iostream>

void threadSafeLog(const std::string& msg)
{
    static std::mutex s_log_mtx;

    std::lock_guard l(s_log_mtx);
    std::cout << msg << "\n";
}

constexpr size_t CACHE_LINE_SIZE = 64;

/**
 * @brief The main scheduler class
 * 
 * @tparam VTLS_T virtual thread local storage type. Must be default constructable.
 */
template <typename VTLS_T>
class scheduler final {
public:
    /**
     * @brief Virtual thread ID
     * 
     */
    using vthread_id_t = size_t;
    
    /**
     * @brief An invalid virtual thread ID
     * 
     */
    static constexpr size_t INVALID_VTHREAD_ID = std::numeric_limits<vthread_id_t>::max();
    
    class task;

    /**
     * @brief A class holding information about a virtual thread
     */
    class alignas(CACHE_LINE_SIZE) vthread_info {
    public:
        vthread_info(vthread_id_t id)
            : m_thread_id(id)
        {}

        /**
         * @brief Returns the virtual thread ID
         */
        vthread_id_t id() const
        {
            return m_thread_id;
        }

        /**
         * @brief Returns a thread local data of the virtual thread 
         */
        VTLS_T& data()
        {
            return m_thread_data;
        }

        bool operator> (const vthread_info& other) const
        {
            return id() > other.id();
        }

    private:
        vthread_id_t m_thread_id;
        thread_local static VTLS_T m_thread_data;
    };

    /**
     * @brief A simple unit executed atop of the virtual threads
     */
    class task {
    public:
        virtual ~task() {}

        /**
         * @brief A function implementing the main body of the task. Each task executes this function and terminates
         * 
         * @param s A scheduler invoking this task
         * @param info Information about the virtual thread executing the task
         */
        virtual void run(scheduler &s, vthread_info &info) = 0;
    };

    /**
     * @brief Construct a new scheduler object
     * 
     * @param num_threads Number of real threads
     * @param time_to_idle_ms Time in milliseconds before a thread goes to sleep
     */
    scheduler(size_t num_threads, size_t time_to_idle_ms)
    {
        m_thread_count = num_threads;

        init_exec_threads(num_threads, time_to_idle_ms);
        init_scheduling_thread();
    }

    ~scheduler()
    {
        for (auto&& thread : m_threads)
        {
            thread.join();
        }
        m_scheduler_thread.join();
    }

    /**
     * @brief Add a new task into the scheduler's queue
     * @note Can be called in parallel
     * 
     * @param task Task to be added
     */
    void add_task(std::unique_ptr<task> &&t)
    {
        {
            std::lock_guard l(m_tasks_mtx);
            m_tasks.push(std::move(t));
        }
        m_tasks_semaphore.release();
    }
private:
    struct vthread_data
    {
        vthread_data(size_t i)
            : info(i)
        {}

        bool operator> (const vthread_data& other) const
        {
            return info > other.info;
        }

        vthread_info info;
        std::atomic<bool> sleeping = false;

        std::unique_ptr<task> current_task;
        std::binary_semaphore task_ready{0};
        std::binary_semaphore sleep_trap{0};
        std::binary_semaphore terminate{0};
    };

    void init_scheduling_thread()
    {
        m_scheduler_thread = std::thread([this]()
        {
            // The scheduler thread code goes here, it is a consumer for threads, and a
            // producer for tasks (in relation to task-execution threads).
            while (true)
            {
                if (m_threads_semaphore.try_acquire())
                {
                    if (m_tasks_semaphore.try_acquire())
                    {
                        std::scoped_lock l(m_free_threads_mtx, m_tasks_mtx);

                        vthread_data& data = m_free_threads.top();
                        m_free_threads.pop();
                        data.current_task = std::move(m_tasks.front());
                        m_tasks.pop();
                        data.task_ready.release();

                        if (data.sleeping) data.sleep_trap.release();
                    }
                    else
                    {
                        m_threads_semaphore.release();

                        std::lock_guard l(m_free_threads_mtx);
                        if (m_free_threads.size() >= m_thread_count)
                        {
                            auto free_threads_copy = m_free_threads;

                            bool should_terminate = true;
                            while (!free_threads_copy.empty())
                            {
                                auto && thread = free_threads_copy.top().get();
                                if(!thread.sleeping)
                                {
                                    should_terminate = false;
                                    break;
                                }
                                free_threads_copy.pop();
                            }

                            if (should_terminate)
                            {
                                while (!m_free_threads.empty())
                                {
                                    auto && thread = m_free_threads.top().get();
                                    thread.terminate.release();
                                    thread.sleep_trap.release();
                                    m_free_threads.pop();
                                }

                                break;
                            }
                        }
                    }
                }
            }
        });
    }

    void init_exec_threads(size_t thread_count, size_t time_to_idle_ms)
    {
        m_threads.resize(thread_count);
        for (size_t i = 0; i < thread_count; i++)
        {
            m_threads[i] = std::thread(
                [this, i, time_to_idle_ms]()
                {
                    // This is to improve readability, because without it, the
                    // calls are getting way too long.
                    using clock = std::chrono::high_resolution_clock;
                    using duration = std::chrono::duration<double, std::milli>;

                    // First goes the init code, that will only run once.
                    {
                        using namespace std::chrono_literals;
                        std::this_thread::sleep_for(i * 10ms);
                    }

                    vthread_data data(i);
                    {
                        std::lock_guard l(m_free_threads_mtx);
                        m_free_threads.push(data);
                    }
                    m_threads_semaphore.release();
                    auto wait_start = clock::now();

                    // Now we should put the task processing loop here.
                    while (!data.terminate.try_acquire())
                    {
                        if (duration(clock::now() - wait_start).count() <= time_to_idle_ms)
                        {
                            // While the thread is idling, poll actively for
                            // the next task.
                            if (data.task_ready.try_acquire())
                            {
                                data.current_task->run(*this, data.info);
                                data.current_task = nullptr;
                                {
                                    std::lock_guard l(m_free_threads_mtx);
                                    m_free_threads.push(data);
                                }
                                m_threads_semaphore.release();
                                wait_start = clock::now();
                            }
                        }
                        else
                        {
                            // Once the idling time has passed, trap the
                            // thread in a sleep semaphore that can be
                            // released from the scheduler thread.
                            data.sleeping = true;
                            data.sleep_trap.acquire();
                            wait_start = clock::now();
                            data.sleeping = false;
                        }
                    }
                }
            );
        }
    }

    std::vector<std::thread> m_threads;
    std::thread m_scheduler_thread;

    std::mutex m_free_threads_mtx;
    std::priority_queue<
        std::reference_wrapper<vthread_data>,
        std::vector<std::reference_wrapper<vthread_data>>,
        std::greater<vthread_data>> m_free_threads;

    std::mutex m_tasks_mtx;
    std::queue<std::unique_ptr<task>> m_tasks;

    std::counting_semaphore<> m_threads_semaphore{0};
    std::counting_semaphore<> m_tasks_semaphore{0};

    size_t m_thread_count;
};

template <typename VTLS_T>
thread_local VTLS_T scheduler<VTLS_T>::vthread_info::m_thread_data = VTLS_T{};

#endif // PRIORITY_SCHEDULER_HPP
