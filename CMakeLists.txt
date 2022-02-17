cmake_minimum_required(VERSION 3.14)

project(TaskManager)

include(cmake/CheckCompiler.cmake)

include(cmake/CompileOptions.cmake)

include(cmake/Helpers.cmake)

include(cmake/Config.cmake)

include(cmake/Libraries.cmake)

include(cmake/Sanitizers.cmake)

include(cmake/Protobuf.cmake)

include(cmake/TaskManager.cmake)

add_executable(main
        src/main.cc)
target_link_libraries(main TaskManager)

add_executable(server
    src/server.cc)
target_link_libraries(server TaskManager)

add_executable(client
    src/client.cc)
target_link_libraries(client TaskManager)

add_executable(ddos_main
    src/ddos_main.cc)
target_link_libraries(ddos_main TaskManager)

include(cmake/Tests.cmake)
