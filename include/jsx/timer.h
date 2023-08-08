//===-- jsx/timer.h - Simple wall clock timer -----------------------------===//
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

#pragma once

#include <chrono>
#include <string>

namespace jsx::timer {

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

/// Simple "high-resolution" timer for measuring elapsed time.
class Timer {
  TimePoint m_start;

public:
  /// Create a new timer.
  ///
  /// Automatically starts the timer if \p auto_start is true (default).
  Timer(bool auto_start = true);

  /// Reset the starting point of the timer to now.
  void reset();

  /// Get the time point the timer was started at.
  [[nodiscard]] TimePoint start() const;

  /// Get the amount of elapsed time (in milliseconds) since the timer was
  /// started.
  [[nodiscard]] uint64_t elapsed_ms() const;
};

/// Simple scoped-based "high-resolution" timer for measuring elapsed time.
///
/// Starts counting as soon as it is initialized (enters scope) and reports the
/// elapsed time when it falls out of scope. The elapsed time can either be
/// written to a pre-determined location or by executing a callback.
class ScopedTimer : public Timer {
  using TimeoutCallbackMs = std::function<void(uint64_t elapsed_ms)>;

  uint64_t *m_elapsed_ms_out;
  TimeoutCallbackMs m_on_destroy_callback;

public:
  /// Create a new scoped timer which writes the elapsed time (in milliseconds)
  /// to \p elapsed_ms_out upon falling out of scope.
  ScopedTimer(uint64_t *elapsed_ms_out);

  /// Create a new scoped timer which runs the provided \p on_destroy callback
  /// (with the elapsed time as a parameter) upon falling out of scope.
  ScopedTimer(TimeoutCallbackMs on_destroy);
  ~ScopedTimer();
};

} // namespace jsx::timer
