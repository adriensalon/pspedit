function(add_imgui SOURCE_DIR BINARY_DIR)
	set(imgui_sources
		"${SOURCE_DIR}/imgui.cpp"
		"${SOURCE_DIR}/imgui_widgets.cpp"
		"${SOURCE_DIR}/imgui_tables.cpp"
		"${SOURCE_DIR}/imgui_draw.cpp"
		"${SOURCE_DIR}/imgui_demo.cpp"
		"${SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"	
		"${SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
		"${SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
	add_library(imgui STATIC EXCLUDE_FROM_ALL ${imgui_sources})
	set_target_properties(imgui PROPERTIES CXX_STANDARD 17)
	set_target_properties(imgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR})
	target_compile_definitions(imgui PUBLIC -DIMGUI_USE_WCHAR32)
	target_include_directories(imgui PUBLIC "${SOURCE_DIR}")
	target_link_libraries(imgui PRIVATE glfw)
endfunction()