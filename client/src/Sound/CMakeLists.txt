set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

find_package(FMOD REQUIRED)

include_directories(
	${FMOD_INCLUDE_DIR}
)

add_library(Sound
	Sound.h
	Sound.cpp
	SoundSettings.h
	SoundSettings.cpp
	SoundManager.h
	SoundManager.cpp
)

target_link_libraries(Sound
	${FMOD_LIBRARY}
)