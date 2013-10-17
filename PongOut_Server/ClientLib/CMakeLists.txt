add_definitions(-DBOOST_ALL_NO_LIB)

set(Boost_USE_STATIC_LIBS		ON)
set(Boost_USE_MULTITHREADED		ON)
set(Boost_USE_STATIC_RUNTIME	ON)

find_package(Boost 1.49 REQUIRED)

include_directories(
	../ComLib
	${Boost_INCLUDE_DIR}
)

add_library(ClientLib
	Server.cpp Server.h
	SafeQueue.h
	stdafx.h
	GameHandler.cpp GameHandler.h
)

target_link_libraries(
	ComLib
	${BOOST}
)