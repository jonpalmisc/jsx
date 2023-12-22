cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(jsx LANGUAGES CXX)

function(add_jsx_library name)
  list(APPEND JSX_LIBRARIES jsx_${name})
  set(JSX_LIBRARIES ${JSX_LIBRARIES} PARENT_SCOPE)

  add_library(jsx_${name} include/jsx/${name}.h lib/${name}.cpp)
  target_include_directories(jsx_${name} PUBLIC include)
  target_compile_features(jsx_${name} PRIVATE cxx_std_17)
  install(TARGETS jsx_${name} DESTINATION lib)

  add_library(jsx::${name} ALIAS jsx_${name})
endfunction()

function(add_jsx_test name)
  add_executable(test_${name} test/${name}.cpp)
  target_compile_features(test_${name} PRIVATE cxx_std_17)
  target_link_libraries(test_${name} PUBLIC ${JSX_LIBRARIES})
endfunction()

add_jsx_library(hex)
add_jsx_library(log)
add_jsx_library(reader)
add_jsx_library(timer)

add_jsx_test(hex)
add_jsx_test(log)
add_jsx_test(reader)
add_jsx_test(timer)

install(DIRECTORY include/jsx DESTINATION include)
