cmake_minimum_required(VERSION 3.10) # For CXX_STANDARD 17 property on Visual Studio

project(ParametricObjectsConstruction)
enable_language(CXX)

# Config
include(cmake/config.cmake)

# Libraries
include(cmake/opengl.cmake)
include(cmake/glew.cmake)
include(cmake/glfw.cmake)

include(cmake/common.cmake)

# By default build in Release mode
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

get_filename_component(root_dir      ${CMAKE_SOURCE_DIR}   REALPATH)
get_filename_component(src_dir       ${root_dir}/src       REALPATH)
get_filename_component(include_dir   ${root_dir}/include   REALPATH)

# ParametricObjectsConstruction
get_files(
  files "${src_dir}" "${include_dir}"
  OPTIONS recurse
)
make_target(
  ParametricObjectsConstruction "ParametricObjectsConstruction" ${files}
  INCLUDES "${include_dir}" "${OPENGL_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}"
  OPTIONS cxx executable
)
set_property(TARGET ParametricObjectsConstruction PROPERTY CXX_STANDARD 17)
target_link_libraries(ParametricObjectsConstruction ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${GLFW_LIBRARY}) # GLEW before OpenGL

# Debug
#set(CMAKE_VERBOSE_MAKEFILE 1)
