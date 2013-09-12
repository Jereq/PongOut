set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(DevIL REQUIRED)
find_package(GLM REQUIRED)

include_directories(
	${GLFW3_INLCUDE_PATH}
	${OPENGL_INCLUDE_DIR}
	${GLEW_INLCUDE_DIRS}
	${IL_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
)

set(GRAPHICS_LIBS
	${GLFW3_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARIES}
	${IL_LIBRARIES}
)

add_custom_target(CopyShaders ALL
	${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)

add_library(Graphics Graphics.cpp GLSLProgram.cpp)
target_link_libraries(Graphics ${GRAPHICS_LIBS})
