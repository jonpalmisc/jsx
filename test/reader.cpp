#include "jsx/hex.h"
#include "jsx/log.h"
#include "jsx/reader.h"

using namespace jsx;

uint8_t dump_data[] = {
    0xcf, 0xfa, 0xed, 0xfe, 0x0c, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0xd8, 0x05, 0x00, 0x00,
    0x85, 0x80, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00,
    0x48, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x50, 0x41, 0x47, 0x45, 0x5a, 0x45
};

struct PartialMachHeader64 {
    uint32_t magic;
    uint32_t cpu_type;
    uint32_t cpu_subtype;
    uint32_t file_type;
};

void read_macho_header(Reader &reader)
{
    uint8_t start[32];
    reader.read(start, sizeof(start));
    reader.seek(0);

    log_info("%s", hex_format_dump(start, sizeof(start)).c_str());

    PartialMachHeader64 header;
    if (!reader.read_object(header))
        log_fatal("Failed to read object!");
    if (header.magic != 0xfeedfacf)
        log_fatal("Bad magic!");
    if (header.cpu_type != 0x100000c)
        log_fatal("Bad CPU type!");
    if (header.cpu_subtype != 0x0)
        log_fatal("Bad CPU subtype!");
    if (header.file_type != 0x2)
        log_fatal("Bad file type!");

    reader.seek(0);
    uint32_t magic, cpu_type, cpu_subtype, file_type;
    if (!reader.read_int(magic) || magic != header.magic)
        log_fatal("Failed to read magic!");
    if (!reader.read_int(cpu_type) || cpu_type != header.cpu_type)
        log_fatal("Failed to read CPU type!");
    if (!reader.read_int(cpu_subtype) || cpu_subtype != header.cpu_subtype)
        log_fatal("Failed to read CPU subtype!");
    if (!reader.read_int(file_type) || file_type != header.file_type)
        log_fatal("Failed to read file type!");

    reader.seek(0);
    if (!reader.read_int(magic, Endianness::Big) || magic != 0xcffaedfe)
        log_fatal("Failed to read magic!");
}

int main(int argc, char const **argv)
{
    (void)argc;

    BufferReader buffer_reader(dump_data, sizeof(dump_data), Endianness::Little);
    read_macho_header(buffer_reader);

    auto file_reader = FileReader::from_path(argv[0], Endianness::Little);
    if (!file_reader)
        log_fatal("Failed to create file reader for own executable.");

    read_macho_header(*file_reader);

    return 0;
}
