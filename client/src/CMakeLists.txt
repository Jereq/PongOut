cmake_minimum_required(VERSION 2.6)
project(PongOut)

# The version number
set(PongOut_VERSION_MAJOR 0)
set(PongOut_VERSION_MINOR 0)
set(PongOut_VERSION_PATCH 1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(
	-std=c++11
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(WIN32_EXECUTABLE ON)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(PLATFORM_PATH "Platform_${CMAKE_SYSTEM_NAME}")

add_subdirectory(${PLATFORM_PATH})
include_directories(${PLATFORM_PATH})

set(EXTRA_LIBS ${EXTRA_LIBS} Graphics)

# Network code is easier if restricted to only Little Endian
include(TestBigEndian)
TEST_BIG_ENDIAN(isBigEndian)
if(isBigEndian)
	message(FATAL_ERROR "Big Endian machines currently not supported. Contact the developers for added support.")
endif(isBigEndian)

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/PongOutConfig.h.in"
	"${PROJECT_BINARY_DIR}/PongOutConfig.h"
)

# Add the binary tree to the search path for include files so that we will find PongOutConfig.h
include_directories("${PROJECT_BINARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}")

set(Boost_USE_STATIC_LIBS		ON)
set(Boost_USE_MULTITHREADED		ON)
set(Boost_USE_STATIC_RUNTIME	ON)
find_package(Boost 1.49 COMPONENTS filesystem system REQUIRED)

include_directories("${Boost_INCLUDE_DIRS}")
set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

set(SOURCE_FILES
	"${PLATFORM_PATH}/main.cpp"
)

# Add the executable
add_executable(PongOut ${SOURCE_FILES})

target_link_libraries(PongOut ${EXTRA_LIBS})
