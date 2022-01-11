Find_Package(GTest REQUIRED)

set(TASK_MANAGER_LINK_GTEST
  GTest::gtest GTest::gtest_main GTest::gmock)

file(GLOB_RECURSE Tests_SRCS CONFIGURE_DEPENDS
    "*.h"
    "*.cc"
  )

add_executable(ModelTest
  ${Tests_SRCS})

target_include_directories(ModelTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ModelTest
  PRIVATE
  ${TASK_MANAGER_LINK_GTEST}
  TaskManager
)

gtest_discover_tests(ModelTest)
