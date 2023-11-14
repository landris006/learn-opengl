cmake_minimum_required(VERSION 3.27.7)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=1)

project(learn-opengl)

add_executable(${PROJECT_NAME} src/main.cpp src/lib.cpp src/shader.cpp dependencies/glad/src/glad.c)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} OpenGL::GL)
target_include_directories(${PROJECT_NAME} PUBLIC include dependencies/stb_image dependencies/glfw/include dependencies/glad/include)
