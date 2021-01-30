cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Options ---

# --- Dependencies ---

include(cotire)
include(Analyzers)
include(EnableExtraCompilerWarnings)

# --- Sources ---

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# --- Target ---

add_library(physim-lib ${SOURCES} ${INCLUDE})
set_target_properties(physim-lib PROPERTIES CXX_STANDARD 20)

target_include_directories(physim-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(physim-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/physim)
target_link_libraries(physim-lib PUBLIC spdlog fmt)

add_analizers(physim-lib)
enable_extra_compiler_warnings(physim-lib)
cotire(physim-lib)
