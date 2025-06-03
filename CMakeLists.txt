#
#       haze
#       CMakeLists.txt
#

cmake_minimum_required( VERSION 3.28.0 )

project(
        haze
        VERSION      0.0.1
        DESCRIPTION "haze"
        LANGUAGES    CXX
)

set( HAZE_COMPILE_OPTIONS -Wall -Wextra -pedantic -fno-exceptions -fno-rtti )

set( CMAKE_CXX_STANDARD            23 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

if( CMAKE_BUILD_TYPE OR CMAKE_CONFIGURATION_TYPES )
        set( HAZE_DEBUG_OPT_LVL   -O0 )
        set( HAZE_RELEASE_OPT_LVL -O3 )

        set( HAZE_OPT_LVL "$<$<CONFIG:DEBUG>:${HAZE_DEBUG_OPT_LVL}>"
                          "$<$<CONFIG:RELEASE>:${HAZE_RELEASE_OPT_LVL}>"
        )
endif()

add_subdirectory( source )

