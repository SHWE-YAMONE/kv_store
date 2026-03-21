cmake_minimum_required(VERSION 3.10)
project(KV_Store)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

find_package(Threads REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(kv_store ${SOURCES})

target_link_libraries(kv_store PRIVATE Threads::Threads)

target_compile_options(kv_store PRIVATE -Wall -Wextra -Wpedantic)