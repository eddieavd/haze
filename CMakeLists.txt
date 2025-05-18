#
#       haze
#       CMakeLists.txt
#

cmake_minimum_required( VERSION 3.23 )

project( haze
         VERSION      0.0.1
         DESCRIPTION "haze - image manipulation library"
         LANGUAGES    CXX
)

set( HAZE_COMPILE_OPTIONS -Wall -Wextra -pedantic
                          -ftemplate-backtrace-limit=0
                          -fconstexpr-backtrace-limit=0
                          -fno-exceptions -fno-rtti
                          -DUTI_RELEASE
)
if( CMAKE_BUILD_TYPE OR CMAKE_CONFIGURATION_TYPES )
        set( HAZE_DEBUG_OPT_LVL   -O0 )
        set( HAZE_RELEASE_OPT_LVL -O3 )

        set( HAZE_OPT_LVL "$<$<CONFIG:DEBUG>:${HAZE_DEBUG_OPT_LVL}>"
                          "$<$<CONFIG:DEBUG>:${HAZE_DEBUG_OPT_LVL}>"
        )
endif()

add_subdirectory(  include )
#add_subdirectory( examples )

if( HAZE_TEST )
        add_subdirectory( test )
endif()

if( HAZE_BENCH )
        add_subdirectory( bench )
endif()
