find_package(Boost 1.49 COMPONENTS unit_test_framework REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
)

add_executable(PongOut-test
	mainTest.cpp
	SoundTest.cpp
	CoreSystemTest.cpp
	ResourceLoaderTest.cpp
	CollisionTest.cpp
	GameSettingsTest.cpp
	../PhysicsComponent.cpp
	../InputComponent.cpp
	../Map.cpp
	../BlockC.cpp
	../Paddle.cpp
	../Ball.cpp
	../GameObject.cpp
	../GraphicsComponent.cpp
	../InputState.cpp
	../TMLReader.cpp
	../GameSettings.cpp
)

target_link_libraries(PongOut-test
	${Boost_LIBRARIES}
	${EXTRA_LIBS}
)
