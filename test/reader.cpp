#include "jsx/reader.h"
#include "jsx/hex.h"
#include "jsx/log.h"

#include "data.h"

struct PartialMachHeader64 {
    uint32_t magic;
    uint32_t cpu_type;
    uint32_t cpu_subtype;
    uint32_t file_type;
};

void read_macho_header(jsx::Reader &reader)
{
    uint8_t start[32];
    reader.read(start, sizeof(start));
    reader.seek(0);

    jsx::log_info("%s", jsx::hex_format_dump(start, sizeof(start)).c_str());

    PartialMachHeader64 header;
    if (!reader.read_object(header))
        jsx::log_fatal("Failed to read object!");
    if (header.magic != 0xfeedfacf)
        jsx::log_fatal("Bad magic!");
    if (header.cpu_type != 0x100000c)
        jsx::log_fatal("Bad CPU type!");
    if (header.cpu_subtype != 0x0)
        jsx::log_fatal("Bad CPU subtype!");
    if (header.file_type != 0x2)
        jsx::log_fatal("Bad file type!");

    reader.seek(0);
    uint32_t magic, cpu_type, cpu_subtype, file_type;
    if (!reader.read_int(magic) || magic != header.magic)
        jsx::log_fatal("Failed to read magic!");
    if (!reader.read_int(cpu_type) || cpu_type != header.cpu_type)
        jsx::log_fatal("Failed to read CPU type!");
    if (!reader.read_int(cpu_subtype) || cpu_subtype != header.cpu_subtype)
        jsx::log_fatal("Failed to read CPU subtype!");
    if (!reader.read_int(file_type) || file_type != header.file_type)
        jsx::log_fatal("Failed to read file type!");

    reader.seek(0);
    if (!reader.read_int(magic, jsx::Endianness::Big) || magic != 0xcffaedfe)
        jsx::log_fatal("Failed to read magic!");
}

int main(int argc, char const **argv)
{
    (void)argc;

    jsx::BufferReader buffer_reader(MACHO_HEADER, MACHO_HEADER_SIZE, jsx::Endianness::Little);
    read_macho_header(buffer_reader);

    auto file_reader = jsx::FileReader::from_path(argv[0], jsx::Endianness::Little);
    if (!file_reader)
        jsx::log_fatal("Failed to create file reader for own executable.");

    read_macho_header(*file_reader);

    return 0;
}
