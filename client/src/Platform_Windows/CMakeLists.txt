set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/module/")

find_package(Directx11 REQUIRED)



set(PLATFORM_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	PARENT_SCOPE
)

include_directories(
	${DirectX_D3D11_INCLUDE_DIR}
	${DirectX_D3DX11_INCLUDE_DIR}
	${DirectX_D3DX10_INCLUDE_DIR}
)

add_library(Graphics GraphicsWindows.cpp D3D.cpp TestShader.cpp)

target_link_libraries(Graphics
	${DirectX_D3DX11_LIBRARY}
	${DirectX_D3D11_LIBRARY}
	${DirectX_D3DX10_LIBRARY}
	${DirectX_D3D11_LIBRARY}
	${DirectX_DXGI_LIBRARY}
)