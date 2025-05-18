//
//
//      haze/deps
//      stb/stb_util.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/object/pixel.hxx>

#include <uti/core/container/vector.hxx>
#include <uti/core/string/string_view.hxx>

UTI_DIAGS_PUSH()
UTI_DIAGS_DISABLE( -Wdeprecated-declarations )
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
#include <stb/stb_image.h>
//#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#endif
#include <stb/stb_image_write.h>
UTI_DIAGS_POP()


namespace haze::stb
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like Pixel > constexpr vector< Pixel > load_image ( string_view _filename_, ssize_t & _width_, ssize_t & _height_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like Pixel >
constexpr void write_to_png ( string_view _filename_, vector< Pixel > const & _pixels_, ssize_t _width_, ssize_t _height_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace _detail
{

template< meta::pixel_like Pixel >
constexpr vector< stbi_uc > _pixels_to_raw ( vector< Pixel > const & _pixels_, ssize_t _width_, ssize_t _height_, ssize_t _channels_ ) noexcept
{
        vector< stbi_uc > raw( _pixels_.size() * Pixel::num_channels, {} ) ;

        for( ssize_t i = 0; i < _height_; ++i )
        {
                for( ssize_t j = 0; j < _width_; ++j )
                {
                        for( ssize_t k = 0; k < _channels_; ++k )
                        {
                                raw.at( i * _width_ * _channels_ + j * _channels_ + k ) = static_cast< stbi_uc >( _pixels_.at( i * _width_ + j ).channels[ k ] ) ;
                        }
                }
        }
        return raw ;
}

template< meta::pixel_like Pixel >
constexpr vector< Pixel > _raw_to_pixels ( uti::view< stbi_uc > const & _raw_, ssize_t _width_, ssize_t _height_, ssize_t _channels_ ) noexcept
{
        vector< Pixel > pixels( _raw_.size() ) ;

        for( ssize_t i = 0; i < _height_; ++i )
        {
                for( ssize_t j = 0; j < _width_; ++j )
                {
                        pixels.emplace_back() ;

                        for( ssize_t k = 0; k < _channels_; ++k )
                        {
                                pixels.back().channels[ k ] = _raw_.at( i * _width_ * _channels_ + j * _channels_ + k ) ;
                        }
                }
        }
        return pixels ;
}

} // namespace _detail

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like Pixel > constexpr vector< Pixel > load_image ( string_view _filename_, ssize_t & _width_, ssize_t & _height_ ) noexcept
{
        i32_t    width_file {} ;
        i32_t   height_file {} ;
        i32_t channels_file {} ;

        stbi_uc * raw_data = stbi_load( _filename_.data(), &width_file, &height_file, &channels_file, Pixel::num_channels ) ;

        _width_  =  width_file ;
        _height_ = height_file ;

        vector< Pixel > pixels = _detail::_raw_to_pixels< Pixel >( uti::view( raw_data, _width_ * _height_ * Pixel::num_channels ), _width_, _height_, Pixel::num_channels ) ;

        stbi_image_free( raw_data ) ;

        return pixels ;
}

template< meta::pixel_like Pixel > constexpr void write_to_png ( string_view _filename_, vector< Pixel > const & _pixels_, ssize_t _width_, ssize_t _height_ ) noexcept
{
        vector< stbi_uc > raw = _detail::_pixels_to_raw( _pixels_, _width_, _height_, Pixel::num_channels ) ;

        stbi_write_png( _filename_.data(), _width_, _height_, Pixel::num_channels, raw.data(), 0 ) ;
}


} // namespace haze::stb
