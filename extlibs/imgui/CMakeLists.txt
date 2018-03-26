project(imgui)

get_filename_component(root_dir      ${CMAKE_CURRENT_SOURCE_DIR}    REALPATH)
get_filename_component(src_dir       ${root_dir}/src                REALPATH)
get_filename_component(include_dir   ${root_dir}/include            REALPATH)

include_directories("${include_dir} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLFW_INCLUDE_DIR}")

get_files(
        files "${src_dir}" "${include_dir}"
        OPTIONS recurse
)

make_target(
        ${PROJECT_NAME} "imgui" ${files}
        INCLUDES "${include_dir}" "${OPENGL_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}" "${IMGUI_INCLUDE_DIR}"
        OPTIONS cxx no_warnings
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${GLFW_LIBRARY}) # GLEW before OpenGL