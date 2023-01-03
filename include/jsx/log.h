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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define JSX_LOG_FORMAT __attribute__((format(printf, 1, 2)))

/// Log output level options.
typedef enum {
    /// Do not show any log messages.
    JsxLogLevelNone = 0,

    /// Only show error messages.
    JsxLogLevelError = 1,

    /// Show info messages and error messages.
    JsxLogLevelInfo = 2,

    /// Show debug, info, and error messages.
    JsxLogLevelDebug = 3,

    /// Show trace, debug, info, and error messages.
    JsxLogLevelTrace = 4
} JsxLogLevel;

/// Output options/feature flags.
typedef enum : unsigned {
    /// Highlighted output via ANSI escape sequences.
    JsxLogFeatureHighlighting = 1 << 0,

    /// Timestamp-prefixed log messages.
    JsxLogFeatureTimestamps = 1 << 1,
} JsxLogFeature;

/// Set the log output level.
void jsx_log_set_level(JsxLogLevel level);

/// Get the log output level.
JsxLogLevel jsx_log_level(void);

/// Enable an output feature.
void jsx_log_enable_feature(JsxLogFeature feature);

/// Disable an output feature.
void jsx_log_disable_feature(JsxLogFeature feature);

/// Check if an output feature is enabled.
int jsx_log_has_feature(JsxLogFeature feature);

//===----------------------------------------------------------------------===//

/// Log a formatted info message to the standard output stream.
JSX_LOG_FORMAT void jsx_log_info(char const *format, ...);

/// Log a formatted debug message to the standard output stream.
JSX_LOG_FORMAT void jsx_log_debug(char const *format, ...);

/// Log a formatted trace message to the standard output stream.
JSX_LOG_FORMAT void jsx_log_trace(char const *format, ...);

/// Log a formatted message to the standard error stream.
JSX_LOG_FORMAT void jsx_log_error(char const *format, ...);

#ifdef __cplusplus
}
#endif
