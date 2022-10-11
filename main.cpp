//
//
//	HAZElib
//	main.cpp
//

#include "include/HAZElib.hpp"

#include "include/img_manip.hpp"


[[ maybe_unused ]] constexpr static int blur_radius( 32 );

int main ()
{
        haze::image< haze::bw_pixel > img( "pixel.png" );

        auto naive = haze::scale_image( img, 100, haze::scaling_options::interpolation_type::naive            );
        auto neigh = haze::scale_image( img, 100, haze::scaling_options::interpolation_type::nearest_neighbor );
        auto bilin = haze::scale_image( img, 100, haze::scaling_options::interpolation_type::bilinear         );

        haze::util::write_to_png( naive, "naive.png" );
        haze::util::write_to_png( neigh, "neigh.png" );
        haze::util::write_to_png( bilin, "bilin.png" );



	return 0;
}
