cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(jsx LANGUAGES C CXX)

option(JSX_BUILD_TESTS "Build test programs" OFF)

add_library(jsx_delimited_reader STATIC include/jsx/delimited_reader.hpp lib/delimited_reader.cpp)
target_include_directories(jsx_delimited_reader PUBLIC include)
target_compile_features(jsx_delimited_reader PRIVATE cxx_std_17)

add_library(jsx_log STATIC include/jsx/log.h lib/log.c)
target_include_directories(jsx_log PUBLIC include)
target_compile_features(jsx_log PRIVATE c_std_99)

add_library(jsx_hex STATIC include/jsx/hex.h lib/hex.c)
target_include_directories(jsx_hex PUBLIC include)
target_compile_features(jsx_hex PRIVATE c_std_99)

add_library(jsx_test INTERFACE)
target_include_directories(jsx_test INTERFACE include)

if(JSX_BUILD_TESTS)
    add_executable(delimited-reader-test test/delimited_reader.cpp)
    target_link_libraries(delimited-reader-test PUBLIC jsx_delimited_reader)
    target_compile_features(delimited-reader-test PRIVATE cxx_std_17)

    add_executable(log-test test/log.c)
    target_link_libraries(log-test PUBLIC jsx_log)

    add_executable(hex-test test/hex.c)
    target_link_libraries(hex-test PUBLIC jsx_hex)

    add_executable(test-test test/test.c)
    target_link_libraries(test-test PUBLIC jsx_test)
endif()

file(GLOB JSX_SOURCE include/jsx/*.h include/jsx/*.hpp lib/*.c lib/*.cpp test/*.c test/*.cpp)
add_custom_target(format
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND clang-format -i ${JSX_SOURCE}
    COMMENT "Formatting all C/C++ files")
