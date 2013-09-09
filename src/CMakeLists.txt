cmake_minimum_required(VERSION 2.6)
project(PongOut)

# The version number
set(PongOut_VERSION_MAJOR 0)
set(PongOut_VERSION_MINOR 0)
set(PongOut_VERSION_BUGFIX 1)

add_definitions(
	-std=c++11
)

include_directories("${PROJECT_SOURCE_DIR}/Platform_${CMAKE_SYSTEM_NAME}")

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/PongOutConfig.h.in"
	"${PROJECT_BINARY_DIR}/PongOutConfig.h"
)

# Add the binary tree to the search path for include files so that we will find PongOutConfig.h
include_directories("${PROJECT_BINARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}")

# Add the executable
add_executable(PongOut "${PROJECT_SOURCE_DIR}/Platform_${CMAKE_SYSTEM_NAME}/main.cpp")

target_link_libraries(PongOut ${LIBRARIES})
