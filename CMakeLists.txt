cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# Collect all source files automatically
file(GLOB_RECURSE SRC_FILES 
    src/*.cpp
    src/*.c
    imgui/*.cpp
    imgui/*.c
)

add_executable(app ${SRC_FILES})

# Include directories
target_include_directories(app PRIVATE include)
target_include_directories(app PRIVATE include imgui)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(app PRIVATE OpenGL::GL GLEW::GLEW glfw)
# Copy assets folder to the build directory automatically
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets)