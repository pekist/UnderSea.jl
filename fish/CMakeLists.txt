cmake_minimum_required(VERSION 3.10)

project(fish CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(LLVM REQUIRED CONFIG)

set(FISH_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Location of fish")
set(FISH_SRC_LIST fish.cpp)

set(FISH_SRC "")
foreach(src_file ${FISH_SRC_LIST})
  set(src_file "${FISH_DIR}/src/${src_file}")
  list(APPEND FISH_SRC ${src_file})
endforeach()

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if(FISH_EMBED)
add_library(fish STATIC
  ${FISH_SRC})
else()
  add_executable(fish ${FISH_DIR}/src/main.cpp ${FISH_SRC} ${ASMJIT_SRC})
endif()

llvm_map_components_to_libnames(llvm_libs support core)
target_include_directories(fish PUBLIC ${FISH_DIR}/include)
target_link_libraries(fish ${llvm_libs})
