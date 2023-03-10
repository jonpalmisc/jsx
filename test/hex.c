#include <jsx/hex.h>

#include <assert.h>

unsigned char const DemoData[] = {
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49,
    0x48, 0x44, 0x52, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x06,
    0x00, 0x00, 0x00, 0x7f, 0x1d, 0x2b, 0x83, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48,
    0x59, 0x73, 0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a,
    0x9c, 0x18, 0x00, 0x07, 0x0d, 0x81, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0xec,
    0xfd, 0x4d, 0x82, 0xed, 0x2c, 0xae, 0x04, 0x00, 0xc2, 0x79, 0xb9, 0xc7, 0x1e,
    0xf4, 0x1e, 0x7a, 0xff, 0x93, 0x97, 0xf4, 0xc0, 0x06, 0xa4, 0x50, 0x84, 0x90,
    0x9d, 0xe7, 0xd6, 0xab, 0xfe, 0xa1, 0xea, 0xbb, 0x79, 0x6c, 0x83, 0x24, 0x40,
    0x48, 0x21, 0x0c, 0xb8, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0xaf, 0xd1, 0x30, 0xf5,
    0xfb, 0x6f, 0x7c, 0x12, 0xb2, 0xe8, 0x0c, 0xbd, 0xf5, 0xf9, 0x5f, 0xbb, 0xfe,
    0x7e, 0x3e, 0x9f, 0xd6, 0x7a, 0x6b, 0x9f, 0xfe, 0x31, 0xcf, 0x5a, 0xeb, 0x9f,
    0x4f, 0xfb, 0xf4, 0xde, 0xfa, 0xe7, 0x03, 0xf9, 0x7b, 0x6b, 0xad, 0x5f, 0xcf,
    0x7a, 0xdf, 0x34, 0x67, 0x7e, 0x93, 0xaf, 0xf7, 0x7e, 0xb3, 0x05, 0xbe, 0x9f,
    0xcf, 0xc5, 0xa3, 0x6f, 0x19, 0x66, 0x5e, 0x9b, 0x66, 0x55, 0x7b, 0x5e, 0x6d,
    0xa8, 0xf7, 0xce, 0xed, 0xca, 0x20, 0x01, 0x5b, 0x68, 0x64, 0xd4, 0x4d, 0xf1,
    0x61, 0xaf, 0x50, 0x5e, 0x41, 0x63, 0x30, 0x69, 0x3a, 0xfd, 0x19, 0xcb, 0x0a,
    0x9a, 0xa6, 0xad, 0x46, 0x41, 0x76, 0x4e, 0x7a, 0x97, 0x2b, 0xd6, 0x04, 0x09,
    0x3c, 0xcb, 0x5f, 0xa0, 0x63, 0xeb, 0x55, 0xa7, 0x3f, 0x8c, 0xa2, 0xe8, 0xc6,
    0xcc, 0xea, 0x8b, 0xf4, 0x66, 0xce, 0x01, 0x5d, 0x16, 0xf5, 0xcc, 0xc8, 0x37,
    0xb6, 0x1c, 0x51, 0x8c, 0xce, 0x19, 0x2f, 0x9a, 0x51
};

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    jsx_hex_print_dump(DemoData, sizeof(DemoData), 0x1000);
    jsx_hex_print_raw(DemoData, 40);

    return 0;
}
