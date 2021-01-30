cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Subdirectories ---

add_subdirectory(cxxopts EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(magic_enum EXCLUDE_FROM_ALL)

# --- glad ---

add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)


