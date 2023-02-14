cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME glut_test)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCE_FILES src/def.h src/main.cpp)
add_executable(glut_test ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW glfw::glfw)