cmake_minimum_required(VERSION 3.21)

project(
	NNDSL
	VERSION 0.0.1
	LANGUAGES CXX)

find_program(CLANG_FORMAT "clang-format")

set(CMAKE_SUPPRESS_REGENERATION ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "NNDSL Build mode: ${CMAKE_BUILD_TYPE}, should build tests: ${BUILD_TESTING}")

add_library(${PROJECT_NAME} STATIC "src/file_io.cpp" "src/lexer.cpp" "src/parser.cpp")
add_library(NNDSL::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(${PROJECT_NAME} PUBLIC include/nndsl_pch.hpp)

if(CLANG_FORMAT)
	file(GLOB_RECURSE formattable_files "include/**.hpp" "src/**.cpp")

	add_custom_target(
		"clang-format-${PROJECT_NAME}" COMMAND ${CLANG_FORMAT} -i -style=file
		${formattable_files})
endif()

if(CLANG_FORMAT)
	add_dependencies(${PROJECT_NAME} "clang-format-${PROJECT_NAME}")
endif()

add_subdirectory(example)

if(${BUILD_TESTING})
	add_subdirectory(tests)
endif()
