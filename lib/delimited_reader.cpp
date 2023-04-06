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

#include <jsx/delimited_reader.hpp>

using namespace jsx;

DelimitedReader::DelimitedReader(std::string_view data, char field_delimiter, char group_delimiter,
    char quote_char, char escape_char)
    : m_data(data)
    , m_point(0)
    , m_field_delim(field_delimiter)
    , m_group_delim(group_delimiter)
    , m_quote_char(quote_char)
    , m_escape_char(escape_char)
{
}

std::optional<std::string_view> DelimitedReader::next_field_internal(bool preserve_group_delim)
{
    auto mark = m_point;

    // TODO: Make this a for loop.
    char last_char = 0;
    while (m_point < m_data.size()) {
        last_char = m_data[m_point++];

        // Stop when a field or group delimiter is reached.
        if (last_char == m_field_delim || last_char == m_group_delim)
            break;
    }

    auto span = m_point - mark;

    // If the last character read was a delimiter, the span should be reduced
    // by one to avoid including it in the returned field. However, if the
    // stream ended abruptly, the last read character is actually part of the
    // field and should be included.
    if (last_char == m_field_delim || last_char == m_group_delim) {
        --span;

        // If a group delimiter was reached, the point was advanced past it in
        // the loop above; if the caller has requested to "preserve" the group
        // delimiter, roll back the point by one character.
        if (last_char == m_group_delim && preserve_group_delim)
            --m_point;
    }

    return m_data.substr(mark, span);
}

std::optional<std::string_view> DelimitedReader::next_field()
{
    return next_field_internal();
}

std::vector<std::string_view> DelimitedReader::next_group()
{
    std::vector<std::string_view> fields;
    while (m_point < m_data.size()) {
        if (m_data[m_point] == m_group_delim) {
            ++m_point;
            break;
        }

        auto field = next_field_internal(true);
        if (!field)
            break;

        fields.push_back(*field);
    }

    return fields;
}

void DelimitedReader::reset()
{
    m_point = 0;
}

void DelimitedReader::set_field_delimiter(char delimiter)
{
    m_field_delim = delimiter;
}

void DelimitedReader::set_group_delimiter(char delimiter)
{
    m_group_delim = delimiter;
}

void DelimitedReader::set_quote_char(char character)
{
    m_quote_char = character;
}

void DelimitedReader::set_escape_char(char character)
{
    m_escape_char = character;
}
