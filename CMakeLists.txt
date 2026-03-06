cmake_minimum_required(VERSION 3.20)
project(pspedit)

# pspedit_common
file(GLOB_RECURSE pspedit_common_sources "source/common/*.cpp")
add_library(pspedit_common STATIC ${pspedit_common_sources})
set_property(TARGET pspedit_common PROPERTY CXX_STANDARD 17)
target_include_directories(pspedit_common PUBLIC "source/")

if(PSP)
	include(cmake/add_libpspexploit.cmake)
	add_libpspexploit("external/libpspexploit" "${CMAKE_CURRENT_BINARY_DIR}/external/libpspexploit")
	
	# pspedit_runtime
	file(GLOB_RECURSE pspedit_runtime_sources "source/runtime/*.cpp")
	add_executable(pspedit_runtime ${pspedit_runtime_sources})
	set_property(TARGET pspedit_runtime PROPERTY CXX_STANDARD 17)
	target_link_libraries(pspedit_runtime PRIVATE pspedit_common pspexploit pspdebug pspgu pspge pspdisplay pspctrl psprtc)
	create_pbp_file(
		TARGET pspedit_runtime
		ICON_PATH NULL
		BACKGROUND_PATH NULL
		PREVIEW_PATH NULL
		TITLE pspedit_runtime
		VERSION 01.00)
else()
	include(cmake/add_libdwarf.cmake)
	include(cmake/add_libusb.cmake)
	include(cmake/add_glfw.cmake)
	include(cmake/add_imgui.cmake)
	include(cmake/add_nfd.cmake)
	include(cmake/add_stb.cmake)
	add_libdwarf("external/libdwarf" "${CMAKE_CURRENT_BINARY_DIR}/external/libdwarf")
	add_libusb("external/libusb" "${CMAKE_CURRENT_BINARY_DIR}/external/libusb")
	add_glfw("external/glfw" "${CMAKE_CURRENT_BINARY_DIR}/external/glfw")
	add_imgui("external/imgui" "${CMAKE_CURRENT_BINARY_DIR}/external/imgui")
	add_nfd("external/nfd" "${CMAKE_CURRENT_BINARY_DIR}/external/nfd")
	add_stb("external/stb" "${CMAKE_CURRENT_BINARY_DIR}/external/stb")
	add_library(common_headers INTERFACE)
	add_library(runtime_headers INTERFACE)
	target_include_directories(common_headers INTERFACE "source/common")
	target_include_directories(runtime_headers INTERFACE "source/runtime")
	add_subdirectory("source/editor" "${CMAKE_BINARY_DIR}/editor")
	set_target_properties(pspedit PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
	target_sources(pspedit PRIVATE ${runtime_zip_cpp})
endif()
