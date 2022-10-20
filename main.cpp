//
//
//	HAZElib
//	main.cpp
//

#include "include/HAZElib.hpp"

#ifdef __APPLE__
#       ifndef METAL
#               define METAL
#       endif
#endif

#define IMG_PATH "city2.jpeg"


[[ maybe_unused ]] constexpr static unsigned blur_radius( 64 );

int main ()
{
#ifdef METAL
        haze::gpu_ops< haze::bw_pixel > gpu;
        haze::image  < haze::bw_pixel > img( "IMG_PATH" );

        auto edge = gpu.detect_edges( img );

        haze::util::write_to_png( edge, "edge.png" );
#endif


	return 0;
}
