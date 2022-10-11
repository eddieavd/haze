//
//
//      HAZElib
//      stb_util.hpp
//

#pragma once

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "deps/stblib"

#include "image.hpp"


namespace haze
{


template< typename Pixel >
class image;


namespace util
{


template< typename Pixel >
constexpr void raw_to_pixels ( stbi_uc *  raw_data, Pixel * pixels, std::size_t const width, std::size_t const height, std::size_t const channels );

template< typename Pixel >
constexpr void pixels_to_raw ( Pixel * pixels, stbi_uc * raw_data, std::size_t const width, std::size_t const height, std::size_t const channels = sizeof( Pixel ) );

template< typename Pixel >
constexpr Pixel * load_image ( std::string const & filename, std::size_t * width, std::size_t * height );

template< typename Pixel >
constexpr void write_to_jpg ( image< Pixel > const & image, std::string const & filename, int const quality );

template< typename Pixel >
constexpr void write_to_png ( image< Pixel > const & image, std::string const & filename );


namespace detail
{


template< typename Pixel >
constexpr void _raw_to_pixels_impl ( stbi_uc * raw_data, Pixel * pixels, std::size_t const width, std::size_t const height, std::size_t const channels )
{
        for( std::size_t i = 0; i < height; ++i )
        {
                for( std::size_t j = 0; j < width; ++j )
                {
                        for( std::size_t c = 0; c < channels; ++c )
                        {
                                pixels[ i * width + j ].values[ c ] = raw_data[ i * width * channels + j * channels + c ];
                        }
                }
        }
}

template< typename Pixel >
constexpr void _pixels_to_raw_impl ( Pixel * pixels, stbi_uc * raw_data, std::size_t const width, std::size_t const height, std::size_t const channels )
{
        for( std::size_t i = 0; i < height; ++i )
        {
                for( std::size_t j = 0; j < width; ++j )
                {
                        for( std::size_t c = 0; c < channels; ++c )
                        {
                                raw_data[ i * width * channels + j * channels + c ] = pixels[ i * width + j ].values[ c ];
                        }
                }
        }
}

template< typename Pixel >
constexpr Pixel * _load_image_impl ( std::string const & filename, std::size_t * width, std::size_t * height )
{
        int width_file    { 0 };
        int height_file   { 0 };
        int channels_file { 0 };

        stbi_uc * raw_data = stbi_load( filename.c_str(), &width_file, &height_file, &channels_file, sizeof( Pixel ) );

        *width  = width_file;
        *height = height_file;

        Pixel * pixels = new Pixel[ width_file * height_file ];

        _raw_to_pixels_impl( raw_data, pixels, *width, *height, sizeof( Pixel ) );

        stbi_image_free( raw_data );

        return pixels;
}

template< typename Pixel >
constexpr void _write_to_jpg_impl ( image< Pixel > const & img, std::string const & filename, int const quality )
{
        stbi_uc * raw_data = new stbi_uc[ img.width() * img.height() * img.channels() ];

        _pixels_to_raw_impl( img.data(), raw_data, img.width(), img.height(), img.channels() );

        stbi_write_jpg( filename.c_str(), img.width(), img.height(), img.channels(), raw_data, quality );

        delete[] raw_data;
}

template< typename Pixel >
constexpr void _write_to_png_impl ( image< Pixel > const & img, std::string const & filename )
{
        stbi_uc * raw_data = new stbi_uc[ img.width() * img.height() * img.channels() ];

        _pixels_to_raw_impl( img.data(), raw_data, img.width(), img.height(), img.channels() );

        stbi_write_png( filename.c_str(), img.width(), img.height(), img.channels(), raw_data, 0 ); // 0 is stride bytes, prolly should take as arg

        delete[] raw_data;
}


} // namespace detail


template< typename Pixel >
constexpr void raw_to_pixels ( stbi_uc *  raw_data, Pixel * pixels, std::size_t const width, std::size_t const height, std::size_t const channels )
{
        detail::_raw_to_pixels_impl( raw_data, pixels, width, height, channels );
}

template< typename Pixel >
constexpr void pixels_to_raw ( Pixel * pixels, stbi_uc * raw_data, std::size_t const width, std::size_t const height, std::size_t const channels )
{
        detail::_pixels_to_raw_impl( pixels, raw_data, width, height, channels );
}

template< typename Pixel >
constexpr Pixel * load_image ( std::string const & filename, std::size_t * width, std::size_t * height )
{
        return detail::_load_image_impl< Pixel >( filename, width, height );
}

template< typename Pixel >
constexpr void write_to_jpg ( image< Pixel > const & image, std::string const & filename, int const quality )
{
        detail::_write_to_jpg_impl( image, filename, quality );
}

template< typename Pixel >
constexpr void write_to_png ( image< Pixel > const & image, std::string const & filename )
{
        detail::_write_to_png_impl( image, filename );
}


} // namespace util


} // namespace haze
