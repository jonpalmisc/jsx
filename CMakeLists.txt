cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(jsx LANGUAGES CXX)

function(add_jsx_library NAME)
  add_library(jsx_${NAME} include/jsx/${NAME}.h lib/${NAME}.cpp)
  target_include_directories(jsx_${NAME} PUBLIC include)
  target_compile_features(jsx_${NAME} PRIVATE cxx_std_17)
  install(TARGETS jsx_${NAME} DESTINATION lib)
endfunction()

add_jsx_library(hex)
add_jsx_library(log)
add_jsx_library(timer)

install(DIRECTORY include/jsx DESTINATION include)

add_executable(playground test/playground.cpp)
target_compile_features(playground PRIVATE cxx_std_17)
target_link_libraries(playground PUBLIC jsx_hex jsx_log jsx_timer)
