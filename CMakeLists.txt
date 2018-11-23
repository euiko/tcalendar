cmake_minimum_required(VERSION 3.0.0)

include("HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.55.tar.gz"
    SHA1 "da31d97da47cdde05f958786b6729ba9b773bb17"
)

project(calendar VERSION 0.1.0)

include(CTest)
enable_testing()


include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
set(PROJECT_SRC_DIR ${CMAKE_HOME_DIRECTORY}/src)

ExternalProject_Add(project_cppurses
    GIT_REPOSITORY https://github.com/a-n-t-h-o-n-y/CPPurses
    GIT_TAG v0.1.2
    GIT_SUBMODULES
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

add_library(cppurses STATIC IMPORTED)
set_property(TARGET cppurses PROPERTY IMPORTED_LOCATION ${EXTERNAL_INSTALL_LOCATION}/lib64/libcppurses.a)
add_dependencies(cppurses project_cppurses)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

add_subdirectory(src release)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
