//
//
//      HAZElib
//      gpu_ops.hpp
//

#pragma once


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
