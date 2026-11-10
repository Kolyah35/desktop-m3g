# I tried to fetch GLFW via CPM but i fcking failed...
include(FetchContent)

find_package(glfw3 3.3 CONFIG QUIET)
if(TARGET glfw)
    set(M3G_GLFW_TARGET glfw)
elseif(TARGET glfw3::glfw)
    set(M3G_GLFW_TARGET glfw3::glfw)
else()
    option(M3G_FETCH_GLFW "Fetch GLFW 3.4 if no installed package is found" ON)
    if(NOT M3G_FETCH_GLFW)
        message(FATAL_ERROR "GLFW 3.3+ not found; install glfw3 or enable M3G_FETCH_GLFW")
    endif()
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(glfw
        URL "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(glfw)
    set(M3G_GLFW_TARGET glfw)
endif()

file(GLOB SAMPLE_DIRECTORIES CONFIGURE_DEPENDS LIST_DIRECTORIES true
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/sample-*"
)

foreach(sample_name IN LISTS SAMPLE_DIRECTORIES)
    set(sample_dir "${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}")

    if(NOT IS_DIRECTORY "${sample_dir}")
        continue()
    endif()

    file(GLOB sample_sources CONFIGURE_DEPENDS "${sample_dir}/*.cpp")

    if(NOT sample_sources)
        continue()
    endif()

    add_executable(${sample_name} ${sample_sources})
    target_compile_features(${sample_name} PRIVATE cxx_std_20)
    target_include_directories(${sample_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/support")
    target_link_libraries(${sample_name} PRIVATE m3g::m3g ${M3G_GLFW_TARGET})

    set_target_properties(${sample_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${sample_name}"
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${sample_name}>"
    )

    if(M3G_BUILD_SHARED AND WIN32)
        add_custom_command(TARGET ${sample_name} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:m3g>" "$<TARGET_FILE_DIR:${sample_name}>" 
            VERBATIM
        )
    endif()

    add_custom_command(TARGET ${sample_name} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:zlib>" "$<TARGET_FILE_DIR:${sample_name}>"
        VERBATIM
    )

    if(IS_DIRECTORY "${sample_dir}/resources")
        file(GLOB_RECURSE assets CONFIGURE_DEPENDS LIST_DIRECTORIES false "${sample_dir}/resources/*")

        add_custom_command(TARGET ${sample_name} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${sample_dir}/resources"
                "$<TARGET_FILE_DIR:${sample_name}>/resources"
            COMMENT "Deploying resources for ${sample_name}"
            VERBATIM
        )

        add_custom_target(${sample_name}-assets ALL
            COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${sample_dir}/resources"
                "$<TARGET_FILE_DIR:${sample_name}>/resources"
            DEPENDS ${sample_name} ${assets}
            VERBATIM
        )
    endif()
endforeach()
