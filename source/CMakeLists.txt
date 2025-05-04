#
#       haze
#       source/CMakeLists.txt
#

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

target_compile_options( haze PRIVATE -std=c++26               )
target_compile_options( haze PRIVATE "${HAZE_OPT_LVL}"         )
target_compile_options( haze PRIVATE "${HAZE_COMPILE_OPTIONS}" )

target_include_directories( haze PRIVATE ${PROJECT_SOURCE_DIR}/deps )

target_sources( haze PRIVATE
        haze/haze.cxx
)
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

target_link_libraries( haze PRIVATE
        uti_core::uti_core
        uti_sys::uti_sys
)

add_executable( haze_demo demo.cxx )

target_compile_options( haze_demo PRIVATE -std=c++26                )
target_compile_options( haze_demo PRIVATE "${HAZE_OPT_LVL}"         )
target_compile_options( haze_demo PRIVATE "${HAZE_COMPILE_OPTIONS}" )
# target_compile_options( haze_demo PRIVATE -g -fsanitize=address -fno-omit-frame-pointer )

target_include_directories( haze_demo PRIVATE ${PROJECT_SOURCE_DIR}/deps )

target_link_libraries( haze_demo PRIVATE
        haze::haze
        uti_core::uti_core
        uti_sys::uti_sys
        c++
)
