cmake_minimum_required( VERSION 3.21 )

project( HAZElib VERSION 0.1.0 )

option( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_CXX_STANDARD          20   )
set( CMAKE_CXX_STANDARD_REQUIRED True )

add_compile_options( -Wall -Wextra -pedantic -O3 )
#add_link_options( -fsanitize=address -fsanitize=undefined )

add_subdirectory( include )
add_subdirectory(    test )
add_subdirectory(   bench )

if( HAZE_DEBUG )
        add_compile_options( -DHAZE_DEBUG -DNPL_DEBUG )
endif()

if( HAZE_RELEASE )
        add_compile_options( -DHAZE_RELEASE -DNPL_RELEASE )
endif()

add_executable( HAZElib main.cpp )

target_link_libraries( HAZElib PUBLIC ${EXTRA_LIBS} )

if( METAL )
        add_link_options( -fno-objc-arc )
        target_link_libraries( HAZElib PUBLIC "-framework Metal" "-framework MetalKit" "-framework Foundation" )
endif()
