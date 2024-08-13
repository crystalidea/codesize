cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(codesize VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core)

# Add the executable
add_executable(codesize main.cpp helpers.cpp)

# Link the executable to the Qt libraries
target_link_libraries(codesize Qt6::Core)

# Include directories
target_include_directories(codesize PRIVATE ${CMAKE_SOURCE_DIR})

# Optionally, you can specify the output directory
set_target_properties(codesize PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)