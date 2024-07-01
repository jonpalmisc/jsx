//
//  jsx/hex.h
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
//

#pragma once

#include <string>
#include <vector>

namespace jsx {

/// Encode \p data to a hexadecimal string.
std::string hex_encode(std::vector<uint8_t> const &data);

/// Encode \p length bytes of \p data to a hexadecimal string.
std::string hex_encode(uint8_t const *data, size_t length);

/// Configurable hex dump formatter.
class HexDumper {
public:
    /// Number of spaces to pad the left side of the content with.
    size_t left_margin;

    /// Number of spaces to pad the right side of the content with.
    size_t right_margin;

    /// Number of spaces to separate each byte with.
    size_t byte_margin;

    /// Number of spaces to separate groups with.
    size_t column_margin;

    /// Number of bytes per each column.
    size_t bytes_per_column;

    /// Number of bytes per each line.
    size_t bytes_per_line;

    /// Should the offset of each line be shown before its content?
    bool show_offset;

    /// Should an ASCII preview of each line be shown after its content?
    bool show_ascii;

    /// Create the default dump formatter; has `xxd`-like configuration.
    HexDumper();

    /// Create a formatted hex dump of \p size bytes of \p data.
    ///
    /// The effective offset of data will start at \p base_offset.
    [[nodiscard]] std::string format(uint8_t const *data, size_t size,
        uint64_t base_offset = 0) const;
    [[nodiscard]] std::string format(std::vector<uint8_t> const &data,
        uint64_t base_offset = 0) const;
};

/// Create a formatted hex dump of \p data.
///
/// The effective offset of data will start at \p base_offset.
[[nodiscard]] inline std::string hex_format_dump(uint8_t const *data,
    size_t size, uint64_t base_offset = 0)
{
    return HexDumper().format(data, size, base_offset);
}

[[nodiscard]] inline std::string hex_format_dump(std::vector<uint8_t> const &data,
    uint64_t base_offset = 0)
{
    return hex_format_dump(data.data(), data.size(), base_offset);
}

}
