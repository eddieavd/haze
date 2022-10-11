//
//
//      HAZElib
//      img_manip.hpp
//

#pragma once


#include <array>
#include <cmath>

#include "image.hpp"
#include "util.hpp"

#include "deps/natprolib"


namespace haze
{


struct scaling_options
{
        enum interpolation_type
        {
                naive,
                nearest_neighbor,
                bilinear,
                bicubic
        };

        float              scale_x;
        float              scale_y;
        interpolation_type type   ;
};

template< typename Pixel >
constexpr image< Pixel > scale_image ( image< Pixel > const & img, scaling_options const & options );

template< typename Pixel >
constexpr image< Pixel > scale_image ( image< Pixel > const & img, float const scale_x, float const scale_y, scaling_options::interpolation_type type );

template< typename Pixel >
constexpr image< Pixel > scale_image ( image< Pixel > const & img, float const scale, scaling_options::interpolation_type type );

template< typename Pixel >
constexpr image< Pixel > pixelize_image_naive ( image< Pixel > const & img, std::size_t const pixel_size );

template< typename Pixel >
constexpr image< Pixel > blur_image_naive ( image< Pixel > const & img, std::size_t const blur_radius );

template< typename Pixel >
constexpr image< Pixel > get_brightest_rect_naive ( image< Pixel > const & img, std::size_t const rect_width, std::size_t const rect_height );

template< typename Pixel >
class pixel_field
{
public:
        pixel_field ( image< Pixel > const & img )
                : width_( img.width() ), height_( img.height() )
        {
                _fill_pixel_field( img );
        }

        pixel_field ( std::string const & filename )
        {
                int width_file;
                int height_file;
                int channels_file;

                stbi_uc * raw_data = stbi_load( filename.c_str(), &width_file, &height_file, &channels_file, sizeof( Pixel ) );

                if( raw_data )
                {
                        width_  = width_file;
                        height_ = height_file;

                        _fill_pixel_field( raw_data );
                        stbi_image_free( raw_data );
                }
        }

        pixel_field ( pixel_field< Pixel > const &  ) = delete;
        pixel_field ( pixel_field< Pixel >       && ) = delete;

        image< Pixel > get_image () const
        {
                HAZE_ASSERT( !empty(), "pixel_field::get_image: called on empty pixel field" );

                image< Pixel > img( width_, height_ );

                _fill_image( img );

                return img;
        }

        image< Pixel > get_subimage ( std::size_t const x1, std::size_t const y1, std::size_t const x2, std::size_t y2 ) const
        {
                HAZE_ASSERT( x1 <= x2 && y1 < y2 && x2 < width() && y2 < height(), "pixel_field::get_subimage: out of range" );

                image< Pixel > img( x2 - x1, y2 - y1 );

                _fill_image( img, x1, y1, x2, y2 );

                return img;
        }

        image< Pixel > get_blurred_image ( std::size_t const blur_radius ) const
        {
                HAZE_ASSERT( !empty(), "pixel_field::get_blurred_image: called on empty pixel field" );

                image< Pixel > blr( width_, height_ );

                _blur_image( blr, blur_radius );

                return blr;
        }

        image< Pixel > get_blurred_subimage ( std::size_t const x1, std::size_t const y1, std::size_t const x2, std::size_t const y2, std::size_t const blur_radius ) const
        {
                HAZE_ASSERT( x1 <= x2 && y1 <= y2 && x2 < width() && y2 < height(), "pixel_field::get_blurred_subimage: out of bounds" );

                image< Pixel > blr( x2 - x1 + 1, y2 - y1 + 1 );

                _blur_image( blr, blur_radius, x1, y1, x2, y2 );

                return blr;
        }

        [[ nodiscard ]] constexpr auto    width () const noexcept { return this->width_   ; }
        [[ nodiscard ]] constexpr auto   height () const noexcept { return this->height_  ; }
        [[ nodiscard ]] constexpr auto channels () const noexcept { return this->channels_; }

        [[ nodiscard ]] constexpr bool    empty () const noexcept { return width_ == 0 && height_ == 0; }
private:
        std::size_t width_    { 0 };
        std::size_t height_   { 0 };
        std::size_t channels_ { sizeof( Pixel ) };

        std::array< npl::rq::prefix_array< npl::rq::prefix_array< long long > >, sizeof( Pixel ) > fields_;

        void _fill_pixel_field ( image< Pixel > const & img );
        void _fill_pixel_field ( stbi_uc              * raw );

        void _fill_image ( image< Pixel > & dest ) const noexcept;
        void _fill_image ( image< Pixel > & dest,
                           std::size_t const x1, std::size_t const y1,
                           std::size_t const x2, std::size_t const y2 ) const noexcept;

        void _blur_image ( image< Pixel > & dest, std::size_t const blur_radius ) const noexcept;
        void _blur_image ( image< Pixel > & dest, std::size_t const blur_radius,
                           std::size_t const x1, std::size_t const y1,
                           std::size_t const x2, std::size_t const y2 ) const noexcept;
};

template< typename Pixel >
void pixel_field< Pixel >::_fill_pixel_field ( image< Pixel > const & img )
{
        for( std::size_t i = 0; i < img.height(); ++i )
        {
                std::array< npl::rq::prefix_array< long long >, sizeof( Pixel ) > lines;

                for( std::size_t j = 0; j < img.width(); ++j )
                {
                        for( std::size_t k = 0; k < img.channels(); ++k )
                        {
                                lines.at( k ).push_back( img.at( i, j ).values[ k ] );
                        }
                }

                for( std::size_t k = 0; k < sizeof( Pixel ); ++k )
                {
                        this->fields_.at( k ).emplace_back( std::move( lines.at( k ) ) );
                }
        }
}

template< typename Pixel >
void pixel_field< Pixel >::_fill_pixel_field ( stbi_uc * raw )
{
        for( std::size_t i = 0; i < height_; ++i )
        {
                std::array< npl::rq::prefix_array< long long >, sizeof( Pixel ) > lines;

                for( std::size_t j = 0; j < width_; ++j )
                {
                        for( std::size_t k = 0; k < channels_; ++k )
                        {
                                lines.at( k ).push_back( raw[ i * width_ * channels_ + j * channels_ + k ] );
                        }
                }

                for( std::size_t k = 0; k < sizeof( Pixel ); ++k )
                {
                        this->fields_.at( k ).emplace_back( std::move( lines.at( k ) ) );
                }
        }
}

template< typename Pixel >
void pixel_field< Pixel >::_fill_image ( image< Pixel > & dest ) const noexcept
{
        _fill_image( dest, 0, 0, this->width_ - 1, this->height_ - 1 );
}

template< typename Pixel >
void pixel_field< Pixel >::_fill_image ( image< Pixel > & dest, std::size_t const x1, std::size_t const y1, std::size_t const x2, std::size_t const y2 ) const noexcept
{
        for( std::size_t i = y1; i <= y2; ++i )
        {
                for( std::size_t j = x1; j <= x2; ++j )
                {
                        for( std::size_t c = 0; c < sizeof( Pixel ); ++c )
                        {
                                dest.at( i - y1, j - x1 ).values[ c ] = this->fields_.at( c ).range( i, j, i, j );
                        }
                }
        }
}

template< typename Pixel >
void pixel_field< Pixel >::_blur_image ( image< Pixel > & dest, std::size_t const blur_radius ) const noexcept
{
        _blur_image( dest, blur_radius, 0, 0, this->width_ - 1, this->height_ - 1 );
}

template< typename Pixel >
void pixel_field< Pixel >::_blur_image ( image< Pixel > & dest, std::size_t const blur_radius, std::size_t const x1, std::size_t const y1, std::size_t const x2, std::size_t const y2 ) const noexcept
{
        std::size_t current_blur_radius = blur_radius;

        for( std::size_t i = y1; i <= y2; ++i )
        {
                for( std::size_t j = x1; j <= x2; ++j )
                {
                        current_blur_radius = std::min( { blur_radius, i, j, y2 - i, x2 - j } );

                        for( std::size_t c = 0; c < sizeof( Pixel ); ++c )
                        {
                                auto avg = this->fields_.at( c ).range( i - current_blur_radius / 2, j - current_blur_radius / 2, i + current_blur_radius / 2, j + current_blur_radius / 2 );

                                avg /= current_blur_radius * current_blur_radius;

                                dest.at( i - y1, j - x1 ).values[ c ] = avg;
                        }
                }
        }
}


namespace detail
{


template< typename Pixel >
constexpr void _scale_naive_impl ( image< Pixel > const & src, image< Pixel > & dest, float scale_x, float scale_y )
{
        for( std::size_t i = 0; i < dest.height(); ++i )
        {
                for( std::size_t j = 0; j < dest.width(); ++j )
                {
                        dest.at( i, j ) = src.at( i / scale_y, j / scale_x );
                }
        }
}

template< typename Pixel >
constexpr void _scale_nearest_neighbor_impl ( image< Pixel > const & src, image< Pixel > & dest, float scale_x, float scale_y )
{
        for( std::size_t i = 0; i < dest.height(); ++i )
        {
                for( std::size_t j = 0; j < dest.width(); ++j )
                {
                        std::size_t x0 = std::floor( j / scale_x );
                        std::size_t y0 = std::floor( i / scale_y );
                        std::size_t x1 = std::ceil ( j / scale_x );
                        std::size_t y1 = std::ceil ( i / scale_y );

                        std::size_t x = std::round( j / scale_x ) > x0 ? x1 : x0;
                        std::size_t y = std::round( i / scale_y ) > y0 ? y1 : y0;

                        x = x >= src.width()  ? src.width()  - 1 : x;
                        y = y >= src.height() ? src.height() - 1 : y;

                        dest.at( i, j ) = src.at( y, x );
                }
        }
}

template< typename Pixel >
constexpr void _scale_bilinear_impl ( image< Pixel > const & src, image< Pixel > & dest, float scale_x, float scale_y )
{
        for( std::size_t i = 0; i < dest.height(); ++i )
        {
                for( std::size_t j = 0; j < dest.width(); ++j )
                {
                        float row = i / scale_y;
                        float col = j / scale_x;

                        std::size_t x0 = std::floor( col );
                        std::size_t y0 = std::floor( row );

                        std::size_t x1 = std::ceil( col );
                        std::size_t y1 = std::ceil( row );

                        float bias_x = col - x0;
                        float bias_y = row - y0;

                        for( std::size_t c = 0; c < dest.channels(); ++c )
                        {
                                float y0_val = src.at( y0, x0 ).values[ c ] * ( 1 - bias_x ) + src.at( y0, x1 ).values[ c ] * bias_x;
                                float y1_val = src.at( y1, x0 ).values[ c ] * ( 1 - bias_x ) + src.at( y1, x1 ).values[ c ] * bias_x;

                                float val = y0_val * ( 1 - bias_y ) + y1_val * bias_y;

                                dest.at( i, j ).values[ c ] = val;
                        }
                }
        }
}

template< typename Pixel >
constexpr void _scale_bicubic_impl ( [[ maybe_unused ]] image< Pixel > const & src, [[ maybe_unused ]] image< Pixel > & dest, [[ maybe_unused ]] float scale_x, [[ maybe_unused ]] float scale_y )
{

}

template< typename Pixel >
constexpr void _scale_image_impl ( image< Pixel > const & src, image< Pixel > & dest, scaling_options const & options )
{
        if( options.scale_x < 1 && options.scale_y < 1 )
        {
                _scale_naive_impl( src, dest, options.scale_x, options.scale_y );
        }

        switch( options.type )
        {
                case scaling_options::interpolation_type::naive:
                        _scale_naive_impl( src, dest, options.scale_x, options.scale_y );
                        break;

                case scaling_options::interpolation_type::nearest_neighbor:
                        _scale_nearest_neighbor_impl( src, dest, options.scale_x, options.scale_y );
                        break;

                case scaling_options::interpolation_type::bilinear:
                        _scale_bilinear_impl( src, dest, options.scale_x, options.scale_y );
                        break;

                case scaling_options::interpolation_type::bicubic:
                        _scale_bicubic_impl( src, dest, options.scale_x, options.scale_y );
                        break;

                default:
                        break;
        }
}


} // namespace detail


template< typename Pixel >
constexpr image< Pixel > scale_image ( image< Pixel > const & img, scaling_options const & options )
{
        image< Pixel > scaled( img.width() * options.scale_x, img.height() * options.scale_y );

        detail::_scale_image_impl( img, scaled, options );

        return scaled;
}

template< typename Pixel >
constexpr image< Pixel > scale_image ( image< Pixel > const & img, float const scale_x, float const scale_y, scaling_options::interpolation_type type )
{
        image< Pixel > scaled( img.width() * scale_x, img.height() * scale_y );

        detail::_scale_image_impl( img, scaled, { scale_x, scale_y, type } );

        return scaled;
}

template< typename Pixel >
constexpr image< Pixel > scale_image ( image< Pixel > const & img, float const scale, scaling_options::interpolation_type type )
{
        image< Pixel > scaled( img.width() * scale, img.height() * scale );

        detail::_scale_image_impl( img, scaled, { scale, scale, type } );

        return scaled;
}

/*
template< typename Pixel >
constexpr image< Pixel > pixelize_image_naive ( image< Pixel > const & img, std::size_t const pixel_size )
{

}

template< typename Pixel >
constexpr image< Pixel > blur_image_naive ( image< Pixel > const & img, std::size_t const blur_radius )
{

}

template< typename Pixel >
constexpr image< Pixel > get_brightest_rect_naive ( image< Pixel > const & img, std::size_t const rect_width, std::size_t const rect_height )
{

}
*/


} // namespace haze































