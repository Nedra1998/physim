cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Subdirectories ---

add_subdirectory(cxxopts EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(magic_enum EXCLUDE_FROM_ALL)

set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# --- glad ---

add_library(glad EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
add_library(glad::glad ALIAS glad)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_link_libraries(glad PUBLIC glfw)

# --- imgui ---

add_library(
  imgui EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp)
add_library(imgui::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

add_library(imgui-glfw EXCLUDE_FROM_ALL
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp)
add_library(imgui::glfw ALIAS imgui-glfw)
target_include_directories(imgui-glfw
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)
target_link_libraries(imgui-glfw PUBLIC imgui glfw)

add_library(imgui-opengl3 EXCLUDE_FROM_ALL
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui::opengl3 ALIAS imgui-opengl3)
target_include_directories(imgui-opengl3
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)
target_link_libraries(imgui-opengl3 PUBLIC imgui glad)
target_compile_definitions(imgui-opengl3 PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

add_library(
  imgui-implot EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_items.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot.cpp)
add_library(imgui::implot ALIAS imgui-implot)
target_include_directories(imgui-implot
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/implot)
target_link_libraries(imgui-implot PUBLIC imgui)
