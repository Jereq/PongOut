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

	Chat.cpp Chat.h
	RequestCreateUser.cpp RequestCreateUser.h
	RequestFriendlist.cpp RequestFriendlist.h
	RequestLogin.cpp RequestLogin.h
	RequestLogout.cpp RequestLogout.h
	ResponseConnect.cpp ResponseConnect.h
	ResponseCreateUser.cpp ResponseCreateUser.h
	ResponseFriendlist.cpp ResponseFriendlist.h
	ResponseLogin.cpp ResponseLogin.h
)