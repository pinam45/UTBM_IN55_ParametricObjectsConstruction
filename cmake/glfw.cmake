message(STATUS "Configuring GLFW")

# Config
set(EMBEDDED_GLFW_NAME             "glfw")
set(EMBEDDED_GLFW_VERSION          "3.2.1")
set(EMBEDDED_GLFW_MINGW_FILENAME   "libglfw3.a")
set(EMBEDDED_GLFW_MSVC_FILENAME    "glfw3.lib")

# Work
if(CONFIG_OS_WINDOWS)
	set(COMPILER_FOLDER "mingw")
	set(GLFW_LIB_FILE_NAME ${EMBEDDED_GLFW_MINGW_FILENAME})
	if(CONFIG_COMPILER_MSVC)
		set(COMPILER_FOLDER "msvc")
		set(GLFW_LIB_FILE_NAME ${EMBEDDED_GLFW_MSVC_FILENAME})
	endif()

	set(ARCH_FOLDER "Win32")
	if(CONFIG_ARCH_64BITS)
		set(ARCH_FOLDER "x64")
	endif()

	set(EMBEDED_GLFW_PATH ${CMAKE_SOURCE_DIR}/extlibs/${EMBEDDED_GLFW_NAME}-${EMBEDDED_GLFW_VERSION}/)
	set(EMBEDED_GLFW_LIB_PATH ${EMBEDED_GLFW_PATH}/lib/${COMPILER_FOLDER}/${ARCH_FOLDER})

	get_filename_component(GLFW_INCLUDE_DIR   ${EMBEDED_GLFW_PATH}/include                     REALPATH)
	get_filename_component(GLFW_LIBRARY       ${EMBEDED_GLFW_LIB_PATH}/${GLFW_LIB_FILE_NAME}   REALPATH)
else()
	find_package(PkgConfig REQUIRED)
	if (NOT PKG_CONFIG_FOUND)
		message(FATAL_ERROR "PkgConfig not found (used to find GLFW)")
		return()
	endif()

	pkg_search_module(GLFW REQUIRED glfw3)
	if (NOT GLFW_FOUND)
		message(FATAL_ERROR "oh noze! GLFW!")
		return()
	endif()
	set(GLFW_INCLUDE_DIR   ${GLFW_INCLUDE_DIRS})
	set(GLFW_LIBRARY       ${GLFW_LIBRARIES})
endif()

# Message
message("> include: ${GLFW_INCLUDE_DIR}")
message("> library: ${GLFW_LIBRARY}")
