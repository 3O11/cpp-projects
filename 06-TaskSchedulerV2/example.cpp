#include "priority_scheduler.hpp"

#include <string>
#include <iostream>

void threadSafeLog(const std::string& msg)
{
    static std::mutex mtx;

    std::lock_guard l(mtx);
    std::cout << msg << "\n";
}

struct no_vtls {};
using simple_scheduler = scheduler<int>;

class simple_task : public simple_scheduler::task {
    const std::string name;
    const size_t wait_ms;
public:
    explicit simple_task(const std::string &name, size_t wait_ms) 
            : name(name), wait_ms(wait_ms) {}

    void run(scheduler<int> &, scheduler<int>::vthread_info &i) override {
        i.data() = i.id();
        printf("Thread %d is working very hard ...\n", i.data());
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
        printf("%s ends\n", this->name.c_str());
    }
};

int main() {
    using namespace std::chrono_literals;
    auto num_threads = std::thread::hardware_concurrency();

    simple_scheduler s(num_threads, 500);
    //s.add_task(std::make_unique<simple_task>("Waiting game for 0", 20000));
    //s.add_task(std::make_unique<simple_task>("Waiting game for 1", 20100));
    //s.add_task(std::make_unique<simple_task>("Waiting game for 2", 20200));
    //s.add_task(std::make_unique<simple_task>("Waiting game for 3", 20300));
    for (size_t i = 1; i <= 64; ++i) {
        threadSafeLog("Adding task " + std::to_string(i));
        s.add_task(std::make_unique<simple_task>("S-" + std::to_string(i), i * 100));
        //std::this_thread::sleep_for(110ms);
    }

    {
        threadSafeLog("Starting sleep");
        std::this_thread::sleep_for(30s);
    }

    threadSafeLog("Attempting to add task to dead scheduler");
    s.add_task(std::make_unique<simple_task>("Dead", 100));
}
