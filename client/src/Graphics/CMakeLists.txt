set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	find_package(Directx11 REQUIRED)

	include_directories(
		${DirectX_D3D11_INCLUDE_DIR}
		${DirectX_D3DX11_INCLUDE_DIR}
		${DirectX_D3DX10_INCLUDE_DIR}
	)

	set(GRAPHICS_SOURCES
		GraphicsWindows.cpp GraphicsWindow.h
		D3D.cpp D3D.h
		TestShader.cpp TestShader.h
		ErrorCodeList.h ErrorCodeList.h
		DXAssetInstancing.h
	)

	set(GRAPHICS_LIBS
		${DirectX_D3DX11_LIBRARY}
		${DirectX_D3D11_LIBRARY}
		${DirectX_D3DX10_LIBRARY}
		${DirectX_D3D11_LIBRARY}
		${DirectX_DXGI_LIBRARY}
	)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(GLFW3 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(DevIL REQUIRED)

	include_directories(
		${GLFW3_INLCUDE_PATH}
		${OPENGL_INCLUDE_DIR}
		${GLEW_INLCUDE_DIRS}
		${IL_INCLUDE_DIR}
	)

	set(GRAPHICS_LIBS
		${GLFW3_LIBRARIES}
		${OPENGL_gl_LIBRARY}
		${GLEW_LIBRARIES}
		${IL_LIBRARIES}
	)

	set(GRAPHICS_SOURCES
		GraphicsLinux.cpp
		GLSLProgram.cpp
	)

	add_custom_target(CopyShaders ALL
		${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/shadersLinux ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
	)
endif()

add_library(Graphics
	${GRAPHICS_SOURCES}
)

target_link_libraries(Graphics
	${GRAPHICS_LIBS}
	ResourceLoader
)

