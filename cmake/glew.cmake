message(STATUS "Configuring GLEW")

# Config
set(EMBEDDED_GLEW_NAME             "glew")
set(EMBEDDED_GLEW_VERSION          "2.1.0")
set(EMBEDDED_GLEW_MINGW_FILENAME   "libglew32.a")
set(EMBEDDED_GLEW_MSVC_FILENAME    "glew32.lib")

# Work
if(CONFIG_OS_WINDOWS)
	set(COMPILER_FOLDER "mingw")
	set(GLEW_LIB_FILE_NAME ${EMBEDDED_GLEW_MINGW_FILENAME})
	if(CONFIG_COMPILER_MSVC)
		set(COMPILER_FOLDER "msvc")
		set(GLEW_LIB_FILE_NAME ${EMBEDDED_GLEW_MSVC_FILENAME})
	endif()

	set(ARCH_FOLDER "Win32")
	if(CONFIG_ARCH_64BITS)
		set(ARCH_FOLDER "x64")
	endif()

	set(EMBEDED_GLEW_PATH ${CMAKE_SOURCE_DIR}/extlibs/${EMBEDDED_GLEW_NAME}-${EMBEDDED_GLEW_VERSION}/)
	set(EMBEDED_GLEW_LIB_PATH ${EMBEDED_GLEW_PATH}/lib/${COMPILER_FOLDER}/${ARCH_FOLDER})

	get_filename_component(GLEW_INCLUDE_DIR   ${EMBEDED_GLEW_PATH}/include                     REALPATH)
	get_filename_component(GLEW_LIBRARY       ${EMBEDED_GLEW_LIB_PATH}/${GLEW_LIB_FILE_NAME}   REALPATH)

	add_definitions(-DGLEW_STATIC)
else()
	find_package(GLEW REQUIRED)
	if (NOT GLEW_FOUND)
		message(FATAL_ERROR "GLEW not found")
		return()
	endif()
endif()

# Message
message("> include: ${GLEW_INCLUDE_DIR}")
message("> library: ${GLEW_LIBRARY}")
