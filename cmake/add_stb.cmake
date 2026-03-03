function(add_stb SOURCE_DIR BINARY_DIR)
	add_library(stb INTERFACE)
	target_include_directories(stb INTERFACE ${SOURCE_DIR})
endfunction()