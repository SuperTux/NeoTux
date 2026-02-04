cmake_minimum_required(VERSION 3.14)
project(NeoTux LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(NEOTUX_DATA_DIR "${CMAKE_BINARY_DIR}/data" CACHE PATH "Where NeoTux will look for game assets.")

if (NOT EXISTS "${NEOTUX_DATA_DIR}")
	if ("${NEOTUX_DATA_DIR}" STREQUAL "${CMAKE_BINARY_DIR}/data")
		message("No compiled data directory found. Did you run the script?")
	else()
		message("Your specified data directory doesn't exist. Is it placed?")
	endif()
	
	# Realy dumb... whatever
	file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

# TODO: my module function
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf)
find_package(SDL3_image)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3_image REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_ttf REQUIRED IMPORTED_TARGET sdl3-ttf)
#pkg_check_modules(SDL3_mixer REQUIRED IMPORTED_TARGET sdl3-mixer)

include(FetchContent)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

set(MINIAUDIO_VERSION "0.11.23" CACHE STRING "Version of miniaudio to use")
set(MINIAUDIO_NO_MP3 YES)
set(MINIAUDIO_NO_FLAC YES)
set(MINIAUDIO_NO_LIBVORBIS NO)
FetchContent_Declare(
	miniaudio
	URL https://github.com/mackron/miniaudio/archive/refs/tags/${MINIAUDIO_VERSION}.tar.gz
)
FetchContent_MakeAvailable(miniaudio)

set(SFSEXP_VERSION "1.4.1" CACHE STRING "Version of SfSexp to use")
FetchContent_Declare(
	sfsexp
	URL https://github.com/mjsottile/sfsexp/releases/download/v${SFSEXP_VERSION}/sfsexp-${SFSEXP_VERSION}.tar.gz
)
FetchContent_MakeAvailable(sfsexp)
include("${PROJECT_SOURCE_DIR}/cmake/sfsexp.cmake")

if (PSP)
	message(STATUS "Building PSP version...")
	set(NEOTUX_PSP ON)
	set(NEOTUX_DATA_DIR "data")
endif()

set(SSQ_BUILD_STATIC_ONLY Off)

#TODO broken
if (NOT PSP)
	if(EXISTS "${PROJECT_SOURCE_DIR}/external/simplesquirrel")
		message(STATUS "Using external/simplesquirrel")
		add_subdirectory("${PROJECT_SOURCE_DIR}/external/simplesquirrel")
	else()
		FetchContent_Declare(
			simplesquirrel
			GIT_REPOSITORY "https://github.com/swagtoy/simplesquirrel"
			GIT_BRANCH "master"
		)
		FetchContent_MakeAvailable(simplesquirrel)
	endif()
endif()

option(NEOTUX_BGFX "Use BGFX library for NeoTux." OFF)
if(NEOTUX_BGFX)
	set(BGFX_VERSION "v1.129.8914-493" CACHE STRING "Version of bgfx.cmake to use")
	FetchContent_Declare(
		bgfx
		URL https://github.com/bkaradzic/bgfx.cmake/releases/download/${BGFX_VERSION}/bgfx.cmake.${BGFX_VERSION}.tar.gz
	)

	set(BGFX_BUILD_EXAMPLES OFF)
	set(BGFX_BUILD_TOOLS ON)
	FetchContent_MakeAvailable(bgfx)
	include("${CMAKE_BINARY_DIR}/_deps/bgfx-src/cmake/bgfxToolUtils.cmake")

	bgfx_compile_shaders(
		TYPE VERTEX
		SHADERS "${NEOTUX_DATA_DIR}/shaders/vert.glsl"
		VARYING_DEF "${NEOTUX_DATA_DIR}/shaders/varying.def.sc"
		OUTPUT_DIR "${CMAKE_BINARY_DIR}/include/shaders"
	)

	bgfx_compile_shaders(
		TYPE FRAGMENT
		SHADERS "${NEOTUX_DATA_DIR}/shaders/frag.glsl"
		VARYING_DEF "${NEOTUX_DATA_DIR}/shaders/varying.def.sc"
		OUTPUT_DIR "${CMAKE_BINARY_DIR}/include/shaders"
	)
endif()

set(BUILD_TESTING OFF CACHE BOOL Enable tests)
if(BUILD_TESTING)
  #include(SuperTux/BuildTests)
  set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
  enable_testing()
  add_subdirectory(tests)
endif()

file(GLOB_RECURSE NEOTUX_SRC CONFIGURE_DEPENDS
	src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp
	tests/game_tests/*.cpp tests/game_tests/*.hpp)

if(NOT NEOTUX_BGFX)
	list(FILTER NEOTUX_SRC EXCLUDE REGEX bgfx/)
endif()

add_executable(NeoTux ${NEOTUX_SRC} )
target_include_directories(NeoTux PRIVATE src tests/game_tests)
target_link_libraries(NeoTux PUBLIC
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
	SDL3_image::SDL3_image

	SFSEXP

	miniaudio
	miniaudio_libvorbis
)

# NOTE: They seem to have forgotten to add this include directory to the miniaudio_libvorbis target.
target_include_directories(NeoTux SYSTEM PUBLIC ${miniaudio_SOURCE_DIR}/extras/decoders/libvorbis)

if(NEOTUX_BGFX)
	target_sources(NeoTux PRIVATE "${NEOTUX_DATA_DIR}/shaders/frag.glsl" "${NEOTUX_DATA_DIR}/shaders/vert.glsl")
	target_link_libraries(NeoTux PUBLIC bgfx)
endif()
option(NEOTUX_ASAN "Use Address Santizer." OFF)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_options(NeoTux PUBLIC -O0 -g -ggdb -fno-omit-frame-pointer)
	target_link_options(NeoTux PUBLIC -O0 -g -ggdb -fno-omit-frame-pointer)
	
	if (NEOTUX_ASAN)
		target_compile_options(NeoTux PUBLIC -fsanitize=address)
		target_link_options(NeoTux PUBLIC -fsanitize=address)
	endif()
endif()

set(extra_convert_cmds "")
include(cmake/psp.cmake)
message(STATUS "Data dir: ${NEOTUX_DATA_DIR}")

configure_file(config.h.in ${PROJECT_BINARY_DIR}/config.h)
target_include_directories(NeoTux PRIVATE ${PROJECT_BINARY_DIR})

add_custom_command(TARGET NeoTux POST_BUILD
	COMMAND ${CMAKE_SOURCE_DIR}/scripts/convert2spritesheet.pl
	ARGS --src="${CMAKE_SOURCE_DIR}/data" --dest="${NEOTUX_DATA_DIR}" --quiet --summary ${extra_convert_cmds})

# TODO: change this to neotux
set_target_properties(NeoTux PROPERTIES OUTPUT_NAME supertux3)

set_property(TARGET NeoTux PROPERTY CXX_STANDARD 20)
