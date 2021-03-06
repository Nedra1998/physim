cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Options ---

# --- Dependencies ---

include(cotire)
include(Resources)
include(EnableExtraCompilerWarnings)

# --- Resources ---

create_resources(${PROJECT_SOURCE_DIR}/resources/fonts/iosevka
                 ${CMAKE_CURRENT_BINARY_DIR})

# --- Sources ---

file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB_RECURSE CXX_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE C_INCLUDE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE CXX_INCLUDE CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
set(SOURCES ${C_SOURCES} ${CXX_SOURCES})
set(INCLUDE ${C_INCLUDE} ${CXX_INCLUDE})

list(
  APPEND
  SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/iosevka_bold.h
  ${CMAKE_CURRENT_BINARY_DIR}/iosevka_bold_italic.h
  ${CMAKE_CURRENT_BINARY_DIR}/iosevka_italic.h
  ${CMAKE_CURRENT_BINARY_DIR}/iosevka_regular.h)

# --- Target ---

add_executable(physim-bin ${SOURCES} ${INCLUDE})
target_include_directories(physim-bin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
                                              ${CMAKE_CURRENT_BINARY_DIR}/)

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
target_link_libraries(
  physim-bin
  PUBLIC physim-lib
         cxxopts
         magic_enum
         glfw
         glad::glad
         imgui::imgui
         imgui::glfw
         imgui::opengl3
         imgui::implot)

add_analizers(physim-bin)
enable_extra_compiler_warnings(physim-bin)
cotire(physim-bin)
