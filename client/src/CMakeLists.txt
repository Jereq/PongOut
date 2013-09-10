cmake_minimum_required(VERSION 2.6)
project(PongOut)

# The version number
set(PongOut_VERSION_MAJOR 0)
set(PongOut_VERSION_MINOR 0)
set(PongOut_VERSION_PATCH 1)


add_definitions(
	-std=c++11
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(WIN32_EXECUTABLE ON)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Network code is easier if restricted to only Little Endian
include(TestBigEndian)
TEST_BIG_ENDIAN(isBigEndian)
if(isBigEndian)
	message(FATAL_ERROR "Big Endian machines currently not supported. Contact the developers for added support.")
endif(isBigEndian)

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
