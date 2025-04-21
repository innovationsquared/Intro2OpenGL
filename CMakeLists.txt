cmake_minimum_required(VERSION 3.10)
project(Cals_renderer VERSION 0.0.1)
set(OpenGL_GL_PREFERENCE GLVND)
set(SOURCES
src/main.cpp
src/glad.c)
include_directories(dependencies)
add_executable(Cals_renderer ${SOURCES})
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(Cals_renderer PRIVATE glfw OpenGL::GL)
