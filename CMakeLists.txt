cmake_minimum_required(VERSION 3.12)
project(gitfed CXX)

# Enable userver libraries that are needed in this project
set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

# Add userver dependency
add_subdirectory(third_party/userver)
userver_setup_environment()

