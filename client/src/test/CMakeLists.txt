find_package(Boost 1.49 COMPONENTS unit_test_framework REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
)

add_executable(PongOut-test
	mainTest.cpp
	SoundTest.cpp
	../ICoreSystem.cpp
	CoreSystemTest.cpp
	ResourceLoaderTest.cpp
)

target_link_libraries(PongOut-test
	${Boost_LIBRARIES}
	${EXTRA_LIBS}
)
