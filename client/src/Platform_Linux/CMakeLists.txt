set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
	${GLFW3_INLCUDE_PATH}
	${OPENGL_INCLUDE_DIR}
	${GLEW_INLCUDE_DIRS}
)

set(GRAPHICS_LIBS
	${GLFW3_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARIES}
)

add_library(Graphics Graphics.cpp)
target_link_libraries(Graphics ${GRAPHICS_LIBS})
