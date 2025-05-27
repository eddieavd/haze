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

template< meta::image_like ImageType > constexpr ImageType load_image ( string_view _filename_                            ) noexcept ;
template< meta::image_like ImageType > constexpr void     store_image ( string_view _filename_, ImageType const & _image_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType > constexpr ImageType load_image ( string_view _filename_ ) noexcept
{
        ssize_t  width ;
        ssize_t height ;
        auto    pixels = stb::load_image< typename ImageType::pixel_type >( _filename_, width, height ) ;

        typename ImageType::shape_type shape {} ;

        shape.end_point().x() =  width ;
        shape.end_point().y() = height ;

        return ImageType( UTI_MOVE( pixels ), shape ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType > constexpr void store_image ( string_view _filename_, ImageType const & _image_ ) noexcept
{
        stb::write_to_png< typename ImageType::pixel_type >( _filename_, _image_.pixels(), _image_.width(), _image_.height() ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
