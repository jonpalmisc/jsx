cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(jsx LANGUAGES C)

option(JSX_BUILD_TESTS "Build test programs" OFF)

add_library(jsx_log STATIC include/jsx/log.h lib/log.c)
target_include_directories(jsx_log PUBLIC include)

add_library(jsx_hex STATIC include/jsx/hex.h lib/hex.c)
target_include_directories(jsx_hex PUBLIC include)

if(JSX_BUILD_TESTS)
    add_executable(log-test test/log.c)
    target_link_libraries(log-test PUBLIC jsx_log)

    add_executable(hex-test test/hex.c)
    target_link_libraries(hex-test PUBLIC jsx_hex)
endif()
