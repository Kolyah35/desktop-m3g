# =================================
# M3G project config file.
# =================================

# required cmake
cmake_minimum_required(VERSION 3.21)

# project info
project(desktop_m3g LANGUAGES C CXX)

# =================================
# Avairable options
# =================================
option(USE_GL "Build the desktop OpenGL backend" ON)
option(USE_GL_ES "Build the OpenGL ES 1 backend (legacy)" OFF)
option(BUILD_SAMPLES "Build the GLFW samples" ON)
option(M3G_BUILD_SHARED "Build M3G as a shared library" OFF)
option(M3G_FETCH_ZLIB "Fetch zlib if no system installation is found" ON)

if(USE_GL AND USE_GL_ES)
    message(FATAL_ERROR "USE_GL and USE_GL_ES are mutually exclusive")
elseif(NOT USE_GL AND NOT USE_GL_ES)
    message(FATAL_ERROR "Select USE_GL or USE_GL_ES")
endif()
if(BUILD_SAMPLES AND NOT USE_GL)
    message(FATAL_ERROR "The desktop GLFW samples require USE_GL=ON")
endif()

# ================================
# Default Build is "Debug"
# ================================
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type is not specified, then")
    message(STATUS "  set CMAKE_BUILD_TYPE=Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "Configuring m3g for ${CMAKE_BUILD_TYPE}...")

# for "Debug"
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "  set CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/bin")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# for "Release"
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # nothing
endif(CMAKE_BUILD_TYPE STREQUAL "Release")

# ==================================
# dependend libraries
# ==================================

find_package(ZLIB QUIET)
if(NOT ZLIB_FOUND)
    if(NOT M3G_FETCH_ZLIB)
        message(FATAL_ERROR "zlib not found: install zlib or enable M3G_FETCH_ZLIB")
    endif()
    include(cmake/CPM.cmake)

    CPMAddPackage("gh:madler/zlib@1.4")
endif()

CPMAddPackage("gh:unittest-cpp/unittest-cpp@2.0.0")

set(M3G_REQUIRED_LIBRARIES zlibstatic UnitTest++)
set(M3G_REQUIRED_INCLUDE_DIR ${zlib_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/glad/include "${unittest-cpp_SOURCE_DIR}")


# =============================
# project src
# =============================
configure_file(src/Config.hpp.in "${PROJECT_BINARY_DIR}/generated/Config.hpp")

add_subdirectory(src)

if (BUILD_SAMPLES)
    add_subdirectory(sample)
endif()

# ================================
# Confirm current configuration
# ================================
message(STATUS "Generating Config.hpp ...")
message(STATUS "USE_GL    = ${USE_GL}")
message(STATUS "USE_GL_ES = ${USE_GL_ES}")


