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
  src/model/TaskId.cc
  src/model/TaskIdProducer.cc
  src/model/Task.cc
  src/model/TaskDate.cc
  src/model/TaskManager.cc
  src/model/TaskId.h
  src/model/TaskIdProducer.h
  src/model/Task.h
  src/model/TaskDate.h
  src/model/TaskManager.h
)

add_library(TaskManager
  STATIC ${TaskManager_SRCS}
)

target_include_directories(TaskManager
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(tests)
