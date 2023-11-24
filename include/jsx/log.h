//
//  jsx/log.h
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

namespace jsx {

/// Log output levels.
enum class LogLevel {
  None,
  Error,
  Warning,
  Info,
  Debug,
  Trace,
};

/// Optional output features.
enum class LogOption {
  /// Color log messages by type.
  Color,
};

/// Set the log output level.
void set_log_level(LogLevel level);

/// Enable or disable a log option.
void set_log_option(LogOption option, bool enabled);

#define JSX_LOG_FORMAT __attribute__((format(printf, 1, 2)))

/// Log a formatted message to the standard error stream.
JSX_LOG_FORMAT void log_error(char const *format, ...);

/// Log a formatted warning message to the standard output stream.
JSX_LOG_FORMAT void log_warn(char const *format, ...);

/// Log a formatted info message to the standard output stream.
JSX_LOG_FORMAT void log_info(char const *format, ...);

/// Log a formatted debug message to the standard output stream.
JSX_LOG_FORMAT void log_debug(char const *format, ...);

/// Log a formatted trace message to the standard output stream.
JSX_LOG_FORMAT void log_trace(char const *format, ...);

} // namespace jsx
