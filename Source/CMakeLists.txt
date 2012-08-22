##################################################
## Project
##################################################
cmake_minimum_required(VERSION 2.6)
set(target Dungeon)
project(${target})
init_project()

# Set target system (console/win32)
if(WIN32)
  set(system WIN32)
else()
  set(system "")
endif()

##################################################
## Find packages
##################################################
find_package(PixelLight)

##################################################
## Source files
##################################################
add_sources(
    src/Main.cpp
    src/Application.cpp
    src/Config.cpp
    src/SNMLightRandomAnimation.cpp
    src/Gui/IngameGui.cpp
    src/Gui/WindowBase.cpp
    src/Gui/WindowMenu.cpp
    src/Gui/WindowResolution.cpp
    src/Gui/WindowText.cpp
)
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_sources(
		Dungeon.rc
	)
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	src
	${PL_PLCORE_INCLUDE_DIR}
	${PL_PLMATH_INCLUDE_DIR}
	${PL_PLGRAPHICS_INCLUDE_DIR}
	${PL_PLGUI_INCLUDE_DIR}
	${PL_PLGUIXMLTEXT_INCLUDE_DIR}
	${PL_PLINPUT_INCLUDE_DIR}
	${PL_PLRENDERER_INCLUDE_DIR}
	${PL_PLMESH_INCLUDE_DIR}
	${PL_PLSCENE_INCLUDE_DIR}
	${PL_PLENGINE_INCLUDE_DIR}
	${PL_PLPHYSICS_INCLUDE_DIR}
	${PL_PLFRONTENDPLGUI_INCLUDE_DIR}
)

##################################################
## Additional libraries
##################################################
add_libs(
	${PL_PLCORE_LIBRARY}
	${PL_PLMATH_LIBRARY}
	${PL_PLGRAPHICS_LIBRARY}
	${PL_PLGUI_LIBRARY}
	${PL_PLGUIXMLTEXT_LIBRARY}
	${PL_PLINPUT_LIBRARY}
	${PL_PLRENDERER_LIBRARY}
	${PL_PLMESH_LIBRARY}
	${PL_PLSCENE_LIBRARY}
	${PL_PLENGINE_LIBRARY}
	${PL_PLPHYSICS_LIBRARY}
	${PL_PLFRONTENDPLGUI_LIBRARY}
)

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
)
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_compile_defs(
		${WIN32_COMPILE_DEFS}
		_WINDOWS
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_compile_defs(
		${LINUX_COMPILE_DEFS}
	)
endif()

##################################################
## Compiler flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_compile_flags(
		${WIN32_COMPILE_FLAGS}
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	add_compile_flags(
		${LINUX_COMPILE_FLAGS}
	)
endif()

##################################################
## Linker flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_linker_flags(
		${WIN32_LINKER_FLAGS}
	)
endif()

##################################################
## Build
##################################################
add_executable(${target} ${system} ${src})
target_link_libraries (${target} ${libs})
set_project_properties(${target})

##################################################
## Dependencies
##################################################

##################################################
## Post-Build
##################################################

# Executable
add_custom_command(TARGET ${target}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX} "${CMAKE_SOURCE_DIR}/Bin/${PL_ARCHBITSIZE}"
)

##################################################
## Install
##################################################
