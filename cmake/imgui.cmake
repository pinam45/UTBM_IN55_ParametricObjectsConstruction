add_subdirectory(extlibs/imgui)

message(STATUS "Configuring Dear ImGui")

get_filename_component(IMGUI_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/extlibs/imgui/include  REALPATH)
set(IMGUI_LIBRARY imgui)

# Message
message("> include: ${IMGUI_INCLUDE_DIR}")
