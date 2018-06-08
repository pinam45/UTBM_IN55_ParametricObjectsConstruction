cmake_minimum_required(VERSION 3.10) # For CXX_STANDARD 17 property on Visual Studio

enable_language(CXX)

# Common
include(cmake/common.cmake)

# Config
include(cmake/config.cmake)

# Libraries
include(cmake/opengl.cmake)
include(cmake/glew.cmake)
include(cmake/glfw.cmake)
include(cmake/glm.cmake)
include(cmake/imgui.cmake)

# By default build in Release mode
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

project(ParametricObjectsConstruction)

get_filename_component(root_dir      ${CMAKE_SOURCE_DIR}   ABSOLUTE)
get_filename_component(src_dir       ${root_dir}/src       ABSOLUTE)
get_filename_component(include_dir   ${root_dir}/include   ABSOLUTE)
get_filename_component(shaders_dir   ${root_dir}/shaders   ABSOLUTE)


# ParametricObjectsConstruction
get_files(
  files "${src_dir}" "${include_dir}"
  OPTIONS recurse
)

make_target(
  ParametricObjectsConstruction "ParametricObjectsConstruction" ${files}
  INCLUDES "${include_dir}"
  EXT_INCLUDES "${OPENGL_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}" "${IMGUI_INCLUDE_DIR}" "${GLM_INCLUDE_DIR}"
  OPTIONS cxx executable
)

set_target_properties(ParametricObjectsConstruction PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set_property(TARGET ParametricObjectsConstruction PROPERTY CXX_STANDARD 17)
target_link_libraries(ParametricObjectsConstruction ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${GLFW_LIBRARY} ${IMGUI_LIBRARY}) # GLEW before OpenGL

configure_folder(${shaders_dir}   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")

# Debug
#set(CMAKE_VERBOSE_MAKEFILE 1)
