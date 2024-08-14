cmake_minimum_required(VERSION 3.16)

project(codesize VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(3rdParty)

# Gather all source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(codesize ${SOURCES})

install(TARGETS codesize DESTINATION bin)