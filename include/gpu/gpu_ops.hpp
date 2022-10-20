//
//
//      HAZElib
//      gpu_ops.hpp
//

#pragma once


#include "../image.hpp"
#include "../img_manip.hpp"
#include "../img_transform.hpp"

#ifdef METAL
#       include "metal/metal_ops.hpp"
#else

#endif



namespace haze
{


template< typename Pixel >
class gpu_ops
{
public:
        gpu_ops ()
#ifdef METAL
                : mtl_ops( MTL::CreateSystemDefaultDevice() )
#else
#endif
        {}

        template< std::size_t Size >
        image< Pixel > transform_image ( image< Pixel > const & src, kernel< Size, Size > const & kern )
        {
#ifdef METAL
                return mtl_ops.transform_image( src, kern );
#else
#endif
        }

        image< Pixel > detect_edges ( image< Pixel > const & src )
        {
#ifdef METAL
                return mtl_ops.detect_edges( src );
#else
#endif
        }

        image< Pixel > mean_blur ( image< Pixel > const & src, unsigned const blur_radius )
        {
#ifdef METAL
                return mtl_ops.mean_blur_kern( src, blur_radius );
#else
#endif
        }

        image< Pixel > mean_blur ( pixel_field< Pixel > const & src, unsigned const blur_radius )
        {
#ifdef METAL
                return mtl_ops.mean_blur( src, blur_radius );
#else
#endif
        }

        image< Pixel > lens_blur ( image< Pixel > const & src, unsigned const blur_radius )
        {
#ifdef METAL
                return mtl_ops.lens_blur( src, blur_radius );
#else
#endif
        }

        image< Pixel > lens_blur ( pixel_field< Pixel > const & src, unsigned const blur_radius )
        {
#ifdef METAL
                return mtl_ops.lens_blur( src, blur_radius );
#else
#endif
        }
private:
#ifdef METAL
        metal_ops< Pixel > mtl_ops;
#else
#endif
};


} // namespace haze
