set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CORE_SOURCES
		CoreSystemWindows.cpp CoreSystemWindows.h
	)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(GLFW3 REQUIRED)

	include_directories(
		${GLFW3_INLCUDE_PATH}
	)

	set(CORE_LIBS
		${GLFW3_LIBRARIES}
	)

	set(CORE_SOURCES
		CoreSystemLinux.cpp
	)
endif()

list(APPEND CORE_SOURCES
	ICoreSystem.cpp ICoreSystem.h
)

add_library(CoreSystem
	${CORE_SOURCES}
)

target_link_libraries(CoreSystem
	${CORE_LIBS}
	Graphics
	Sound
)

