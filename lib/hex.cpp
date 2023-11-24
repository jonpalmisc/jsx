//
//  jsx/hex.cpp
//  https://github.com/jonpalmisc/jsx
//
//  Copyright (c) 2022-2023 Jon Palmisciano. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.

#include <jsx/hex.h>

#include <iomanip>
#include <sstream>

namespace jsx {

std::string hex_encode(std::vector<uint8_t> const &data) {
  return hex_encode(data.data(), data.size());
}

std::string hex_encode(uint8_t const *data, size_t length) {
  std::stringstream result_stream;

  result_stream << std::hex;
  for (size_t i = 0; i < length; ++i)
    result_stream << static_cast<uint32_t>(data[i]);

  return result_stream.str();
}

HexDumper::HexDumper()
    : left_margin(2), right_margin(2), byte_margin(0), column_margin(1),
      bytes_per_column(2), bytes_per_line(16), show_offset(true),
      show_ascii(true) {}

std::string HexDumper::format(std::vector<uint8_t> const &data,
                              uint64_t base_offset) const {
  return format(data.data(), data.size(), base_offset);
}

std::string HexDumper::format(uint8_t const *data, size_t size,
                              uint64_t base_offset) const {
  std::stringstream dump;

  auto left_margin_text = std::string(left_margin, ' ');
  auto right_margin_text = std::string(right_margin, ' ');
  auto column_margin_text = std::string(column_margin, ' ');
  auto byte_margin_text = std::string(byte_margin, ' ');

  // Print a single line of the hex dump.
  auto dump_line = [&, this](uint64_t offset, uint8_t const *line,
                             size_t length) {
    // Configure output formatting.
    dump << std::setfill('0') << std::hex;

    // Prefix each line with its address/offset, if requested.
    if (show_offset)
      dump << std::setw(8) << offset << ":" << left_margin_text;

    // Print each byte in the line while grouping them into columns.
    for (size_t i = 0; i < bytes_per_line; ++i) {
      // Insert an extra space to separate columns.
      if (i > 0 && i % bytes_per_column == 0)
        dump << column_margin_text;

      // If the next byte of data is present, print it; otherwise, print spaces
      // to fill the gap.
      if (i < length)
        dump << std::setw(2) << static_cast<uint32_t>(line[i]);
      else
        dump << "  ";

      dump << byte_margin_text;
    }

    // Print the same bytes again formatted as characters, if requested.
    if (show_ascii) {
      dump << right_margin_text;

      for (size_t i = 0; i < bytes_per_line; ++i) {
        if (i >= length) {
          dump << ' ';
          continue;
        }

        // Display values that map to printable characters as characters, map
        // all non-printable characters to a period.
        auto display_char = line[i] >= 0x20 && line[i] < 0x7f ? line[i] : '.';
        dump << static_cast<char>(display_char);
      }
    }

    dump << "\n";
  };

  size_t offset = 0;
  while (size - offset >= bytes_per_line) {
    dump_line(base_offset + offset, data + offset, bytes_per_line);
    offset += bytes_per_line;
  }

  // In the case where the given data is not an even multiple of the bytes per
  // line, the above loop will not have formatted all of the available data and
  // a partial line will need to be emitted.
  if (auto remainder = size - offset; remainder > 0)
    dump_line(base_offset + offset, data + offset, remainder);

  return dump.str();
}

} // namespace jsx
