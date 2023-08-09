//===-- jsx/hex.cpp - Hexadecimal encoding/decoding & formatting ----------===//
//
// Copyright (c) 2022-2023 Jon Palmisciano. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//===----------------------------------------------------------------------===//

#include <jsx/hex.h>

#include <iomanip>
#include <sstream>

namespace jsx::hex {

std::string encode(std::vector<uint8_t> data) {
  return encode(data.data(), data.size());
}

std::string encode(uint8_t const *data, size_t length) {
  std::stringstream result_stream;

  result_stream << std::hex;
  for (size_t i = 0; i < length; ++i)
    result_stream << static_cast<uint32_t>(data[i]);

  return result_stream.str();
}

constexpr size_t dump_bytes_per_line = 16;
constexpr size_t dump_bytes_per_column = 2;

std::string dump(std::vector<uint8_t> data, uint64_t base_address) {
  return dump(data.data(), data.size(), base_address);
}

std::string dump(uint8_t const *data, size_t length, uint64_t base_address) {
  std::stringstream dump_stream;

  // Print a single line of the hex dump.
  auto dump_line = [](std::stringstream &dump, uint64_t offset,
                      uint8_t const *data, size_t length) {
    // Prefix each line with its address/offset, also configure zero padding.
    dump << std::setfill('0') << std::setw(8) << std::hex << offset << ":  ";

    // Print each byte in the line while grouping them into columns.
    for (size_t i = 0; i < dump_bytes_per_line; ++i) {
      // Insert an extra space to separate columns.
      if (i > 0 && i % dump_bytes_per_column == 0)
        dump << ' ';

      // If the next byte of data is present, print it; otherwise, print spaces
      // to fill the gap.
      if (i < length)
        dump << std::setw(2) << static_cast<uint32_t>(data[i]);
      else
        dump << "  ";
    }
    dump << "  ";

    // Print the same bytes again formatted as characters.
    for (size_t i = 0; i < dump_bytes_per_line; ++i) {
      if (i >= length) {
        dump << ' ';
        continue;
      }

      // Display values that map to printable characters as characters, map all
      // non-printable characters to a period.
      auto display_char = data[i] >= 0x20 && data[i] < 0x7f ? data[i] : '.';
      dump << static_cast<char>(display_char);
    }
    dump << "\n";
  };

  auto offset = 0;
  while (length - offset >= dump_bytes_per_line) {
    dump_line(dump_stream, base_address + offset, data + offset,
              dump_bytes_per_line);
    offset += dump_bytes_per_line;
  }

  // In the case where the given data is not an even multiple of the bytes per
  // line, the above loop will not have formatted all of the available data and
  // a partial line will need to be emitted.
  auto remainder = length - offset;
  if (remainder > 0)
    dump_line(dump_stream, base_address + offset, data + offset, remainder);

  return dump_stream.str();
}

} // namespace jsx::hex
