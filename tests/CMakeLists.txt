include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  base_test
  base.cc
)

target_link_libraries(
  base_test
  gtest_main
  TaskManager
)

include(GoogleTest)
gtest_discover_tests(base_test)
