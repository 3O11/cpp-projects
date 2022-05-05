#include "priority_scheduler.hpp"

#include <string>

struct no_vtls {};
using simple_scheduler = scheduler<no_vtls>;

class simple_task : public simple_scheduler::task {
    const std::string name;
    const size_t wait_ms;
public:
    explicit simple_task(const std::string &name, size_t wait_ms) 
            : name(name), wait_ms(wait_ms) {}

    void run(scheduler<no_vtls> &, scheduler<no_vtls>::vthread_info &) override {
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
}