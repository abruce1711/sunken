#### Pre-requisites

# Cmake version
cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(SUNKEN)



#### Setup Directories

# Main output directory
SET(output_directory "${CMAKE_BINARY_DIR}/bin/")

# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${output_directory})



#### Add External Dependencies

add_subdirectory("lib/sfml")
set(sfml_include "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")



#### Resources

add_custom_target(
	copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E
		copy_directory
    		"${CMAKE_SOURCE_DIR}/res"
    		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}/res"
)



#### Libraries

# Maths library
add_library(lib_maths INTERFACE)
target_sources(lib_maths
	INTERFACE "${CMAKE_SOURCE_DIR}/lib/maths/maths.hh"
)
target_include_directories(lib_maths
	INTERFACE "${CMAKE_SOURCE_DIR}/lib/maths"
	SYSTEM INTERFACE ${sfml_include}
)

# Renderer library
file(GLOB_RECURSE sources
	lib/renderer/*.cc
	lib/renderer/*.hh
)
add_library(lib_renderer STATIC ${sources})
target_include_directories(lib_renderer
	INTERFACE "${CMAKE_SOURCE_DIR}/lib/renderer/"
)
target_link_libraries(lib_renderer
	lib_maths
	sfml-graphics
)

# Tile loader library
file(GLOB_RECURSE sources
	lib/tile_level_loader/*.cc
	lib/tile_level_loader/*.hh
)
add_library(lib_tile_level_loader STATIC ${sources})
target_include_directories(lib_tile_level_loader
	INTERFACE "${CMAKE_SOURCE_DIR}/lib/tile_level_loader/"
)
target_link_libraries(lib_tile_level_loader
	lib_renderer
	lib_maths
	sfml-graphics
)

# Entity-Component
file(GLOB_RECURSE sources
	lib/ecm/*.cc
	lib/ecm/*.hh
)
add_library(lib_ecm STATIC ${sources})
target_include_directories(lib_ecm
	INTERFACE "${CMAKE_SOURCE_DIR}/lib/ecm"
)
target_link_libraries(lib_ecm
	lib_tile_level_loader
	lib_renderer
	lib_maths
)

# Scene
file(GLOB_RECURSE sources
	lib/scene/*.cc
	lib/scene/*.hh
)
add_library(lib_scene STATIC ${sources})
target_include_directories(lib_scene
	INTERFACE "${CMAKE_SOURCE_DIR}/lib/scene/"
)
target_link_libraries(lib_scene
	lib_ecm
	lib_tile_level_loader
	lib_renderer
	lib_maths
	sfml-graphics
)



#### Sunken

file(GLOB_RECURSE sources
	src/*.cc
	src/*.hh
)
add_executable(Sunken ${sources})
target_include_directories(Sunken
	PRIVATE ${sfml_include}
)
target_link_libraries(Sunken
	lib_scene
	lib_ecm
	lib_tile_level_loader
	lib_renderer
	lib_maths
	sfml-graphics
)
set_target_properties(Sunken
	PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}"
)
add_dependencies(Sunken copy_resources)