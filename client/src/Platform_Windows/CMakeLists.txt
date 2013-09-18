set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/module/")

find_package(Directx11 REQUIRED)

add_library(Graphics GraphicsWindows.cpp D3D.cpp)


set(PLATFORM_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	PARENT_SCOPE
)

include_directories(
	${DXSDK_DIR}
)