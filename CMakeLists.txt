cmake_minimum_required(VERSION 3.12)
project(gitfed CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/seastar)

file(GLOB protocol_server_sources protocol/*.cpp)
add_executable(protocol_server ${protocol_server_sources}
  protocol/ProtocolActions.cpp
  protocol/ProtocolActions.hpp
  protocol/Mod.cpp
  protocol/Mod.hpp
  protocol/ModMgr.cpp
  protocol/ModMgr.hpp
  protocol/Server.cpp
)
target_include_directories(protocol_server PUBLIC protocol third_party/SQLiteCpp/include third_party/json/single_include)
target_link_libraries(protocol_server
  Seastar::seastar
  SQLiteCpp
  sqlite3
  pthread
  dl
  crypto
)
