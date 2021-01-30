# --- Project ---

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Vis VERSION 0.1.0 LANGUAGES CXX)

# --- Dependencies ---

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CPM)
include(cotire)
set(CPM_USE_LOCAL_PACKAGES ON)

# --- Compile Commands ---

if("${CMAKE_GENERATOR}" MATCHES "Makefiles" AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug|RelWithDebInfo")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  if (NOT EXISTS "${PROJECT_SOURCE_DIR}/compile_commands.json")
    execute_process(COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${PROJECT_BINARY_DIR}/compile_commands.json
      ${PROJECT_SOURCE_DIR}/compile_commands.json)
  endif()
endif()

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

add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(test)
