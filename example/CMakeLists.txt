cmake_minimum_required(VERSION 3.21)

project(
	NNDSLExample
	VERSION 0.0.1
	LANGUAGES CXX)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE NNDSL::NNDSL)

if(CLANG_FORMAT)
	file(GLOB_RECURSE formattable_files "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp")
	message(STATUS ${formattable_files})

	add_custom_target(
		"clang-format-${PROJECT_NAME}" COMMAND ${CLANG_FORMAT} -i -style=file
		${formattable_files})
endif()

if(CLANG_FORMAT)
	add_dependencies(${PROJECT_NAME} "clang-format-${PROJECT_NAME}")
endif()

add_custom_command(
	TARGET NNDSLExample POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/test_files
	$<TARGET_FILE_DIR:NNDSL>)

add_custom_command(
	TARGET NNDSLExample POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/test_files
	$<TARGET_FILE_DIR:NNDSLExample>)
