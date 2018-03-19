message(STATUS "Configuring OpenGL")

find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found")
	return()
endif()
set(OPENGL_INCLUDE_DIR   ${OPENGL_INCLUDE_DIRS})
set(OPENGL_LIBRARY       ${OPENGL_gl_LIBRARY})
