cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Options ---

# --- Dependencies ---

include(cotire)
include(Catch)
include(EnableExtraCompilerWarnings)

# --- Sources ---

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# --- Target ---

add_executable(physim-test ${SOURCES} ${INCLUDE})
target_include_directories(physim-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/)

if(NOT BIN_DIR)
  set_target_properties(
    physim-test
    PROPERTIES CXX_STANDARD 20
               OUTPUT_NAME local-test
               RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
else()
  set_target_properties(
    physim-test
    PROPERTIES CXX_STANDARD 20
               OUTPUT_NAME local-test
               RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
endif()
target_link_libraries(physim-test PUBLIC physim-lib Catch2::Catch2WithMain)

enable_extra_compiler_warnings(physim-test)
cotire(physim-test)
catch_discover_tests(physim-test)
