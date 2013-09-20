cmake_minimum_required(VERSION 2.8.10)
# -----------------------------------------------------------------------------
# Find DirectX SDK
# Define:
# DirectX_FOUND
# DirectX_INCLUDE_DIR
# DirectX_LIBRARY
# DirectX_ROOT_DIR

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	#set 32/64
	if(CMAKE_CL_64)
		set (DirectX_ARCHITECTURE x64)
 		message("WIN64")
	else()
		set (DirectX_ARCHITECTURE x86)
  		message("WIN32")
	endif()

	### DIRECTX SDK FOR !FUBAR OS ###
	find_path (DirectX_ROOT_DIR
		Include/d3d9.h
		PATHS
			"$ENV{DXSDK_DIR}"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK (June 2010)"
			"$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK (June 2010)"
		DOC "DirectX SDK root directory")

	if(DirectX_ROOT_DIR)
		message("${DirectX_ROOT_DIR}/Include")
		message("${DirectX_ROOT_DIR}/Lib/${DirectX_ARCHITECTURE}")
		message("${DirectX_ROOT_DIR}/Utilities/bin/${DirectX_ARCHITECTURE}")
		set(DirectX_INC_SEARCH_PATH "${DirectX_ROOT_DIR}/Include")
		set(DirectX_LIB_SEARCH_PATH "${DirectX_ROOT_DIR}/Lib/${DirectX_ARCHITECTURE}")
		set(DirectX_BIN_SEARCH_PATH "${DirectX_ROOT_DIR}/Utilities/bin/${DirectX_ARCHITECTURE}")
	else()
		message("DirectX SDK (June 2010) directory was not found!")
	endif()

	### DIRECTX SDK FOR WINDOWS8 ###
	#if (DEFINED MSVC_VERSION AND NOT ${MSVC_VERSION} LESS 1700)
	#	message("I try my hardest to be an apple")
	#	find_path (WIN8_SDK_ROOT_DIR
	#		Include/um/windows.h
	#		PATHS
	#			"$ENV{ProgramFiles}/Windows Kits/8.0"
	#			"$ENV{ProgramFiles(x86)}/Windows Kits/8.0"
	#			DOC "Windows 8 SDK root directory"
	#	)
	#
	#	if (WIN8_SDK_ROOT_DIR)
	#		message("Windows 8 directory found")
	#		set (DirectX_INC_SEARCH_PATH "${WIN8_SDK_ROOT_DIR}/Include/um" "${WIN8_SDK_ROOT_DIR}/Include/shared")
	#		set (DirectX_LIB_SEARCH_PATH "${WIN8_SDK_ROOT_DIR}/Lib/Win8/um/${DirectX_ARCHITECTURE}")
	#		set (DirectX_BIN_SEARCH_PATH "${WIN8_SDK_ROOT_DIR}/bin/x86")
	#	endif ()
	#endif ()
	
	### ADDITIONAL LINKS ###
	#--- D3D10 ---#
	#D3D10 - DIR
	find_path (DirectX_D3D10_INCLUDE_DIR d3d10.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "d3d10.h dir")
	#D3D10 - LIB
	find_library (DirectX_D3D10_LIBRARY d3d10
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "d3d10 lib")
	#D3DX10 - DIR
	find_path (DirectX_D3DX10_INCLUDE_DIR d3dx10.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "d3dx10.h dir")
	#D3DX10 - LIB
	find_library (DirectX_D3DX10_LIBRARY d3dx10
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "d3dx10 lib")
	#SET BOTH AS FOUND
	if (DirectX_D3D10_INCLUDE_DIR AND DirectX_D3D10_LIBRARY)
		set (DirectX_D3D10_FOUND 1)
		if (DirectX_D3DX10_INCLUDE_DIR AND DirectX_D3DX10_LIBRARY)
			set (DirectX_D3DX10_FOUND 1)
		message("dir: ${DirectX_D3DX10_INCLUDE_DIR}")
		message("lib: ${DirectX_D3DX10_LIBRARY}")
		endif()
	endif ()

	#--- D3D11 ---#
	#D3D11 - DIR
	find_path (DirectX_D3D11_INCLUDE_DIR d3d11.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "d3d11.h dir")
	#D3D11 - LIB	
	find_library (DirectX_D3D11_LIBRARY d3d11
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "d3d11 lib")
	#D3DX11 - DIR
	find_path (DirectX_D3DX11_INCLUDE_DIR D3DX11.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "d3dx11.h dir")
	#D3DX11 - LIB
	find_library (DirectX_D3DX11_LIBRARY D3DX11
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "d3dx11 lib")
	#SET BOTH AS FOUND
	if (DirectX_D3D11_INCLUDE_DIR AND DirectX_D3D11_LIBRARY)
		set (DirectX_D3D11_FOUND 1)
		if (DirectX_D3DX11_INCLUDE_DIR AND DirectX_D3DX11_LIBRARY)
			set (DirectX_D3DX11_FOUND 1)
		message("dir: ${DirectX_D3DX11_INCLUDE_DIR}")
		message("lib: ${DirectX_D3DX11_LIBRARY}")
		endif()
	endif()
	
	#--- DXGI --->
	#DXGI - DIR
	find_path (DirectX_DXGI_INCLUDE_DIR DXGI.h
		PATHS ${DirectX_INC_SEARCH_PATH}
		DOC "DXGI.h dir")
	#DXGI - LIB
	find_library (DirectX_DXGI_LIBRARY DXGI
		PATHS ${DirectX_LIB_SEARCH_PATH}
		DOC "DXGI lib")
	#SET BOTH AS FOUND
	if (DirectX_DXGI_INCLUDE_DIR  AND DirectX_DXGI_LIBRARY)
		set (DirectX_DXGI_FOUND 1)
		message("dir: ${DirectX_DXGI_INCLUDE_DIR}")
		message("lib: ${DirectX_DXGI_LIBRARY}")
	endif()
	#MARK
	mark_as_advanced (
		DirectX_D3DX10_INCLUDE_DIR
		DirectX_D3DX10_LIBRARY
		DirectX_D3DX11_INCLUDE_DIR
		DirectX_D3DX11_LIBRARY
	)
	
	mark_as_advanced (
		DirectX_D3DX10_FOUND
		DirectX_D3D10_FOUND
		DirectX_D3DX11_FOUND
		DirectX_D3D11_FOUND
	)
	
	if(DirectX_D3DX10_FOUND AND DirectX_D3DX11_FOUND)
		message("All is good")

	endif()
endif()
