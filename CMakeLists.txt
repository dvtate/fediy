cmake_minimum_required(VERSION 3.12)
project(gitfed CXX)

set(CMAKE_CXX_STANDARD 20)

# Enable userver libraries that are needed in this project
set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

# Add userver dependency
add_subdirectory(third_party/userver)
userver_setup_environment()

