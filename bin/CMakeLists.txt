cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Options ---

# --- Dependencies ---

include(cotire)
include(EnableExtraCompilerWarnings)

cpmaddpackage(
  NAME
  cxxopts
  GITHUB_REPOSITORY
  jarro2783/cxxopts
  VERSION
  2.2.1
  OPTIONS
  "CXXOPTS_BUILD_EXAMPLES Off"
  "CXXOPTS_BUILD_TESTS Off")

# --- Sources ---

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# --- Target ---

add_executable(physim-bin ${SOURCES} ${INCLUDE})
target_include_directories(physim-bin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/)

if(NOT BIN_DIR)
  set_target_properties(
    physim-bin
    PROPERTIES CXX_STANDARD 20
               OUTPUT_NAME sim
               RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
else()
  set_target_properties(
    physim-bin
    PROPERTIES CXX_STANDARD 20
               OUTPUT_NAME sim
               RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
endif()
target_link_libraries(physim-bin PUBLIC physim-lib cxxopts)

enable_extra_compiler_warnings(physim-bin)
cotire(physim-bin)
