project(NNDSL_Tests)

Include(FetchContent)

set(CATCH_BUILD_TESTING OFF CACHE BOOL "")
set(CATCH_BUILD_EXAMPLES OFF CACHE BOOL "")
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v2.11.1
)

FetchContent_Declare(ApprovalTests
	GIT_REPOSITORY https://github.com/approvals/ApprovalTests.cpp.git
	GIT_TAG master)

FetchContent_MakeAvailable(Catch2 ApprovalTests)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)

add_executable(NNDSL_Tests main.cpp test.cpp test2.cpp)
target_link_libraries(NNDSL_Tests PRIVATE Catch2::Catch2 ApprovalTests::ApprovalTests)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DAPPROVAL_TESTS_HIDE_DEPRECATED_CODE=1)

if(CLANG_FORMAT)
	file(GLOB_RECURSE formattable_files "./**.cpp")

	add_custom_target(
		"clang-format-${PROJECT_NAME}" COMMAND ${CLANG_FORMAT} -i -style=file
		${formattable_files})
endif()

if(CLANG_FORMAT)
	add_dependencies(${PROJECT_NAME} "clang-format-${PROJECT_NAME}")
endif()

if(TARGET Catch2::Catch2Main)
	# link Catch2::Catch2Main only if it exists (so probably catch2 v3)
	target_link_libraries(NNDSL_Tests PRIVATE Catch2::Catch2Main)
endif()
