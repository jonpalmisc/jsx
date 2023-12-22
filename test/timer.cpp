#include "jsx/timer.h"
#include "jsx/log.h"

#include <thread>

int main(int argc, char const **argv)
{
    {
        jsx::ScopedTimer callback_timer([](uint64_t ms) {
            jsx::log_info("Callback timer was in scope for %llu ms.", ms);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    uint64_t writeback_ms = 0;
    {
        jsx::ScopedTimer writeback_timer(&writeback_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    jsx::log_info("Writeback timer was in scope for %llu ms.", writeback_ms);

    return 0;
}
