cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# Collect all source files automatically
file(GLOB_RECURSE SRC_FILES 
    src/*.cpp
    src/*.c
)

add_executable(app ${SRC_FILES})

# Include directories
target_include_directories(app PRIVATE include)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(app PRIVATE OpenGL::GL glfw)