//
//
//	HAZElib
//	main.cpp
//

#include "include/HAZElib.hpp"

#ifndef METAL
#define METAL
#endif

#define IMG_PATH "city2.jpeg"


[[ maybe_unused ]] constexpr static unsigned blur_radius( 64 );

int main ()
{
        haze::gpu_ops    < haze::thicc_rgb_pixel > gpu;
        haze::pixel_field< haze::thicc_rgb_pixel > field( IMG_PATH );

        haze::image blurred = gpu.lens_blur( field, blur_radius );

        haze::util::write_to_png( blurred, "gpu_lens.png" );


	return 0;
}
