#ifndef PRIORITY_SCHEDULER_HPP
#define PRIORITY_SCHEDULER_HPP

#include <cassert>
#include <limits>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <semaphore>
#include <vector>
#include <queue>
#include <algorithm>

constexpr size_t CACHE_LINE_SIZE = 64;

/**
 * @brief The main scheduler class
 * 
 * @tparam VTLS_T virtual thread local storage type. Must be default constructable.
 */
template<typename VTLS_T>
class scheduler {
private:
    class vthread;
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
    
    /**
     * @brief A class holding information about a virtual thread
     */
    class alignas(CACHE_LINE_SIZE) vthread_info {
    public:
        vthread_info(vthread_id_t id)
            : m_id(id)
        {}

        /**
         * @brief Returns the virtual thread ID
         */
        vthread_id_t id() const
        {
            return m_id;
        }

        /**
         * @brief Returns a thread local data of the virtual thread 
         */
        VTLS_T &data()
        {
            return m_data;
        }

        bool operator> (const vthread_info& other) const
        {
            return m_id > other.m_id;
        }

    private:
        vthread_id_t m_id;
        VTLS_T m_data{};

        friend vthread;
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
        : m_thread_count(num_threads), m_max_idle(time_to_idle_ms)
    {
        m_vthreads.reserve(num_threads);
        for (int64_t i = num_threads - 1; i >= 0; --i)
        {
            m_vthreads.push_back(std::make_unique<vthread_info>(i));
        }

        init_thread();
    }

    ~scheduler()
    {
        m_exit_semaphore.acquire();
    }

    /**
     * @brief Add a new task into the scheduler's queue
     * @note Can be called in parallel
     * 
     * @param task Task to be added
     */
    void add_task(std::unique_ptr<task> &&t)
    {
        std::lock_guard l(m_tasks_mtx);
        m_tasks.push(std::move(t));
    }

private:
    const size_t m_thread_count;
    const size_t m_max_idle;
    std::atomic<size_t> m_active_threads = 0;
    std::atomic<size_t> m_idle_threads   = 0;
    std::mutex m_vthreads_mtx;
    std::vector<std::unique_ptr<vthread_info>> m_vthreads;
    std::mutex m_tasks_mtx;
    std::queue<std::unique_ptr<task>> m_tasks;
    std::binary_semaphore m_exit_semaphore{0};

    void init_thread()
    {
        std::thread([this]()
        {
            // Increase thread count so that other threads don't
            // accidentally create too many system threads.
            ++m_active_threads;
            ++m_idle_threads;

            // Utilities so that working with time is easier
            using clock = std::chrono::high_resolution_clock;
            using duration = std::chrono::duration<double, std::milli>;

            // Data
            std::unique_ptr<task> current_task;
            std::unique_ptr<vthread_info> current_vthread;

            // Main exec loop
            auto idle_start = clock::now();
            while (duration(clock::now() - idle_start).count() <= m_max_idle)
            {
                {
                    std::lock_guard l(m_tasks_mtx);
                    if (m_tasks.size())
                    {
                        current_task = std::move(m_tasks.front());
                        m_tasks.pop();
                        --m_idle_threads;
                    }

                    if (m_tasks.size() && m_active_threads < m_thread_count && m_idle_threads == 0)
                    {
                        init_thread();
                    }
                }
            
                {
                    // There is no check if the vthread queue is empty, because
                    // each thread can have at most one vthread, and the amount
                    // of threads is the same as the amount of vthreads.
                    std::lock_guard l(m_vthreads_mtx);
                    current_vthread = std::move(m_vthreads.back());
                    m_vthreads.pop_back();
                }

                if (current_task)
                {
                    current_task->run(*this, *current_vthread);
                    current_task = nullptr;
                    idle_start = clock::now();
                    ++m_idle_threads;
                }

                {
                    std::lock_guard l(m_vthreads_mtx);
                    m_vthreads.insert(
                        std::upper_bound(m_vthreads.begin(), m_vthreads.end(), current_vthread, [](auto&& a, auto&& b){ return a > b; }),
                        std::move(current_vthread)
                    );
                }
            }

            --m_active_threads;
            if (!m_active_threads) m_exit_semaphore.release();
        }).detach();
    }
};

#endif // PRIORITY_SCHEDULER_HPP
