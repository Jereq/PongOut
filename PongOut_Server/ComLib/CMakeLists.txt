add_definitions(-DBOOST_ALL_NO_LIB)

set(Boost_USE_STATIC_LIBS		ON)
set(Boost_USE_MULTITHREADED		ON)
set(Boost_USE_STATIC_RUNTIME	ON)

find_package(Boost 1.49 REQUIRED)

include_directories(
	${Boost_INCLUDE_DIR}
)

add_library(ComLib
	msgBase.cpp msgBase.h
	PacketHandler.cpp PacketHandler.h
	stdafx.h

	AcknowledgeLast.cpp AcknowledgeLast.h
	Chat.cpp Chat.h
	CreateUserRequest.cpp CreateUserRequest.h
	FriendlistRequest.cpp FriendlistRequest.h
	LoginRequest.cpp LoginRequest.h
	LogoutRequest.cpp LogoutRequest.h
	FriendlistResponse.cpp FriendlistResponse.h
	msgBase.cpp msgBase.h
	GameMessage.cpp GameMessage.h
	CreateGameRequest.cpp CreateGameRequest.h
	CreateGameResponse.cpp CreateGameResponse.h
	PacketHandler.cpp PacketHandler.h
	PaddleUpdateRequest.cpp PaddleUpdateRequest.h
	GameTickUpdate.cpp GameTickUpdate.h
	EndGameRequest.cpp EndGameRequest.h
	EndGameResponse.cpp EndGameResponse.h
	LaunchBallRequest.cpp LaunchBallRequest.h
)