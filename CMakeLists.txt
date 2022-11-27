cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Dandelion VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB ${PROJECT_NAME}_SOURCES
	src/*.cpp
	src/fixes/*.cpp
	src/modifications/*.cpp
	src/optimizations/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
	include
	include/Dandelion
	include/Dandelion/fixes
	include/Dandelion/modifications
	include/Dandelion/optimizations
)


set(GEODE_LINK_NIGHTLY On)
set(GEODE_LINK_SOURCE On)
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_BINARY_DIR}/geode)
target_link_libraries(${PROJECT_NAME} geode-sdk)
create_geode_file(${PROJECT_NAME})
