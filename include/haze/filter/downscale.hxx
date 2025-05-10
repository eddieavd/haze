//
//
//      haze
//      filter/downscale.hxx
//

#pragma once

#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType >
constexpr ImageType downscale_linear ( ImageType const & _image_, double _factor_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType >
constexpr ImageType downscale_linear ( ImageType const & _image_, double _factor_ ) noexcept
{
        ImageType scaled( _image_.width() / _factor_, _image_.height() / _factor_ ) ;

        for( ssize_t i = 0; i < scaled.height(); ++i )
        {
                for( ssize_t j = 0; j < scaled.width(); ++j )
                {
                        scaled.at( i, j ) = _image_.at( i * _factor_, j * _factor_ ) ;
                }
        }
        return scaled ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
