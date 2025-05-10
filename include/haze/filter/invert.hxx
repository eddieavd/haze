//
//
//      haze
//      filter/invert.hxx
//

#pragma once

#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>


namespace haze
{


template< meta::image_like ImageType >
constexpr void invert_image ( ImageType & _image_ ) noexcept
{
        for( auto & pixel : _image_ )
        {
                pixel.channels[ 0 ] = 255 - pixel.channels[ 0 ] ;
                pixel.channels[ 1 ] = 255 - pixel.channels[ 1 ] ;
                pixel.channels[ 2 ] = 255 - pixel.channels[ 2 ] ;
        }
}


} // namespace haze
