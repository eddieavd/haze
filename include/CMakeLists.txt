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

target_include_directories( haze INTERFACE ${PROJECT_SOURCE_DIR}/deps             )
target_include_directories( haze INTERFACE ${PROJECT_SOURCE_DIR}/deps/metalcpp    )
target_include_directories( haze INTERFACE ${PROJECT_SOURCE_DIR}/deps/metalcppext )

target_sources( haze PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
)

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

target_link_libraries( haze INTERFACE
        uti_core::uti_core
        loguru::loguru
)

target_compile_options( haze INTERFACE -fno-objc-arc )

target_link_libraries( haze INTERFACE "-framework Cocoa"        )
target_link_libraries( haze INTERFACE "-framework Foundation"   )
target_link_libraries( haze INTERFACE "-framework CoreGraphics" )
target_link_libraries( haze INTERFACE "-framework QuartzCore"   )
target_link_libraries( haze INTERFACE "-framework Metal"        )
target_link_libraries( haze INTERFACE "-framework MetalKit"     )
target_link_libraries( haze INTERFACE "-framework OpenGL"       )
