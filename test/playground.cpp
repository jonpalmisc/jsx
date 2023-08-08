#include <jsx/hex.h>
#include <jsx/log.h>
#include <jsx/timer.h>

#include <thread>

using namespace jsx;

int main(int argc, char **argv) {
  log::set_level(log::Level::Debug);

  timer::Timer clock;
  {
    timer::ScopedTimer callback_timer([](uint64_t ms) {
      log::info(
          "Error and warning messages logged in %llu ms. (Expected: ~2 ms)",
          ms);
    });
    log::error("This is an error message.");
    log::warn("This is a warning message.");
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  uint64_t info_debug_time = 0;
  {
    timer::ScopedTimer info_debug_timer(&info_debug_time);
    log::info("This is an info message.");
    log::debug("This is a debug message.");
    std::this_thread::sleep_for(std::chrono::milliseconds(7));
  }

  auto hex_string = hex::encode({'A', 'A', 'A', 'A'});
  log::info("Printing \"AAAA\" in hex: %s", hex_string.c_str());

  std::this_thread::sleep_for(std::chrono::milliseconds(34));
  log::info("All functionality tested in %llu ms. (Expected: ~41 ms.)",
            clock.elapsed_ms());
  log::info("Info and debug messages logged in %llu ms. (Expected: ~7 ms.)",
            info_debug_time);

  return 0;
}
