//
//  Copyright (c) 2023 Jon Palmisciano. All rights reserved.
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

#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace jsx {

class DelimitedReader {
    std::string_view m_data;
    size_t m_point;

    char m_field_delim;
    char m_group_delim;
    char m_quote_char;
    char m_escape_char;

    std::optional<std::string_view> next_field_internal(bool consume_group_delimiter = false);

public:
    DelimitedReader(std::string_view data, char field_delimiter = ',', char group_delimiter = '\n',
        char quote_char = '"', char escape_char = '\\');

    /// Attempt to get the next field present in the input.
    std::optional<std::string_view> next_field();

    /// Get the next group of fields present in the input.
    ///
    /// The number of fields returned is not necessarily guaranteed to match the
    /// number returned in any previous calls to this function.
    std::vector<std::string_view> next_group();

    /// Reset the reader's position to the start of the input.
    void reset();

    /// Set the field delimiter character.
    ///
    /// Calling this method in the middle of parsing is undefined behavior.
    void set_field_delimiter(char delimiter);

    /// Set the group delimiter character.
    ///
    /// Calling this method in the middle of parsing is undefined behavior.
    void set_group_delimiter(char delimiter);

    /// Set the field quote character.
    ///
    /// Calling this method in the middle of parsing is undefined behavior.
    void set_quote_char(char character);

    /// Set the escape character used in quoted fields.
    ///
    /// Calling this method in the middle of parsing is undefined behavior.
    void set_escape_char(char character);
};

}
