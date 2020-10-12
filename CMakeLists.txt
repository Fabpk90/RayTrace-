cmake_minimum_required(VERSION 3.16)
project(RayTracingOneWeekEnd)

set(CMAKE_CXX_STANDARD  20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(libs)

link_directories(libs)
include_directories(libs)
add_executable(RayTracingOneWeekEnd main.cpp Sphere.cpp shaders/Shader.cpp Ray.cpp Camera.cpp ThreadPool.cpp ThreadPool.hpp)
target_link_libraries(RayTracingOneWeekEnd glfw3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")