cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(pdtasks)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

include_directories(src/Model)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  base_test
  tests/base.cc
)
target_link_libraries(
  base_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(base_test)
