cmake_minimum_required(VERSION 3.12) 
project(tappo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -O3")

set(TAPPO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/tappo)

# Include required headers.
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${TAPPO_INCLUDE_DIR})

# Specify all cpp files in the source directory as sources for convenience.
file(GLOB_RECURSE TAPPO_SRCS src/*.cpp)

# Example build flags that defaults to be opened.
set(TAPPO_BUILD_TEST ON)
set(TAPPO_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
if(TAPPO_BUILD_TEST)
  add_executable(parser ${TAPPO_TEST_DIR}/parser.cpp)
endif()

# Install settings.
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
install(DIRECTORY tappo/ DESTINATION tappo)
