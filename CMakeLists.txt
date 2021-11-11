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

add_subdirectory(TaskManager)
add_subdirectory(tests)
