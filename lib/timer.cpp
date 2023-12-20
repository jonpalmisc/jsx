//
//  jsx/timer.cpp
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

#include <jsx/timer.h>

namespace jsx {

constexpr auto now = std::chrono::high_resolution_clock::now;

Timer::Timer(bool auto_start)
    : m_start(auto_start ? now() : Instant::min())
{
}

void Timer::reset()
{
    m_start = now();
}

Instant Timer::start() const
{
    return m_start;
}

uint64_t Timer::elapsed_ms() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(now() - m_start)
        .count();
}

ScopedTimer::ScopedTimer(uint64_t *elapsed_ms_out)
    : m_elapsed_ms_out(elapsed_ms_out)
    , m_on_destroy_callback(nullptr)
{
}

ScopedTimer::ScopedTimer(TimeoutCallbackMs on_destroy)
    : m_elapsed_ms_out(nullptr)
    , m_on_destroy_callback(std::move(on_destroy))
{
}

ScopedTimer::~ScopedTimer()
{
    if (m_on_destroy_callback) {
        m_on_destroy_callback(elapsed_ms());
    } else if (m_elapsed_ms_out) {
        *m_elapsed_ms_out = elapsed_ms();
    }
}

}
