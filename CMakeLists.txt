cmake_minimum_required(VERSION 3.14)

project(TaskManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_compile_options(-Werror -Wno-language-extension-token)

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

enable_testing()

set(TaskManager_SRCS
        src/model/id/TaskId.cc
        src/model/id/TaskIdProducer.cc
        src/model/task/Task.cc
        src/model/task/TaskDate.cc
        src/model/task_manager/TaskManager.cc
        src/model/id/TaskId.h
        src/model/id/TaskIdProducer.h
        src/model/task/Task.h
        src/model/task/TaskDate.h
        src/model/task_manager/TaskManager.h
)

add_library(TaskManager
  STATIC ${TaskManager_SRCS}
)

target_include_directories(TaskManager
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(tests)
