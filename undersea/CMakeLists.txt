cmake_minimum_required(VERSION 3.10)

project(UnderSea)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(FISH_DIR "../fish/")
include("${FISH_DIR}/CMakeLists.txt")

set(UNDERSEA_SRC
  src/Main.cpp
  src/attributes.cpp
  src/buffers.cpp
  src/game_state.cpp
  src/glad.c
  src/shader.cpp
  src/shader_program.cpp
  src/texture.cpp
  src/window.cpp
  src/world_chunk.cpp)

set(UNDERSEA_DEPS dl glfw)

add_executable(under-sea
  ${UNDERSEA_SRC})

target_include_directories(under-sea PRIVATE include)

target_link_libraries(under-sea ${UNDERSEA_DEPS} ${ASMJIT_DEPS})

file(COPY res/ DESTINATION "${CMAKE_BINARY_DIR}/res")
