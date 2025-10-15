# Copyright (c) 2025 RealTimeChris (Chris M.)
# 
# This file is part of software offered under a restricted-use license to a designated Licensee,
# whose identity is confirmed in writing by the Author.
# 
# License Terms (Summary):
# - Exclusive, non-transferable license for internal use only.
# - Redistribution, sublicensing, or public disclosure is prohibited without written consent.
# - Full ownership remains with the Author.
# - License may terminate if unused for [X months], if materially breached, or by mutual agreement.
# - No warranty is provided, express or implied.
# 
# Full license terms are provided in the LICENSE file distributed with this software.
# 
# Signed,
# RealTimeChris (Chris M.)
# 2025
# */

cmake_minimum_required(VERSION 3.28)

project("nihilus-network" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(gRPC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_CSHARP_PLUGIN OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_NODE_PLUGIN OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_PHP_PLUGIN OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_PYTHON_PLUGIN OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_RUBY_PLUGIN OFF CACHE BOOL "" FORCE)

set(gRPC_BUILD_CODEGEN OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPC_CPP_PLUGIN OFF CACHE BOOL "" FORCE)
set(gRPC_BACKWARDS_COMPATIBILITY_MODE OFF CACHE BOOL "" FORCE)

set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_PROTOC_BINARIES OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_LIBPROTOC OFF CACHE BOOL "" FORCE)
set(protobuf_WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE BOOL "" FORCE)

set(ABSL_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ABSL_BUILD_TEST_HELPERS OFF CACHE BOOL "" FORCE)
set(ABSL_USE_EXTERNAL_GOOGLETEST ON CACHE BOOL "" FORCE)
set(ABSL_FIND_GOOGLETEST OFF CACHE BOOL "" FORCE)
set(ABSL_RUN_TESTS OFF CACHE BOOL "" FORCE)
set(ABSL_USE_GOOGLETEST_HEAD OFF CACHE BOOL "" FORCE)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF CACHE BOOL "" FORCE)

set(RE2_BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(CARES_STATIC ON CACHE BOOL "" FORCE)
set(CARES_SHARED OFF CACHE BOOL "" FORCE)
set(CARES_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(CARES_BUILD_TESTS OFF CACHE BOOL "" FORCE)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(WITH_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WITH_BENCHMARK OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(gRPC_INSTALL OFF CACHE BOOL "Don't install gRPC" FORCE)
set(ABSL_ENABLE_INSTALL OFF CACHE BOOL "Don't install Abseil" FORCE)
set(protobuf_INSTALL OFF CACHE BOOL "Don't install Protobuf" FORCE)
set(utf8_range_ENABLE_INSTALL OFF CACHE BOOL "Don't install utf8_range" FORCE)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.76.0-pre1
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

add_library("${PROJECT_NAME}" STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/ssl_client.cpp")

target_link_libraries("${PROJECT_NAME}" PRIVATE grpc++)

add_library("nihilus::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp")

set_target_properties(
	"${PROJECT_NAME}" PROPERTIES
	OUTPUT_NAME "${PROJECT_NAME}"
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

include("cmake/flags_and_options.cmake")

target_include_directories("${PROJECT_NAME}" PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)

target_compile_options("${PROJECT_NAME}" PUBLIC
    "$<$<CONFIG:Release>:${NIHILUS_NETWORK_COMPILE_OPTIONS}>"
)

target_link_options("${PROJECT_NAME}" PUBLIC
    "$<$<CONFIG:Release>:${NIHILUS_NETWORK_LINK_OPTIONS}>"
)

target_compile_definitions("${PROJECT_NAME}" PUBLIC
    "${NIHILUS_NETWORK_COMPILE_DEFINITIONS}" 
)

set(CONFIG_FILE_NAME "${PROJECT_NAME}Config.cmake")
set(EXPORTED_TARGETS_NAME "${PROJECT_NAME}Targets")
set(EXPORTED_TARGETS_FILE_NAME "${EXPORTED_TARGETS_NAME}.cmake")

include(CMakePackageConfigHelpers)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/${EXPORTED_TARGETS_FILE_NAME}\")
")

set(VERSION_FILE_NAME "${PROJECT_NAME}ConfigVersion.cmake")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${VERSION_FILE_NAME}"
	VERSION "1.0.0"
	COMPATIBILITY AnyNewerVersion
)

install(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
	"${CMAKE_CURRENT_BINARY_DIR}/${VERSION_FILE_NAME}"
	DESTINATION "share/${PROJECT_NAME}"
)

install(
	DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/Include/"
	DESTINATION "include"
)

install(
	TARGETS "${PROJECT_NAME}"
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

install(CODE "
	file(WRITE \"\${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/${EXPORTED_TARGETS_FILE_NAME}\"
\"add_library(nihilus::${PROJECT_NAME} STATIC IMPORTED)
set_target_properties(nihilus::${PROJECT_NAME} PROPERTIES
	IMPORTED_LOCATION \\\"\\\${CMAKE_CURRENT_LIST_DIR}/../../../lib/\\\${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}\\\${CMAKE_STATIC_LIBRARY_SUFFIX}\\\"
	INTERFACE_INCLUDE_DIRECTORIES \\\"\\\${CMAKE_CURRENT_LIST_DIR}/../../../include\\\"
)
\")
")

if ("${NIHILUS_NETWORK_TEST}")
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()