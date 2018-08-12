cmake_minimum_required(VERSION 3.5)
project(process)

option(PROCESS_TESTS "Run unit tests" OFF)
if(PROCESS_TESTS)
  add_subdirectory(test)
endif()

file(GLOB srcs src/*.cpp)
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${srcs})
target_include_directories(${PROJECT_NAME} INTERFACE include)
