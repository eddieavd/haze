//
//
//      haze
//      core/object/image_io.hxx
//

#pragma once

#include <stb/stb_util.hxx>

#include <haze/core/object/image.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > constexpr image< PixelType > load_image ( string_view _filename_                                     ) noexcept ;
template< meta::pixel_like PixelType > constexpr void              store_image ( string_view _filename_, image< PixelType > const & _image_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > constexpr image< PixelType > load_image ( string_view _filename_ ) noexcept
{
        ssize_t  width ;
        ssize_t height ;
        auto    pixels = stb::load_image< PixelType >( _filename_, width, height ) ;

        return image< PixelType >( UTI_MOVE( pixels ), width, height ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > constexpr void store_image ( string_view _filename_, image< PixelType > const & _image_ ) noexcept
{
        stb::write_to_png( _filename_, _image_.pixels(), _image_.width(), _image_.height() ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
