#include "priority_scheduler.hpp"

#include <string>

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
        printf("Task is working very hard ... %d\n", i.data());
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
        printf("%s ends\n", this->name.c_str());
    }
};

int main() {
    using namespace std::chrono_literals;
    auto num_threads = std::thread::hardware_concurrency();

    simple_scheduler s(num_threads, 500);
    for (size_t i = 1; i <= 64; ++i) {
        s.add_task(std::make_unique<simple_task>("S-" + std::to_string(i), i * 100));
    }

    {
        threadSafeLog("Starting sleep");
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(30s);
    }

    threadSafeLog("Attempting to add task to dead scheduler");
    s.add_task(std::make_unique<simple_task>("Dead", 100));
}
