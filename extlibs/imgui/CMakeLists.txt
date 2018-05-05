project(imgui)

get_filename_component(root_dir      ${CMAKE_CURRENT_SOURCE_DIR}    ABSOLUTE)
get_filename_component(src_dir       ${root_dir}/src                ABSOLUTE)
get_filename_component(include_dir   ${root_dir}/include            ABSOLUTE)

get_files(
	files "${src_dir}" "${include_dir}"
	OPTIONS recurse
)

make_target(
	imgui "extlibs" ${files}
	INCLUDES "${include_dir}"
	OPTIONS cxx no_warnings
)
set_property(TARGET imgui PROPERTY CXX_STANDARD 17)
