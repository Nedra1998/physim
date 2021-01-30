# --- Project ---

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(PhySim VERSION 0.1.0 LANGUAGES CXX C)

# --- Build Type ---

if("${CMAKE_GENERATOR}" MATCHES "Makefiles" AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

# --- Root Project ---

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(PHYSIM_MAIN ON)
endif()

# --- Options ---

option(PHYSIM_BUILD_TESTS "Enable local test generation" ${PHYSIM_MAIN})

# --- Compile Commands ---

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  if (NOT EXISTS "${PROJECT_SOURCE_DIR}/compile_commands.json")
    execute_process(COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${PROJECT_BINARY_DIR}/compile_commands.json
      ${PROJECT_SOURCE_DIR}/compile_commands.json)
  endif()
endif()

# --- Dependencies ---

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cotire)

# --- Utilities ---

if ("${CMAKE_BUILD_TYPE}" MATCHES "Release|MinSizeRel")
  find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
  find_program(CMAKE_CXX_CLANG_TIDY NAMES clang-tidy)
endif()
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# --- Source ---

add_subdirectory(dependencies)
add_subdirectory(src/physim)
add_subdirectory(src/physim-bin)
if(PHYSIM_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
