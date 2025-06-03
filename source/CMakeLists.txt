#
#       haze
#       source/CMakeLists.txt
#

################################################################################
### haze library
################################################################################

add_library( haze )
add_library( haze::haze ALIAS haze )

set_property( TARGET haze PROPERTY SOVERSION "${PROJECT_VERSION}" )

if( CMAKE_BUILD_TYPE OR CMAKE_CONFIGURATION_TYPES )
        set( HAZE_DEBUG_OPTIONS   -DHAZE_DEBUG  )
        set( HAZE_RELEASE_OPTIONS -DHAZE_RELESE )

        set_target_properties( haze PROPERTIES ARCHIVE_OUTPUT_NAME "haze.$<LOWER_CASE:$<CONFIG>>" )
        set_target_properties( haze PROPERTIES LIBRARY_OUTPUT_NAME "haze.$<LOWER_CASE:$<CONFIG>>" )

        target_compile_options( haze PRIVATE "$<$<CONFIG:DEBUG>:${HAZE_DEBUG_OPTIONS}>" )
        target_compile_options( haze PRIVATE "$<$<CONFIG:RELEASE>:${HAZE_RELEASE_OPTIONS}>" )
endif()

target_sources( haze PRIVATE
        ${PROJECT_SOURCE_DIR}/source/haze/gfx/backend/metal/common.cxx
        ${PROJECT_SOURCE_DIR}/source/haze/gfx/backend/metal/allocator.cxx
        ${PROJECT_SOURCE_DIR}/source/haze/gfx/backend/metal/app.cxx
        ${PROJECT_SOURCE_DIR}/source/haze/gfx/backend/metal/glfw_adapter.mm
)
target_sources( haze PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
)

################################################################################
### catch uti
################################################################################

include( FetchContent )

FetchContent_Declare(
        uti
        GIT_REPOSITORY https://github.com/eddieavd/uti.git
        GIT_TAG        master
)
FetchContent_GetProperties( uti )
if( NOT uti_POPULATED )
        set( FETCHCONTENT_QUIET NO )
        FetchContent_MakeAvailable( uti )
endif()

FetchContent_Declare(
        loguru
        GIT_REPOSITORY https://github.com/emilk/loguru.git
        GIT_TAG        master
)
FetchContent_MakeAvailable( loguru )

################################################################################
################################################################################

target_include_directories( haze PUBLIC /opt/homebrew/include )
target_link_directories   ( haze PUBLIC /opt/homebrew/lib     )

target_link_libraries( haze PUBLIC
        uti_core::uti_core
        loguru::loguru
)

target_compile_options( haze PRIVATE -std=c++23                )
target_compile_options( haze PRIVATE "${HAZE_COMPILE_OPTIONS}" )

target_include_directories( haze PUBLIC ${PROJECT_SOURCE_DIR}/deps             )
target_include_directories( haze PUBLIC ${PROJECT_SOURCE_DIR}/deps/metalcpp    )
target_include_directories( haze PUBLIC ${PROJECT_SOURCE_DIR}/deps/metalcppext )

target_link_libraries( haze PUBLIC
    "-framework Metal" 
    "-framework Foundation" 
    "-framework QuartzCore"
    "-framework AppKit" objc
)
target_link_libraries( haze PUBLIC glfw )

################################################################################
################################################################################

add_executable( haze_demo demo.cxx )

target_link_libraries( haze_demo PRIVATE haze::haze )
