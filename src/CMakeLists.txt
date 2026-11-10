include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# =================================
# m3g srcs
# =================================
file(GLOB M3G_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# =================================
# m3g target
# =================================
if(M3G_BUILD_SHARED)
    add_library(m3g SHARED ${M3G_SOURCES})
    set_target_properties(m3g PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    add_library(m3g STATIC ${M3G_SOURCES})
endif()

# =================================
# dependent libraries
# =================================
add_library(m3g::m3g ALIAS m3g)
target_compile_features(m3g PUBLIC cxx_std_17)
target_include_directories(m3g 
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        "${PROJECT_SOURCE_DIR}/include/m3g/math"
        "${PROJECT_SOURCE_DIR}/include/m3g"
        "${PROJECT_BINARY_DIR}/generated"
        "${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(m3g PUBLIC ZLIB::ZLIB)

if(USE_GL)
    target_sources(m3g PRIVATE "${PROJECT_SOURCE_DIR}/glad/src/glad.c")
    target_include_directories(m3g PRIVATE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/glad/include>")
    target_link_libraries(m3g PUBLIC ${CMAKE_DL_LIBS})
    if(M3G_BUILD_SHARED)
        target_compile_definitions(m3g PRIVATE GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)
        target_compile_definitions(m3g INTERFACE GLAD_GLAPI_EXPORT)
    endif()
endif()

install(TARGETS m3g EXPORT m3g_targets)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/m3g"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

install(
    EXPORT m3g_targets
    FILE m3g_targets.cmake
    NAMESPACE m3g::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/m3g
)