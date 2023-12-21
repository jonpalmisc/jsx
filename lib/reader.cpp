//
//  jsx/reader.cpp
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

#include "jsx/reader.h"

namespace jsx {

BufferReader::BufferReader(uint8_t *data, uint64_t size, Endianness endianness)
    : Reader(endianness)
    , m_data(data)
    , m_size(size)
{
}

bool BufferReader::read(uint8_t *buffer, size_t size)
{
    if (m_offset + size > m_size)
        return false;

    std::memcpy(buffer, m_data + m_offset, size);
    seek_relative(size);

    return true;
}

FileReader::FileReader(std::FILE *stream, Endianness endianness)
    : Reader(endianness)
    , m_stream(stream)
    , m_owns_stream(false)
    , m_cached_size(0)
{
    std::fseek(m_stream, 0, SEEK_END);
    m_cached_size = std::ftell(m_stream);
    std::rewind(m_stream);
}

FileReader::FileReader(FileReader &&existing)
    : Reader(existing.m_default_endianness)
{
    m_stream = existing.m_stream;
    m_owns_stream = existing.m_owns_stream;
    m_cached_size = existing.m_cached_size;
    m_offset = existing.m_offset;

    // If the existing reader owned its stream before, it definitely doesn't
    // now; need to set this to false to ensure the old one doesn't try to
    // close the stream when it is destroyed.
    existing.m_owns_stream = false;
}

FileReader::~FileReader()
{
    if (m_owns_stream)
        std::fclose(m_stream);
}

std::optional<FileReader> FileReader::from_path(std::string const &path,
    Endianness endianness)
{
    auto stream = std::fopen(path.c_str(), "rb");
    if (!stream)
        return std::nullopt;

    auto reader = FileReader(stream, endianness);
    reader.m_owns_stream = true;

    return reader;
}

bool FileReader::read(uint8_t *buffer, size_t size)
{
    if (m_offset + size > m_cached_size)
        return false;

    std::fread(buffer, 1, size, m_stream);

    // This seeking is redundant since the underlying file stream behaves the
    // way we want, but is being left around for soundness reasons until
    // further testing has been done.
    seek_relative(size);

    return true;
}

}
