//
//  Copyright (c) 2022-2023 Jon Palmisciano. All rights reserved.
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

#include <jsx/log.h>

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define ANSI_COLOR_RED    "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_RESET  "\x1b[0m"

static struct Config {
    JsxLogLevel level;
    unsigned features;
    int epoch_was_initialized;
    struct timespec epoch;
} g_config = {
    .features = JsxLogFeatureHighlighting,
    .level = JsxLogLevelInfo,
};

static void set_terminal_fg_color(FILE *stream, JsxLogLevel level);
static void clear_terminal_fg_color(FILE *stream);
static inline void time_subtract(struct timespec *lhs, struct timespec *rhs, struct timespec *out);
static void log_timestamp(FILE *stream);
static void log_internal(JsxLogLevel level, char const *format, va_list args);

void jsx_log_set_level(JsxLogLevel level)
{
    g_config.level = level;
}

JsxLogLevel jsx_log_level()
{
    return g_config.level;
}

void jsx_log_enable_feature(JsxLogFeature feature)
{
    g_config.features |= feature;
}

void jsx_log_disable_feature(JsxLogFeature feature)
{
    g_config.features &= ~feature;
}

int jsx_log_has_feature(JsxLogFeature feature)
{
    return g_config.features & feature;
}

#define INTERNAL_LOG_BODY(_level)       \
    va_list args;                       \
    if ((_level) > g_config.level)      \
        return;                         \
    va_start(args, format);             \
    log_internal(_level, format, args); \
    va_end(args);

void jsx_log_info(char const *format, ...)
{
    INTERNAL_LOG_BODY(JsxLogLevelInfo)
}

void jsx_log_debug(char const *format, ...)
{
    INTERNAL_LOG_BODY(JsxLogLevelDebug)
}

void jsx_log_trace(char const *format, ...)
{
    INTERNAL_LOG_BODY(JsxLogLevelTrace)
}

void jsx_log_error(char const *format, ...)
{
    INTERNAL_LOG_BODY(JsxLogLevelError)
}

static void set_terminal_fg_color(FILE *stream, JsxLogLevel level)
{
    if (!jsx_log_has_feature(JsxLogFeatureHighlighting))
        return;

    switch (level) {
    case JsxLogLevelError:
        fprintf(stream, ANSI_COLOR_RED);
        break;
    case JsxLogLevelDebug:
        fprintf(stream, ANSI_COLOR_YELLOW);
        break;
    case JsxLogLevelTrace:
        fprintf(stream, ANSI_COLOR_BLUE);
        break;
    default:
        break;
    }
}

static void clear_terminal_fg_color(FILE *stream)
{
    if (!jsx_log_has_feature(JsxLogFeatureHighlighting))
        return;

    fprintf(stream, ANSI_COLOR_RESET);
}

static inline void time_subtract(struct timespec *lhs, struct timespec *rhs, struct timespec *out)
{
    out->tv_sec = lhs->tv_sec - rhs->tv_sec;
    out->tv_nsec = lhs->tv_nsec - rhs->tv_nsec;

    if (out->tv_nsec < 0) {
        --out->tv_sec;
        out->tv_nsec += 1000000000L;
    }
}

static void log_timestamp(FILE *stream)
{
    struct timespec now, elapsed;

    if (!g_config.epoch_was_initialized) {
        clock_gettime(CLOCK_MONOTONIC, &g_config.epoch);
        g_config.epoch_was_initialized = 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &now);
    time_subtract(&now, &g_config.epoch, &elapsed);

    fprintf(stream, "%04lu.%06lu: ", elapsed.tv_sec, elapsed.tv_nsec / 1000);
}

static void log_internal(JsxLogLevel level, char const *format, va_list args)
{
    FILE *stream = level == JsxLogLevelError ? stderr : stdout;

    set_terminal_fg_color(stream, level);
    if (jsx_log_has_feature(JsxLogFeatureTimestamps))
        log_timestamp(stream);
    vfprintf(stream, format, args);
    clear_terminal_fg_color(stream);

    fprintf(stream, "\n");
}
