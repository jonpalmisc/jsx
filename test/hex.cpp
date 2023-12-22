#include "jsx/hex.h"
#include "jsx/log.h"

#include "data.h"

int main(int argc, char const **argv)
{
    auto dump = jsx::hex_format_dump(MACHO_HEADER, MACHO_HEADER_SIZE, 0x1000);
    jsx::log_info("%s", dump.c_str());

    auto hex_string = jsx::hex_encode(MACHO_HEADER, 48);
    jsx::log_info("%s", hex_string.c_str());

    return 0;
}
