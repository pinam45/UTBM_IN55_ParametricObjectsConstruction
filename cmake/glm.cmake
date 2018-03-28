message(STATUS "Configuring GLM")

# Config
set(EMBEDDED_GLM_NAME      "glm")
set(EMBEDDED_GLM_VERSION   "0.9.9-a2")

set(EMBEDED_GLM_PATH ${CMAKE_SOURCE_DIR}/extlibs/${EMBEDDED_GLM_NAME}-${EMBEDDED_GLM_VERSION}/)
get_filename_component(GLM_INCLUDE_DIR   ${EMBEDED_GLM_PATH}/include REALPATH)

# Message
message("> include: ${GLM_INCLUDE_DIR}")
