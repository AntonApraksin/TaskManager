file(GLOB_RECURSE Proto_SRCS CONFIGURE_DEPENDS
    "protobuf/*.proto"
)

foreach(proto_file ${Proto_SRCS})
  compile_proto_file(${proto_file})
  get_filename_component(basename ${proto_file} NAME_WE)
  set(PROTOBUF_GENERATED_SRCS ${PROTOBUF_GENERATED_SRCS}
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.h
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.cc
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.grpc.pb.h
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.grpc.pb.cc
        )
endforeach(proto_file)

set_source_files_properties(${Proto_SRCS} PROPERTIES COMPILE_OPTIONS "-Wno-unused-variable")
