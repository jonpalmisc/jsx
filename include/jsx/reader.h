//
//  jsx/reader.h
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

namespace jsx {

/// Endianness preference options.
enum class Endianness {
    Default,
    Little,
    Big,
};

/// Abstract binary reader interface.
class Reader {
    static constexpr Endianness TARGET_ENDIANNESS = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        ? Endianness::Little
        : Endianness::Big;

    /// Swap the byte ordering of a primitive numeric type.
    template <typename IntTy>
    inline IntTy byte_swap(IntTy i);

    template <>
    inline uint8_t byte_swap<uint8_t>(uint8_t i)
    {
        return i;
    }

    template <>
    inline uint16_t byte_swap<uint16_t>(uint16_t i)
    {
        return __builtin_bswap16(i);
    }

    template <>
    inline uint32_t byte_swap<uint32_t>(uint32_t i)
    {
        return __builtin_bswap32(i);
    }

    template <>
    inline uint64_t byte_swap<uint64_t>(uint64_t i)
    {
        return __builtin_bswap64(i);
    }

protected:
    /// Default endianness to use when reading integer-like values.
    Endianness m_default_endianness;

    /// Current reading/cursor offset.
    uint64_t m_offset;

    explicit Reader(Endianness endianness)
        : m_default_endianness(endianness)
        , m_offset(0)
    {
    }

    /// Convert an integer to little endian form.
    template <typename IntTy>
    constexpr IntTy to_little_endian(IntTy value)
    {
        if constexpr (TARGET_ENDIANNESS == Endianness::Little)
            return value;
        else
            return byte_swap(value);
    }

    /// Convert an integer to big endian form.
    template <typename IntTy>
    constexpr IntTy to_big_endian(IntTy value)
    {
        if constexpr (TARGET_ENDIANNESS == Endianness::Little)
            return byte_swap(value);
        else
            return value;
    }

public:
    /// Read \p size bytes into \p buffer and advance the cursor.
    virtual bool read(uint8_t *buffer, size_t size) = 0;

    /// Read an integer-like value of type \p IntTy at the cursor offset,
    /// advancing the cursor by the size of \p IntTy.
    ///
    /// The value will be interpreted using the reader's default endianness
    /// unless an alternate \p endianness is passed.
    template <typename IntTy>
    bool read_int(IntTy &value, Endianness endianness = Endianness::Default)
    {
        IntTy raw_int;
        if (!read(reinterpret_cast<uint8_t *>(&raw_int), sizeof(IntTy)))
            return false;

        if (endianness == Endianness::Default)
            endianness = m_default_endianness;
        value = endianness == Endianness::Little
            ? to_little_endian(raw_int)
            : to_big_endian(raw_int);
        return true;
    }

    /// Read an object of type \p ObjectTy at the cursor offset, advancing the
    /// cursor by the size of \p ObjectTy.
    ///
    /// Object fields will not automatically be corrected for endianness.
    template <typename ObjectTy>
    bool read_object(ObjectTy &object)
    {
        return read(reinterpret_cast<uint8_t *>(&object), sizeof(ObjectTy));
    }

    /// Move the cursor to \p offset.
    virtual void seek(uint64_t offset) = 0;

    /// Move the cursor forward or backward by the \p delta.
    virtual void seek_relative(int64_t delta) = 0;

    /// Get the current offset (position) of the cursor.
    virtual uint64_t offset() const = 0;

    /// Get the length of the underlying buffer.
    virtual uint64_t size() const = 0;
};

/// Reader for in-memory buffers.
class BufferReader : public Reader {
    /// Pointer to underlying buffer.
    uint8_t *m_data;

    /// Size of underlying buffer.
    uint64_t m_size;

public:
    BufferReader(uint8_t *data, uint64_t size, Endianness endianness = Endianness::Big);

    bool read(uint8_t *buffer, size_t size) override;

    void seek(uint64_t offset) override
    {
        m_offset = offset;
    }

    void seek_relative(int64_t delta) override
    {
        seek(offset() + delta);
    }

    uint64_t offset() const override
    {
        return m_offset;
    }

    uint64_t size() const override
    {
        return m_size;
    }
};

/// Reader for reading binary files.
class FileReader : public Reader {
    /// Underlying file stream.
    std::FILE *m_stream;

    /// Indicates if the file stream is owned by the reader instance (and
    /// should therefore be closed when the reader is destroyed) or is borrowed
    /// from elsewhere in the program.
    bool m_owns_stream;

    /// File size is checked upon opening the stream and assumed to not change.
    uint64_t m_cached_size;

public:
    /// Create a new file reader wrapping \p stream.
    ///
    /// The caller is responsible for providing a valid pointer, as well as
    /// ensuring that other code does not utilize the provided stream while the
    /// reader is active. The reader does not own the file stream when
    /// constructed this way.
    ///
    /// Creating readers this way is discouraged and may be removed in the
    /// future in favor of \p from_path or a new implementation based on C++
    /// streams.
    FileReader(std::FILE *stream, Endianness endianness = Endianness::Big);

    FileReader(FileReader &existing) = delete;
    FileReader(FileReader &&existing);

    ~FileReader();

    /// Attempt to create a file reader for the file at \p path.
    static std::optional<FileReader> from_path(std::string const &path,
        Endianness endianness = Endianness::Big);

    bool read(uint8_t *buffer, size_t size) override;

    void seek(uint64_t offset) override
    {
        std::fseek(m_stream, offset, SEEK_SET);
        m_offset = offset;
    }

    void seek_relative(int64_t delta) override
    {
        seek(offset() + delta);
    }

    uint64_t offset() const override
    {
        return m_offset;
    }

    uint64_t size() const override
    {
        return m_cached_size;
    }
};

}
