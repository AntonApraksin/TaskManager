find_package(Protobuf REQUIRED)
find_program(PROTOBUF_EXECUTABLE protoc)

find_package(gRPC REQUIRED)
find_program(GRPC_PROTOBUF_PLUGIN grpc_cpp_plugin)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS log log_setup program_options REQUIRED)

find_package(GTest REQUIRED)
