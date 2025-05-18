#
#       haze
#       include/CMakeLists.txt
#

add_library( haze INTERFACE )
add_library( haze::haze ALIAS haze )

set_property( TARGET haze PROPERTY SOVERSION "${PROJECT_VERSION}" )

if( CMAKE_BUILD_TYPE OR CMAKE_CONFIGURATION_TYPES )
        set( HAZE_DEBUG_OPTIONS   -DHAZE_DEBUG  )
        set( HAZE_RELEASE_OPTIONS -DHAZE_RELESE )

        target_compile_options( haze INTERFACE "$<$<CONFIG:DEBUG>:${HAZE_DEBUG_OPTIONS}>" )
        target_compile_options( haze INTERFACE "$<$<CONFIG:RELEASE>:${HAZE_RELEASE_OPTIONS}>" )
endif()

target_compile_options( haze INTERFACE -std=c++26                )
target_compile_options( haze INTERFACE "${HAZE_COMPILE_OPTIONS}" )

target_include_directories( haze INTERFACE ${PROJECT_SOURCE_DIR}/deps )

target_sources( haze PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
#       FILES haze.hxx
)

include( FetchContent )

FetchContent_Declare(
        uti
        GIT_REPOSITORY https://github.com/eddieavd/uti.git
        GIT_TAG        master
)
FetchContent_MakeAvailable( uti )

target_link_libraries( haze INTERFACE
        uti_core::uti_core
        uti_sys::uti_sys
)
