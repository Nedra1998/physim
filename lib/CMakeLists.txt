cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# --- Options ---

# --- Dependencies ---

include(cotire)
include(EnableExtraCompilerWarnings)

# cpmaddpackage(
#   NAME
#   Eigen
#   GITLAB_REPOSITORY
#   libeigen/eigen
#   GIT_TAG
#   3.3.9
#   OPTIONS
#   "EIGEN_TEST_CXX11 On"
#   "EIGEN_BUILD_DOCS Off"
#   "BUILD_TESTING Off")
cpmaddpackage(NAME fmt GITHUB_REPOSITORY fmtlib/fmt GIT_TAG 7.1.3)
cpmaddpackage(NAME spdlog GITHUB_REPOSITORY gabime/spdlog GIT_TAG v1.8.2)

# --- Sources ---

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# --- Target ---

add_library(physim-lib ${SOURCES} ${INCLUDE})
set_target_properties(physim-lib PROPERTIES CXX_STANDARD 20)

target_include_directories(physim-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
# target_link_libraries(physim-lib PRIVATE eigen)
target_link_libraries(physim-lib PUBLIC spdlog fmt)

enable_extra_compiler_warnings(physim-lib)
cotire(physim-lib)
