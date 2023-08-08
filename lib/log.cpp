//===-- jsx/log.cpp - Formatted logging interface -------------------------===//
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

#include <jsx/log.h>

#include <cstdarg>
#include <cstdio>
#include <unordered_map>

namespace jsx {

class Config {
public:
  LogLevel level;
  std::unordered_map<LogOption, bool> features;

  Config() : level(LogLevel::Info) {
    features[LogOption::Highlighting] = true;
    features[LogOption::Timestamps] = false;
  }
};

static Config g_config;

void set_log_level(LogLevel level) { g_config.level = level; }

namespace ansi_escape {

constexpr auto fg_red = "\x1b[31m";
constexpr auto fg_yellow = "\x1b[33m";
constexpr auto fg_blue = "\x1b[34m";
constexpr auto fg_reset = "\x1b[0m";

} // namespace ansi_escape

void set_output_color(FILE *stream, LogLevel level) {
  if (!g_config.features[LogOption::Highlighting])
    return;

  switch (level) {
  case LogLevel::Error:
    std::fprintf(stream, ansi_escape::fg_red);
    break;
  case LogLevel::Warning:
    std::fprintf(stream, ansi_escape::fg_yellow);
    break;
  case LogLevel::Debug:
    std::fprintf(stream, ansi_escape::fg_blue);
    break;
  default:
    break;
  }
}

void clear_output_color(FILE *stream) {
  if (!g_config.features[LogOption::Highlighting])
    return;

  std::fprintf(stream, ansi_escape::fg_reset);
}

void log_internal(LogLevel level, char const *format, va_list args) {
  auto stream = level == LogLevel::Error ? stderr : stdout;

  set_output_color(stream, level);
  std::vfprintf(stream, format, args);
  clear_output_color(stream);
  std::fprintf(stream, "\n");
}

#define INTERNAL_LOG_BODY(_level)                                              \
  std::va_list args;                                                           \
  if ((_level) > g_config.level)                                               \
    return;                                                                    \
  va_start(args, format);                                                      \
  log_internal(_level, format, args);                                          \
  va_end(args);

void log_error(char const *format, ...) { INTERNAL_LOG_BODY(LogLevel::Error); }
void log_info(char const *format, ...) { INTERNAL_LOG_BODY(LogLevel::Info); }
void log_warn(char const *format, ...) { INTERNAL_LOG_BODY(LogLevel::Warning); }
void log_debug(char const *format, ...) { INTERNAL_LOG_BODY(LogLevel::Debug); }

} // namespace jsx
