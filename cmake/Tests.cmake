enable_testing()

file(GLOB_RECURSE Tests_SRCS CONFIGURE_DEPENDS
    "tests/model/*.h"
    "tests/model/*.cc"
    "tests/client/*.h"
    "tests/client/*.cc"
    "tests/service/*.h"
    "tests/service/*.cc"
    "tests/interactor/*.h"
    "tests/interactor/*.cc"
    "tests/persistence/*.h"
    "tests/persistence/*.cc"
    "tests/test_utils/*.h"
    "tests/test_utils/*.cc"
  )

add_executable(test_main
  tests/main.cc
  ${Tests_SRCS})

target_include_directories(test_main PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(test_main
  TaskManager
  GTest::gtest
  GTest::gmock
)
