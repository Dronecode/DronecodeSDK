find_program(PROTOC_BINARY 3.7.1 protoc HINTS ${CMAKE_SOURCE_DIR}/build/default/third_party/install/bin)
find_program(GRPC_CPP_PLUGIN_BINARY grpc_cpp_plugin HINTS ${CMAKE_SOURCE_DIR}/build/default/third_party/install/bin)

if(NOT EXISTS ${PROTOC_BINARY} OR NOT EXISTS ${GRPC_CPP_PLUGIN_BINARY})
    message(FATAL_ERROR "Could not find 'protoc' or 'grpc_cpp_plugin' binaries.")
endif()

function(compile_proto_pb COMPONENT_NAME PB_COMPILED_SOURCE)
    add_custom_command(OUTPUT ${COMPONENT_NAME}/${COMPONENT_NAME}.pb.cc
        DEPENDS ${PROTO_DIR}/${COMPONENT_NAME}/${COMPONENT_NAME}.proto
        COMMAND ${PROTOC_BINARY}
            -I ${PROTO_DIR}
            --cpp_out=.
            ${PROTO_DIR}/${COMPONENT_NAME}/${COMPONENT_NAME}.proto
    )

    set(PB_COMPILED_SOURCE ${COMPONENT_NAME}/${COMPONENT_NAME}.pb.cc PARENT_SCOPE)
endfunction()

function(compile_proto_grpc COMPONENT_NAME GRPC_COMPILED_SOURCE)
    add_custom_command(OUTPUT ${COMPONENT_NAME}/${COMPONENT_NAME}.grpc.pb.cc
        DEPENDS ${PROTO_DIR}/${COMPONENT_NAME}/${COMPONENT_NAME}.proto
        COMMAND ${PROTOC_BINARY}
            -I ${PROTO_DIR}
            --grpc_out=.
            --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_BINARY}
            ${PROTO_DIR}/${COMPONENT_NAME}/${COMPONENT_NAME}.proto
    )

    set(GRPC_COMPILED_SOURCE ${COMPONENT_NAME}/${COMPONENT_NAME}.grpc.pb.cc PARENT_SCOPE)
endfunction()
