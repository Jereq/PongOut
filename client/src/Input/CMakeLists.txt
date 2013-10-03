if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(SourceFiles
		InputWindows.cpp InputWindows.h
	)
else()
	set(SourceFiles
		InputLinux.cpp InputLinux.h
	)
endif()

add_library(Input
	IInput.cpp IInput.h
	${SourceFiles}
)