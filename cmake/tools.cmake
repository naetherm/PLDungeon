##################################################
## CMake tools
##################################################

##################################################
## MACRO: cmaketools_debug_message
##
## Output a debug message (only visible if CMAKETOOLS_DEBUG is set) 
##################################################
macro(cmaketools_debug_message msg)
	if(CMAKETOOLS_DEBUG)
		message(STATUS ${msg})
	endif()
endmacro(cmaketools_debug_message msg)

##################################################
## MACRO: cmaketools_debug_variable
##
## Output the content of a variable as a debug message (only visible if CMAKETOOLS_DEBUG is set) 
##################################################
macro(cmaketools_debug_variable var)
	cmaketools_debug_message("${var} = ${${var}}")
endmacro(cmaketools_debug_variable var)

##################################################
## MACRO: add_to_list
##
## Add variable to list 
##################################################
macro(add_to_list var)
	foreach(value ${ARGN})
		set(${var} ${${var}} ${value})
	endforeach()
endmacro(add_to_list var)

##################################################
## MACRO: remove_from_list
##
## Remove variable from list 
##################################################
macro(remove_from_list var)
	set(new_value "")
	string(REPLACE " " ";" oldvar "${var}")
	foreach(value ${${oldvar}})
		set(found 0)
		foreach(remove ${ARGN})
			if(value STREQUAL ${remove})
				set(found 1)
			endif()
		endforeach()
		if(NOT found)
			set(new_value ${new_value} ${value})
		endif()
	endforeach()
	set(${var} ${new_value})
endmacro(remove_from_list var)

##################################################
## MACRO: add_to_string
##
## Add variable to string 
##################################################
macro(add_to_string var)
	foreach(value ${ARGN})
		set(${var} "${${var}} ${value}")
	endforeach()
endmacro(add_to_string var)

##################################################
## MACRO: remove_from_string
##
## Remove variable from string 
##################################################
macro(remove_from_string var)
	set(new_value " ")
	foreach(value ${${var}})
		set(found 0)
		foreach(remove ${ARGN})
			if(value STREQUAL ${remove})
				set(found 1)
			endif()
		endforeach()
		if(NOT found)
			set(new_value "${new_value} ${value}")
		endif()
	endforeach()
	set(${var} "${new_value}")
endmacro(remove_from_string var)

##################################################
## MACRO: add_sources
##
## Add source files to the project
##################################################
macro(add_sources)
	add_to_list(src ${ARGN})
endmacro(add_sources)

##################################################
## MACRO: remove_sources
##
## Remove source files from the project
##################################################
macro(remove_sources)
  	remove_from_list(src ${ARGN})
endmacro(remove_sources)

##################################################
## MACRO: add_include_directories
##
## Add include directories to the project
##################################################
macro(add_include_directories)
	include_directories(${ARGN})
endmacro(add_include_directories)

##################################################
## MACRO: add_link_directories
##
## Add link directories to the project
##################################################
macro(add_link_directories)
	link_directories(${ARGN})
endmacro(add_link_directories)

##################################################
## MACRO: add_libs
##
## Add external libraries to the project
##################################################
macro(add_libs)
	add_to_list(libs ${ARGN})
endmacro(add_libs)

##################################################
## MACRO: remove_libs
##
## Remove external libraries from the project
##################################################
macro(remove_libs)
	remove_from_list(libs ${ARGN})
endmacro(remove_libs)

##################################################
## MACRO: add_compile_defs
##
## Add compiler definitions
##################################################
macro(add_compile_defs)
	add_to_list(defs ${ARGN})
endmacro(add_compile_defs)

##################################################
## MACRO: remove_compile_defs
##
## Remove compiler definitions
##################################################
macro(remove_compile_defs)
	remove_from_list(defs ${ARGN})
endmacro(remove_compile_defs)

##################################################
## MACRO: add_compile_flags
##
## Add compiler flags or options
##################################################
macro(add_compile_flags)
	add_to_string(compile_flags ${ARGN})
endmacro(add_compile_flags)

##################################################
## MACRO: remove_compile_flags
##
## Remove compiler flags or options
##################################################
macro(remove_compile_flags)
	remove_from_list(compile_flags ${ARGN})
endmacro(remove_compile_flags)

##################################################
## MACRO: add_linker_flags
##
## Add linker flags
##################################################
macro(add_linker_flags)
	add_to_string(link_flags ${ARGN})
endmacro(add_linker_flags)

##################################################
## MACRO: remove_linker_flags
##
## Remove linker flags
##################################################
macro(remove_linker_flags)
	remove_from_string(link_flags ${ARGN})
endmacro(remove_linker_flags)

##################################################
## MACRO: init_project
##
## Initialize project variables
##################################################
macro(init_project)
	# Initialize project variables
	set(src           "")
	set(libs          "")
	set(defs          "")
	set(compile_flags " ")
	set(link_flags    " ")

	# Standalone: Set RPATH to "." so shared libraries are first searched within the
	# directory of the executable (see http://www.cmake.org/Wiki/CMake_RPATH_handling)
	SET(CMAKE_SKIP_BUILD_RPATH				FALSE)		# TRUE for standalone
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH		OFF) 		# ON for standalone
    SET(CMAKE_INSTALL_RPATH					"$ORIGIN")	# NOT "." for the same directory, the executable is in!
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH	FALSE)

	# Reset all default compiler and linker flags as we want the FULL control over these
	set(CMAKE_CXX_FLAGS						"")
	set(CMAKE_CXX_FLAGS_RELEASE				"")
	set(CMAKE_CXX_FLAGS_DEBUG				"")
	set(CMAKE_C_FLAGS						"")
	set(CMAKE_C_FLAGS_RELEASE				"")
	set(CMAKE_C_FLAGS_DEBUG					"")
	set(CMAKE_EXE_LINKER_FLAGS				"")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE		"")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG		"")
	set(CMAKE_SHARED_LINKER_FLAGS			"")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE	"")
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG		"")
endmacro(init_project)

##################################################
## MACRO: set_project_properties
##
## Set the propertiers of the current project
##################################################
macro(set_project_properties target)
	set_target_properties(${target} PROPERTIES COMPILE_DEFINITIONS "${defs}")
	set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${compile_flags}")
	set_target_properties(${target} PROPERTIES LINK_FLAGS "${link_flags}")
	set_target_properties(${target} PROPERTIES LINK_INTERFACE_LIBRARIES "")
endmacro(set_project_properties target)

get_filename_component(CMAKETOOLS_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
cmaketools_debug_variable(CMAKETOOLS_DIR)

# Add our modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
message(STATUS "${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules")
# Set LINUX flag
if(UNIX)
	SET(LINUX "1")
endif()

# Include platform files
if(WIN32)
	include(${CMAKE_SOURCE_DIR}/cmake/Platforms/WindowsMSVC.cmake)
elseif(LINUX)
	# Common settings
	include(${CMAKE_SOURCE_DIR}/cmake/Platforms/LinuxCommon.cmake)

	# Android NDK ("CMAKE_CXX_COMPILER_ID" matches also "GNU", so this test must come first)
	if(ANDROID)
		include(${CMAKE_SOURCE_DIR}/cmake/Platforms/LinuxNDK.cmake)

	# GCC
	elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
		include(${CMAKE_SOURCE_DIR}/cmake/Platforms/LinuxGCC.cmake)

	# Clang
	elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		include(${CMAKE_SOURCE_DIR}/cmake/Platforms/LinuxClang.cmake)

	# ?
	else()
		message(FATAL_ERROR "Unsupported compiler, use GNU, Clang or NDK")
	endif()
endif()
