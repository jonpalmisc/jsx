//
//  Copyright (c) 2022-2023 Jon Palmisciano. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#include <jsx/hex.h>

#include <stdio.h>

#define DUMP_LINE_LENGTH 16

static void print_dump_line(unsigned char const *buffer, size_t length, size_t address)
{
    printf("%08zx:   ", address);

    for (size_t i = 0; i < DUMP_LINE_LENGTH; ++i) {
        if (i > 0 && i % 8 == 0)
            printf(" ");

        if (i < length)
            printf("%02x ", buffer[i]);
        else
            printf("   ");
    }

    printf("   ");

    for (size_t i = 0; i < DUMP_LINE_LENGTH; ++i) {
        unsigned char v = buffer[i];
        if (v >= 0x20 && v < 0x7f)
            printf("%c", v);
        else
            printf(".");
    }

    printf("\n");
}

void jsx_hex_print_dump(unsigned char const *buffer, size_t length, size_t base)
{
    size_t offset = 0;

    while (length - offset >= DUMP_LINE_LENGTH) {
        print_dump_line(buffer + offset, DUMP_LINE_LENGTH, base + offset);
        offset += DUMP_LINE_LENGTH;
    }

    size_t remainder = length - offset;
    if (remainder > 0)
        print_dump_line(buffer + offset, remainder, base + offset);
}

void jsx_hex_print_raw(unsigned char const *buffer, size_t length)
{
    for (size_t i = 0; i < length; ++i)
        printf("%02x", buffer[i]);

    printf("\n");
}
