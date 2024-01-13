cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_CXX_STANDARD  17)

set(CMAKE_BUILD_TYPE Debug)

project(4eq10_solver)

set(NOTATION_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/notation)
file(GLOB NOTATION_SRC ${NOTATION_SRC_DIR}/*.cpp)

set(PERMUTATION_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/permutation)
file(GLOB PERMUTATION_SRC ${PERMUTATION_SRC_DIR}/*.cpp)

add_executable(4eq10_solver     src/main.cpp
                                src/permutation/permutation.cpp
                                ${NOTATION_SRC}
                                ${PERMUTATION_SRC}
)

# target_include_directories(4eq10_solver
#     PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/notation
# )