function(add_glfw SOURCE_DIR BINARY_DIR)
	set(glfw_main_sources
		"${SOURCE_DIR}/src/context.c"
		"${SOURCE_DIR}/src/init.c"
		"${SOURCE_DIR}/src/input.c"
		"${SOURCE_DIR}/src/monitor.c"
		"${SOURCE_DIR}/src/platform.c"
		"${SOURCE_DIR}/src/vulkan.c"
		"${SOURCE_DIR}/src/window.c"
		"${SOURCE_DIR}/src/egl_context.c"
		"${SOURCE_DIR}/src/osmesa_context.c"
		"${SOURCE_DIR}/src/null_init.c"
		"${SOURCE_DIR}/src/null_monitor.c"
		"${SOURCE_DIR}/src/null_window.c"
		"${SOURCE_DIR}/src/null_joystick.c")
	if(APPLE)
		set(glfw_platform_sources
			"${SOURCE_DIR}/src/cocoa_time.c"
			"${SOURCE_DIR}/src/posix_module.c"
			"${SOURCE_DIR}/src/posix_thread.c")
	elseif(WIN32)
		set(glfw_platform_sources
			"${SOURCE_DIR}/src/win32_time.c"
			"${SOURCE_DIR}/src/win32_module.c"
			"${SOURCE_DIR}/src/win32_thread.c"
			"${SOURCE_DIR}/src/win32_init.c"
			"${SOURCE_DIR}/src/win32_joystick.c"
			"${SOURCE_DIR}/src/win32_monitor.c"
			"${SOURCE_DIR}/src/win32_window.c"
			"${SOURCE_DIR}/src/wgl_context.c")    
	else()
		set(glfw_platform_sources 
			"${SOURCE_DIR}/src/posix_module.c"
			"${SOURCE_DIR}/src/posix_time.c"
			"${SOURCE_DIR}/src/posix_thread.c")
	endif()
	set(glfw_sources ${glfw_main_sources} ${glfw_platform_sources})
	add_library(glfw STATIC EXCLUDE_FROM_ALL ${glfw_sources})
	set_target_properties(glfw PROPERTIES CXX_STANDARD 17)
	set_target_properties(glfw PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR})
	target_include_directories(glfw PUBLIC "${SOURCE_DIR}/include")
	target_include_directories(glfw PUBLIC "${SOURCE_DIR}/deps")
	if(WIN32)
		target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
		find_package(OpenGL REQUIRED)
		target_link_libraries(glfw PUBLIC OpenGL::GL)
	endif()
	if(MSVC)
		target_compile_options(glfw PRIVATE /wd4034 /wd4996)
	endif()
endfunction()