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
option(M3G_BUILD_SAMPLES "Build the GLFW samples" ${PROJECT_IS_TOP_LEVEL})
option(M3G_BUILD_TESTS "Build tests" OFF)
option(M3G_BUILD_SHARED "Build M3G as a shared library" OFF)
option(M3G_FETCH_ZLIB "Fetch zlib if no system installation is found" ON)

if(USE_GL AND USE_GL_ES)
    message(FATAL_ERROR "USE_GL and USE_GL_ES are mutually exclusive")
elseif(NOT USE_GL AND NOT USE_GL_ES)
    message(FATAL_ERROR "Select USE_GL or USE_GL_ES")
endif()
if(M3G_BUILD_SAMPLES AND NOT USE_GL)
    message(FATAL_ERROR "The desktop GLFW samples require USE_GL=ON")
endif()


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

if (M3G_BUILD_TESTS)
    CPMAddPackage("gh:unittest-cpp/unittest-cpp@2.0.0")

    # TODO: add_subdirectory(tests)
endif()

set(M3G_REQUIRED_LIBRARIES zlibstatic UnitTest++)
set(M3G_REQUIRED_INCLUDE_DIR ${zlib_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/glad/include "${unittest-cpp_SOURCE_DIR}")


# =============================
# project src
# =============================
configure_file(src/Config.hpp.in "${PROJECT_BINARY_DIR}/generated/Config.hpp")

add_subdirectory(src)

if (M3G_BUILD_SAMPLES)
    add_subdirectory(sample)
endif()

# ================================
# Confirm current configuration
# ================================
message(STATUS "Generating Config.hpp ...")
message(STATUS "USE_GL    = ${USE_GL}")
message(STATUS "USE_GL_ES = ${USE_GL_ES}")


