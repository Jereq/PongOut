cmake_minimum_required(VERSION 2.6)

# Project name
project(PongOut)

# The version number
set(PongOut_VERSION_MAJOR 0)
set(PongOut_VERSION_MINOR 0)
set(PongOut_VERSION_PATCH 1)

# Set default configuration type
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Handle different OS

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
else()
	add_definitions(-std=c++11)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

add_definitions(-DBOOST_ALL_NO_LIB)

set(Boost_USE_STATIC_LIBS		ON)
set(Boost_USE_MULTITHREADED		ON)
set(Boost_USE_STATIC_RUNTIME	ON)

find_package(Boost 1.49 COMPONENTS filesystem system REQUIRED)
find_package(GLM REQUIRED)

include_directories(
	${Boost_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
)

# Use platform dependent folder for platform dependent code.
set(PLATFORM_PATH "Platform_${CMAKE_SYSTEM_NAME}")

add_subdirectory(${PLATFORM_PATH})

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

set(EXTRA_LIBS ${EXTRA_LIBS}
	${Boost_LIBRARIES}
)

set(SOURCE_FILES
	main.cpp
	ICoreSystem.cpp ICoreSystem.h
	Game.cpp Game.h
	Map.cpp Map.h
	Block.cpp Block.h
)

# Add the executable
add_executable(PongOut ${SOURCE_FILES})

target_link_libraries(PongOut ${EXTRA_LIBS})

add_subdirectory(test)
add_custom_target(all_tests ALL
	DEPENDS PongOut-test
)
add_custom_command(TARGET all_tests
	COMMENT "Run tests"
	POST_BUILD COMMAND PongOut-test
)

add_custom_target(CopyResources ALL
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/../resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/resources
)
