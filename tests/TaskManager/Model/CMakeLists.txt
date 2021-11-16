add_executable(
  task_test
  task.cc
)

add_executable(
  id_test
  id.cc
)

add_executable(
  task_manager_test
  task_manager.cc
)

target_link_libraries(
  task_test 
  PRIVATE
  ${TASK_MANAGER_LINK_GTEST}
  TaskManager
)

target_link_libraries(
  id_test
  PRIVATE
  ${TASK_MANAGER_LINK_GTEST}
  TaskManager
)

target_link_libraries(
  task_manager_test
  PRIVATE
  ${TASK_MANAGER_LINK_GTEST}
  TaskManager
)

gtest_discover_tests(task_test)
gtest_discover_tests(id_test)
gtest_discover_tests(task_manager_test)
