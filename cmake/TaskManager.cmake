file(GLOB_RECURSE TaskManager_SRCS CONFIGURE_DEPENDS
    "src/model/*.h"
    "src/model/*.cc"
    "src/utils/*.h"
    "src/utils/*.cc"
    "src/client/*.h"
    "src/client/*.cc"
    "src/logging/*.h"
    "src/logging/*.cc"
    "src/service/*.h"
    "src/service/*.cc"
    "src/interactor/*.h"
    "src/interactor/*.cc"
    "src/persistence/*.h"
    "src/persistence/*.cc"
)

add_library(TaskManager
  STATIC ${TaskManager_SRCS} ${PROTOBUF_GENERATED_SRCS}
)
target_link_libraries(TaskManager PUBLIC
  protobuf::libprotobuf
  gRPC::grpc++
  gRPC::grpc++_reflection
  Boost::log
  Boost::log_setup
  Boost::program_options
)

target_include_directories(TaskManager PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PROTOBUF_CPP_OUT_DIR}
  ${PROTOBUF_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

