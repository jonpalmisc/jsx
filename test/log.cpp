#include "jsx/log.h"

int main(int argc, char const **argv)
{
    jsx::set_log_level(jsx::LogLevel::Trace);
    jsx::set_log_option(jsx::LogOption::Color, true);

    jsx::log_error("This is an %xrror message.", 0xe);
    jsx::log_warn(std::string("This is a warning message."));
    jsx::log_info("This is an %s message.", "info");
    jsx::log_debug("This is a %c%c%c%c%c message.", 'd', 'e', 'b', 'u', 'g');
    jsx::log_trace("This is a trace message.");

    return 0;
}
