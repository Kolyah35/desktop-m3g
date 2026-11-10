# ==============================
# M3G compile config file.
# ==============================

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

add_library(m3g::m3g ALIAS m3g)
target_compile_features(m3g PUBLIC cxx_std_17)

# =================================
# dependent libraries
# =================================
# add_library(m3g::m3g ALIAS m3g) # Хотел выебнуться, а не получилось :(
target_compile_features(m3g PUBLIC cxx_std_17)
target_include_directories(m3g PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated>"
)
target_link_libraries(m3g PUBLIC ZLIB::ZLIB)

if(USE_GL)
    target_sources(m3g PRIVATE "${PROJECT_SOURCE_DIR}/glad/src/glad.c")
    target_include_directories(m3g PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/glad/include>")
    target_link_libraries(m3g PUBLIC ${CMAKE_DL_LIBS})
    if(M3G_BUILD_SHARED)
        target_compile_definitions(m3g PRIVATE GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)
        target_compile_definitions(m3g INTERFACE GLAD_GLAPI_EXPORT)
    endif()
endif()
