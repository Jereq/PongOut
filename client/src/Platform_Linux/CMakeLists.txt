find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
	${OPENGL_INCLUDE_DIR}
	${GLEW_INLCUDE_DIRS}
)

set(GRAPHICS_LIBS
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARIES}
)

add_library(Graphics Graphics.cpp)
target_link_libraries(Graphics ${GRAPHICS_LIBS})
