#
#       haze
#       source/CMakeLists.txt
#

set( RAYLIB_VERSION 5.5 )
include( FetchContent )
FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
)
FetchContent_GetProperties( raylib )
if( NOT raylib_POPULATED )
        set( FETCHCONTENT_QUIET NO )
        FetchContent_MakeAvailable( raylib )
endif()

add_executable( haze_demo demo.cxx )

target_compile_options( haze_demo PRIVATE -std=c++26                )
target_compile_options( haze_demo PRIVATE "${HAZE_OPT_LVL}"         )
target_compile_options( haze_demo PRIVATE "${HAZE_COMPILE_OPTIONS}" )
# target_compile_options( haze_demo PRIVATE -g -fsanitize=address -fno-omit-frame-pointer )

target_include_directories( haze_demo PRIVATE ${PROJECT_SOURCE_DIR}/deps )
target_include_directories( haze_demo PRIVATE /opt/homebrew/include )

target_link_directories( haze_demo PRIVATE /opt/homebrew/lib )

target_link_libraries( haze_demo PRIVATE
        haze::haze
        uti_core::uti_core
        uti_sys::uti_sys
        raylib
        avcodec
        avformat
        avutil
        avfilter
        avdevice
        swscale
        c++
)

target_link_libraries( haze_demo PRIVATE "-framework IOKit"  )
target_link_libraries( haze_demo PRIVATE "-framework Cocoa"  )
target_link_libraries( haze_demo PRIVATE "-framework OpenGL" )
