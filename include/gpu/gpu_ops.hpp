//
//
//      HAZElib
//      gpu_ops.hpp
//

#pragma once

#define METAL


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
                : device_( MTL::CreateSystemDefaultDevice() ),
                  mtl_ops( device_ )
#else

#endif
        {}

        image< Pixel > blur_image ( image< Pixel > const & src, int const blur_radius )
        {
                image< Pixel > blr( src.width() - blur_radius, src.height() - blur_radius );
#ifdef METAL
                mtl_ops.blur_image( src, blr, blur_radius );
#else

#endif
                return blr;
        }
private:
#ifdef METAL
        MTL::Device * device_;
        metal_ops< Pixel > mtl_ops;
#else

#endif
};


} // namespace haze
