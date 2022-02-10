function(compile_proto_file filename)
  get_filename_component(dirname ${filename} DIRECTORY)
  get_filename_component(basename ${filename} NAME_WE)

  add_custom_command(
      OUTPUT ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.cc ${PROTOBUF_CPP_OUT_DIR}/${basename}.pb.h
      ${PROTOBUF_CPP_OUT_DIR}/${basename}.grpc.pb.cc ${PROTOBUF_CPP_OUT_DIR}/${basename}.grpc.pb.h
      DEPENDS ${PROTOBUF_DIR}/${basename}.proto
      COMMAND ${PROTOBUF_EXECUTABLE} ${PROTOBUF_DIR}/${basename}.proto
        --cpp_out=${PROTOBUF_CPP_OUT_DIR}
        --grpc_out=generate_mock_code=true:${PROTOBUF_CPP_OUT_DIR}
        --plugin=protoc-gen-grpc=${GRPC_PROTOBUF_PLUGIN}
        --proto_path=${PROTOBUF_DIR}
      VERBATIM
  )
endfunction(compile_proto_file)
