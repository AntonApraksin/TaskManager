set(TaskManager_SRCS
  src/Model/id.cc
  src/Model/task.cc
  src/Model/task_manager.cc
  include/Model/id.h
  include/Model/task.h
  include/Model/task_manager.h
)

add_library(TaskManager
  STATIC ${TaskManager_SRCS}
)

target_include_directories(TaskManager
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
