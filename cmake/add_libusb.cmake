function(add_libusb SOURCE_DIR BINARY_DIR)
	# Visibility (kept from your script, but simplified)
	set(DEFAULT_VISIBILITY "")
	if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
	# You can also just leave DEFAULT_VISIBILITY empty if you don't care
	set(DEFAULT_VISIBILITY "__attribute__((visibility(\"default\")))")
	endif()

	set(LIBUSB_GEN_INCLUDES "${CMAKE_CURRENT_BINARY_DIR}/gen_include")
	file(MAKE_DIRECTORY "${LIBUSB_GEN_INCLUDES}")

	# Platform macros
	set(_platform_defines "")
	if(WIN32)
	string(APPEND _platform_defines "#define PLATFORM_WINDOWS 1\n")
	else()
	string(APPEND _platform_defines "#define PLATFORM_POSIX 1\n")
	endif()

	# Feature knobs you care about
	set(_feature_defines "")
	if(LIBUSB_ENABLE_LOGGING)
	string(APPEND _feature_defines "#define ENABLE_LOGGING 1\n")
	endif()
	if(LIBUSB_ENABLE_DEBUG_LOGGING)
	string(APPEND _feature_defines "#define ENABLE_DEBUG_LOGGING 1\n")
	endif()
	if(WIN32 AND LIBUSB_ENABLE_WINDOWS_HOTPLUG)
	string(APPEND _feature_defines "#define LIBUSB_WINDOWS_HOTPLUG 1\n")
	endif()

	# Minimal config.h. Leave most HAVE_* undefined unless you want to hardcode them.
	set(_config_h
	"
	#pragma once
	#define DEFAULT_VISIBILITY ${DEFAULT_VISIBILITY}
	${_platform_defines}
	${_feature_defines}
	#if defined(__GNUC__)
		#define PRINTF_FORMAT(a, b) __attribute__ ((format (__printf__, a, b)))
	#else
		#define PRINTF_FORMAT(a, b)
	#endif
	#define STDC_HEADERS 1")
	file(WRITE "${LIBUSB_GEN_INCLUDES}/config.h" "${_config_h}")

	add_library(libusb STATIC)
	target_sources(libusb PRIVATE
		"${LIBUSB_GEN_INCLUDES}/config.h"
		"external/libusb/libusb/core.c"
		"external/libusb/libusb/descriptor.c"
		"external/libusb/libusb/hotplug.c"
		"external/libusb/libusb/io.c"
		"external/libusb/libusb/libusb.h"
		"external/libusb/libusb/libusbi.h"
		"external/libusb/libusb/strerror.c"
		"external/libusb/libusb/sync.c"
		"external/libusb/libusb/version.h"
		"external/libusb/libusb/version_nano.h")
	target_include_directories(libusb
		PRIVATE
			"${LIBUSB_GEN_INCLUDES}"
			"external/libusb/libusb/os")

	if(WIN32)
		target_sources(libusb PRIVATE
			"external/libusb/libusb/libusb-1.0.def"
			"external/libusb/libusb/os/events_windows.c"
			"external/libusb/libusb/os/events_windows.h"
			"external/libusb/libusb/os/threads_windows.c"
			"external/libusb/libusb/os/threads_windows.h"
			"external/libusb/libusb/os/windows_common.c"
			"external/libusb/libusb/os/windows_common.h"
			"external/libusb/libusb/os/windows_usbdk.c"
			"external/libusb/libusb/os/windows_usbdk.h"
			"external/libusb/libusb/os/windows_winusb.c"
			"external/libusb/libusb/os/windows_winusb.h")
			
		target_compile_definitions(libusb PRIVATE HAVE_STRUCT_TIMESPEC=1)

		target_compile_definitions(libusb PRIVATE $<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS=1>)
		target_link_libraries(libusb PRIVATE windowsapp)
		if(LIBUSB_ENABLE_WINDOWS_HOTPLUG)
			target_sources(libusb PRIVATE
				"external/libusb/libusb/os/windows_hotplug.c"
				"external/libusb/libusb/os/windows_hotplug.h")
			target_compile_definitions(libusb PRIVATE LIBUSB_WINDOWS_HOTPLUG=1)
		endif()
	else()
		target_sources(libusb PRIVATE
			"${LIBUSB_ROOT}/os/events_posix.c"
			"${LIBUSB_ROOT}/os/events_posix.h"
			"${LIBUSB_ROOT}/os/threads_posix.c"
			"${LIBUSB_ROOT}/os/threads_posix.h"
		)
		if(CMAKE_SYSTEM_NAME MATCHES "Linux")
			target_sources(libusb PRIVATE
				"${LIBUSB_ROOT}/os/linux_usbfs.c"
				"${LIBUSB_ROOT}/os/linux_usbfs.h"
			)
			if(LIBUSB_ENABLE_UDEV)
				target_sources(libusb PRIVATE
					"${LIBUSB_ROOT}/os/linux_udev.c"
				)
				target_link_libraries(libusb PRIVATE udev)
				target_compile_definitions(libusb PRIVATE HAVE_LIBUDEV=1)
			else()
				target_sources(libusb PRIVATE
					"${LIBUSB_ROOT}/os/linux_netlink.c"
				)
			endif()
			find_package(Threads REQUIRED)
			target_link_libraries(libusb PRIVATE Threads::Threads)
			
		elseif(APPLE)
			target_sources(libusb PRIVATE
				"${LIBUSB_ROOT}/os/darwin_usb.c"
				"${LIBUSB_ROOT}/os/darwin_usb.h"
			)
			target_link_libraries(libusb PRIVATE
				"-framework Foundation"
				"-framework IOKit"
				"-framework Security"
			)
		elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
			target_sources(libusb PRIVATE
				"${LIBUSB_ROOT}/os/netbsd_usb.c"
			)
		elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
			target_sources(libusb PRIVATE
				"${LIBUSB_ROOT}/os/openbsd_usb.c"
			)
		elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
			target_sources(libusb PRIVATE
				"${LIBUSB_ROOT}/os/sunos_usb.c"
			)
			target_link_libraries(libusb PRIVATE -ldevinfo -lnvpair)
		else()
			message(FATAL_ERROR "Unsupported target platform: ${CMAKE_SYSTEM_NAME}")
		endif()
	endif()
	target_include_directories(libusb PUBLIC "external/libusb/libusb")
	if(MSVC)
		target_compile_options(libusb PRIVATE /wd5287)
	endif()
endfunction()