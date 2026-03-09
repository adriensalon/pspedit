function(add_assimp SOURCE_DIR BINARY_DIR)
	
	option(ASSIMP_DOUBLE_PRECISION "Set to ON to enable double precision processing" OFF)
	set(GIT_COMMIT_HASH 0)
	set(ASSIMP_VERSION_MAJOR 5)
	set(ASSIMP_VERSION_MINOR 4)
	set(ASSIMP_VERSION_PATCH 1)
	set(ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
	set(ASSIMP_SOVERSION 5)

	configure_file(
		"${SOURCE_DIR}/include/assimp/revision.h.in"
		"${BINARY_DIR}/include/assimp/revision.h")
	configure_file(
		"${SOURCE_DIR}/include/assimp/config.h.in"
		"${BINARY_DIR}/include/assimp/config.h")
	configure_file(	
		"${SOURCE_DIR}/contrib/zlib/zconf.h.cmakein"
		"${BINARY_DIR}/include/zconf.h" @ONLY)
		
	set(assimp_zlib_sources
		"${SOURCE_DIR}/contrib/zlib/adler32.c"
		"${SOURCE_DIR}/contrib/zlib/compress.c"
		"${SOURCE_DIR}/contrib/zlib/crc32.c"
		"${SOURCE_DIR}/contrib/zlib/deflate.c"
		"${SOURCE_DIR}/contrib/zlib/gzclose.c"
		"${SOURCE_DIR}/contrib/zlib/gzlib.c"
		"${SOURCE_DIR}/contrib/zlib/gzread.c"
		"${SOURCE_DIR}/contrib/zlib/gzwrite.c"
		"${SOURCE_DIR}/contrib/zlib/inflate.c"
		"${SOURCE_DIR}/contrib/zlib/infback.c"
		"${SOURCE_DIR}/contrib/zlib/inftrees.c"
		"${SOURCE_DIR}/contrib/zlib/inffast.c"
		"${SOURCE_DIR}/contrib/zlib/trees.c"
		"${SOURCE_DIR}/contrib/zlib/uncompr.c"
		"${SOURCE_DIR}/contrib/zlib/zutil.c")

	file(GLOB_RECURSE assimp_asset_lib_fbx_sources "${SOURCE_DIR}/code/AssetLib/FBX/*.cpp")
	file(GLOB_RECURSE assimp_asset_lib_gltfcommon_sources "${SOURCE_DIR}/code/AssetLib/glTFCommon/*.cpp")
	file(GLOB_RECURSE assimp_asset_lib_gltf_sources "${SOURCE_DIR}/code/AssetLib/glTF/*.cpp")
	file(GLOB_RECURSE assimp_asset_lib_gltf2_sources "${SOURCE_DIR}/code/AssetLib/glTF2/*.cpp")
	set(assimp_asset_lib_sources
		${assimp_asset_lib_fbx_sources}
		${assimp_asset_lib_gltfcommon_sources}
		${assimp_asset_lib_gltf_sources}
		${assimp_asset_lib_gltf2_sources})
		
	file(GLOB_RECURSE assimp_common_sources "${SOURCE_DIR}/code/Common/*.cpp")
	file(GLOB_RECURSE assimp_geometry_sources "${SOURCE_DIR}/code/Geometry/*.cpp")
	file(GLOB_RECURSE assimp_material_sources "${SOURCE_DIR}/code/Material/*.cpp")
	file(GLOB_RECURSE assimp_pbrt_sources "${SOURCE_DIR}/code/Pbrt/*.cpp")
	file(GLOB_RECURSE assimp_post_processing_sources "${SOURCE_DIR}/code/PostProcessing/*.cpp")
	set(assimp_sources
		${assimp_zlib_sources}
		${assimp_asset_lib_sources}
		${assimp_common_sources}
		${assimp_geometry_sources}
		${assimp_material_sources}
		${assimp_pbrt_sources}
		${assimp_post_processing_sources})
	
	add_library(assimp STATIC EXCLUDE_FROM_ALL ${assimp_sources})
	set_target_properties(assimp PROPERTIES CXX_STANDARD 17)
	set_target_properties(assimp PROPERTIES C_STANDARD 99)
	target_compile_definitions(assimp PRIVATE -DRAPIDJSON_HAS_STDSTRING=1)
	target_compile_definitions(assimp PRIVATE 
		ASSIMP_BUILD_NO_X_IMPORTER
		ASSIMP_BUILD_NO_USD_IMPORTER
		ASSIMP_BUILD_NO_AMF_IMPORTER
		ASSIMP_BUILD_NO_3DS_IMPORTER
		ASSIMP_BUILD_NO_MD3_IMPORTER
		ASSIMP_BUILD_NO_MDL_IMPORTER
		ASSIMP_BUILD_NO_MD2_IMPORTER
		ASSIMP_BUILD_NO_PLY_IMPORTER
		ASSIMP_BUILD_NO_ASE_IMPORTER
		ASSIMP_BUILD_NO_OBJ_IMPORTER
		ASSIMP_BUILD_NO_HMP_IMPORTER
		ASSIMP_BUILD_NO_SMD_IMPORTER
		ASSIMP_BUILD_NO_MDC_IMPORTER
		ASSIMP_BUILD_NO_MD5_IMPORTER
		ASSIMP_BUILD_NO_STL_IMPORTER
		ASSIMP_BUILD_NO_LWO_IMPORTER
		ASSIMP_BUILD_NO_DXF_IMPORTER
		ASSIMP_BUILD_NO_NFF_IMPORTER
		ASSIMP_BUILD_NO_RAW_IMPORTER
		ASSIMP_BUILD_NO_SIB_IMPORTER
		ASSIMP_BUILD_NO_OFF_IMPORTER
		ASSIMP_BUILD_NO_AC_IMPORTER
		ASSIMP_BUILD_NO_BVH_IMPORTER
		ASSIMP_BUILD_NO_IRRMESH_IMPORTER
		ASSIMP_BUILD_NO_IRR_IMPORTER
		ASSIMP_BUILD_NO_Q3D_IMPORTER
		ASSIMP_BUILD_NO_B3D_IMPORTER
		ASSIMP_BUILD_NO_COLLADA_IMPORTER
		ASSIMP_BUILD_NO_TERRAGEN_IMPORTER
		ASSIMP_BUILD_NO_CSM_IMPORTER
		ASSIMP_BUILD_NO_3D_IMPORTER
		ASSIMP_BUILD_NO_LWS_IMPORTER
		ASSIMP_BUILD_NO_OGRE_IMPORTER
		ASSIMP_BUILD_NO_OPENGEX_IMPORTER
		ASSIMP_BUILD_NO_MS3D_IMPORTER
		ASSIMP_BUILD_NO_COB_IMPORTER
		ASSIMP_BUILD_NO_BLEND_IMPORTER
		ASSIMP_BUILD_NO_Q3BSP_IMPORTER
		ASSIMP_BUILD_NO_NDO_IMPORTER
		ASSIMP_BUILD_NO_IFC_IMPORTER
		ASSIMP_BUILD_NO_XGL_IMPORTER
		ASSIMP_BUILD_NO_ASSBIN_IMPORTER
		ASSIMP_BUILD_NO_C4D_IMPORTER
		ASSIMP_BUILD_NO_3MF_IMPORTER
		ASSIMP_BUILD_NO_X3D_IMPORTER
		ASSIMP_BUILD_NO_MMD_IMPORTER
		ASSIMP_BUILD_NO_M3D_IMPORTER
		ASSIMP_BUILD_NO_IQM_IMPORTER)

	set(assimp_private_include_dirs
		"${SOURCE_DIR}"
		"${SOURCE_DIR}/contrib"
		"${SOURCE_DIR}/contrib/openddlparser/code"
		"${SOURCE_DIR}/contrib/openddlparser/include"
		"${SOURCE_DIR}/contrib/pugixml/src"
		"${SOURCE_DIR}/contrib/rapidjson/include"
		"${SOURCE_DIR}/contrib/unzip"
		"${SOURCE_DIR}/contrib/utf8cpp/source"
		"${SOURCE_DIR}/contrib/zlib"
		"${SOURCE_DIR}/code")
	target_include_directories(assimp PRIVATE ${assimp_private_include_dirs})
	
	set(assimp_private_include_dirs
		"${SOURCE_DIR}/include"
		"${BINARY_DIR}/include")
	target_include_directories(assimp PUBLIC ${assimp_private_include_dirs})
		
	if(MSVC)
		target_compile_options(assimp PRIVATE /wd4244 /wd4996)
	endif()

endfunction()