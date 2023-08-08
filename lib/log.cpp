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

namespace jsx::log {

class Config {
public:
  Level level;
  std::unordered_map<Feature, bool> features;

  Config() : level(Level::Info) {
    features[Feature::Highlighting] = true;
    features[Feature::Timestamps] = false;
  }
};

static Config g_config;

void set_level(Level level) { g_config.level = level; }

namespace ansi {

namespace escape {

constexpr auto fg_red = "\x1b[31m";
constexpr auto fg_yellow = "\x1b[33m";
constexpr auto fg_blue = "\x1b[34m";
constexpr auto fg_reset = "\x1b[0m";

} // namespace escape

void set_color(FILE *stream, Level level) {
  if (!g_config.features[Feature::Highlighting])
    return;

  switch (level) {
  case Level::Error:
    std::fprintf(stream, escape::fg_red);
    break;
  case Level::Warning:
    std::fprintf(stream, escape::fg_yellow);
    break;
  case Level::Debug:
    std::fprintf(stream, escape::fg_blue);
    break;
  default:
    break;
  }
}

void clear_color(FILE *stream) {
  if (!g_config.features[Feature::Highlighting])
    return;

  std::fprintf(stream, escape::fg_reset);
}

} // namespace ansi

void log_internal(Level level, char const *format, va_list args) {
  auto stream = level == Level::Error ? stderr : stdout;

  ansi::set_color(stream, level);
  std::vfprintf(stream, format, args);
  ansi::clear_color(stream);
  std::fprintf(stream, "\n");
}

#define INTERNAL_LOG_BODY(_level)                                              \
  std::va_list args;                                                           \
  if ((_level) > g_config.level)                                               \
    return;                                                                    \
  va_start(args, format);                                                      \
  log_internal(_level, format, args);                                          \
  va_end(args);

void error(char const *format, ...) { INTERNAL_LOG_BODY(Level::Error); }
void info(char const *format, ...) { INTERNAL_LOG_BODY(Level::Info); }
void warn(char const *format, ...) { INTERNAL_LOG_BODY(Level::Warning); }
void debug(char const *format, ...) { INTERNAL_LOG_BODY(Level::Debug); }

} // namespace jsx::log
