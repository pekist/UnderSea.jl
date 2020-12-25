cmake_minimum_required(VERSION 3.10)

project(UnderSea)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(under-sea src/Main.cpp
                         src/attributes.cpp
                         src/buffers.cpp
                         src/game_state.cpp
                         src/glad.c
                         src/shader.cpp
                         src/shader_program.cpp
                         src/texture.cpp
                         src/window.cpp
                         src/world_chunk.cpp)

target_include_directories(under-sea PRIVATE include)

target_link_libraries(under-sea PUBLIC dl)
target_link_libraries(under-sea PUBLIC glfw)

file(COPY res/ DESTINATION "${CMAKE_BINARY_DIR}/res")
