cmake_minimum_required(VERSION 3.14)

project(TaskManager)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Protobuf REQUIRED)
find_program(PROTOBUF_EXECUTABLE protoc)

set(PROTOBUF_DIR ${CMAKE_SOURCE_DIR}/protobuf)
set(PROTOBUF_CPP_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/protobuf)

file(MAKE_DIRECTORY ${PROTOBUF_CPP_OUT_DIR})

function(compile_proto_file filename)
  get_filename_component(dirname ${filename} DIRECTORY)
  get_filename_component(basename ${filename} NAME_WE)

  add_custom_command(
      OUTPUT ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.cc ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.h
      DEPENDS ${PROTOBUF_DIR}/${basename}.proto
      COMMAND ${PROTOBUF_EXECUTABLE} ${PROTOBUF_DIR}/${basename}.proto
        --cpp_out=${PROTOBUF_CPP_OUT_DIR}
        --proto_path=${PROTOBUF_DIR}
      VERBATIM
  )
endfunction(compile_proto_file)

file(GLOB_RECURSE Proto_SRCS CONFIGURE_DEPENDS
    "protobuf/*.proto"
)

set_source_files_properties(${Proto_SRCS} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Wpedantic -Werror -Wno-unused-variable)

foreach(proto_file ${Proto_SRCS})
  compile_proto_file(${proto_file})
  get_filename_component(basename ${proto_file} NAME_WE)
  set(PROTOBUF_GENERATED_SRCS ${PROTOBUF_GENERATED_SRCS}
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.h
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.cc
)
endforeach(proto_file)

file(GLOB_RECURSE TaskManager_SRCS CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.cc"
)
set_source_files_properties(TaskManager_SRCS PROPERTIES COMPILE_FLAGS -Wall -Wextra -Wpedantic -Werror -Wno-language-extension-token)

add_library(TaskManager
  STATIC ${TaskManager_SRCS} ${PROTOBUF_GENERATED_SRCS}
)

add_executable(main
        src/main.cc)

target_link_libraries(main TaskManager protobuf::libprotobuf)

target_include_directories(TaskManager
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PROTOBUF_CPP_OUT_DIR}
    ${PROTOBUF_INCLUDE_DIR}
)

add_subdirectory(tests)