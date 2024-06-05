cmake_minimum_required(VERSION 3.12)

project(fediy)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/seastar)

file(GLOB protocol_server_sources protocol/*.cpp)
add_executable(protocol_server ${protocol_server_sources}
    third_party/inih/ini.c
    protocol/ProtocolActions.cpp
    protocol/ProtocolActions.hpp
    protocol/Mod.cpp
    protocol/Mod.hpp
    protocol/ModMgr.cpp
    protocol/ModMgr.hpp
    protocol/Server.cpp
    protocol/routes.cpp
    protocol/routes.hpp
    util/parse_cookie_header.hpp
    protocol/LocalUser.cpp
        protocol/Pages.cpp
        protocol/Pages.hpp
)


# Debug/vs release
target_compile_definitions(protocol_server PRIVATE
        $<$<CONFIG:Debug>:FEDIY_DEBUG=1>
)
target_compile_options(protocol_server PRIVATE
        $<$<CONFIG:Debug>:-O0;-g>
        $<$<CONFIG:Release>:-O3>
)

target_include_directories(protocol_server PUBLIC protocol
    third_party/SQLiteCpp/include
    third_party/json/single_include
    third_party/inja/single_include
    third_party/inih
    third_party/Mustache
)
target_link_libraries(protocol_server
    Seastar::seastar
    SQLiteCpp
    sqlite3
    pthread
    dl
    crypto
)