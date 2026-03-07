function(add_cereal SOURCE_DIR BINARY_DIR)
	add_library(cereal INTERFACE)
	target_include_directories(cereal INTERFACE ${SOURCE_DIR}/include)
endfunction()