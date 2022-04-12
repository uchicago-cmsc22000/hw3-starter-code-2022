cmake_minimum_required(VERSION 3.5.1)
project(libgraph C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include/)

# libgraph.so

add_library(graph SHARED
        src/libgraph/graph.c
        src/libgraph/vlist.c
        src/libgraph/algorithms.c)

# best-first

add_executable(best-first
        src/tools/best-first.c)

target_link_libraries(best-first graph)

# toposort

add_executable(toposort
        src/tools/toposort.c)

target_link_libraries(toposort graph)
