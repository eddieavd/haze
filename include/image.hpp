//
//
//      HAZElib
//      image.hpp
//

#pragma once


#include <string>

#include "util.hpp"
#include "stb_util.hpp"


namespace haze
{


template< typename T, std::size_t Channels >
struct pixel
{
        using value_type = T;

        constexpr static bool is_thicc{ sizeof( T ) > 1 };
        constexpr static std::size_t channels{ Channels };

        value_type values[ channels ];

        constexpr bool operator== ( pixel< value_type, channels > const & other ) const noexcept
        {
                for( std::size_t c = 0; c < channels; ++c )
                {
                        if( values[ c ] != other.values[ c ] )
                        {
                                return false;
                        }
                }
                return true;
        }

        constexpr float luminance () const noexcept
        {
                if constexpr( channels == 1 )
                {
                        return static_cast< float >( values[ 0 ] );
                }
                else
                {
                        return 0.2126 * values[ 0 ] + 0.7152 * values[ 1 ] + 0.0722 * values[ 2 ];

                                        //      https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
                }
        }
};

struct    bw_pixel : pixel< std::uint8_t, 1 > {};
struct   rgb_pixel : pixel< std::uint8_t, 3 > {};
struct alpha_pixel : pixel< std::uint8_t, 4 > {};

struct    thicc_bw_pixel : pixel< unsigned long, 1 > {};
struct   thicc_rgb_pixel : pixel< unsigned long, 3 > {};
struct thicc_alpha_pixel : pixel< unsigned long, 4 > {};


template< typename Pixel >
class image
{
public:
        using pixel_type = Pixel;

        constexpr image () noexcept = default;

        image ( std::size_t const width, std::size_t const height )
                : data_  ( new Pixel[ width * height ] ),
                  width_ (  width ),
                  height_( height )
        {}

        image ( Pixel * data, std::size_t const width, std::size_t const height )
                : data_  (   data ),
                  width_ (  width ),
                  height_( height )
        {}

        image ( std::string const & filename )
        {
                data_ = haze::util::load_image< Pixel >( filename, &width_, &height_ );
        }

        image ( image< Pixel > const & other )
                : data_  ( new Pixel[ other.width() * other.height() ] ),
                  width_ ( other.width() ),
                  height_( other.height() )
        {
                for( std::size_t i = 0; i < height_; ++i )
                {
                        for( std::size_t j = 0; j < width_; ++j )
                        {
                                at( i, j ) = other.at( i, j );
                        }
                }
        }

        image ( image< Pixel > && other )
                : data_  ( other.data_   ),
                  width_ ( other.width_  ),
                  height_( other.height_ )
        {
                other.data_  = nullptr;
                other.width_ = other.height_ = 0;
        }

        auto & operator= ( image< Pixel > const & other )
        {
                if( data_ )
                {
                        delete[] data_;
                }

                data_   = new Pixel[ other.width() * other.height() ];
                width_  = other.width();
                height_ = other.height();

                for( std::size_t i = 0; i < height_; ++i )
                {
                        for( std::size_t j = 0; j < width_; ++j )
                        {
                                at( i, j ) = other.at( i, j );
                        }
                }

                return *this;
        }

        auto & operator= ( image< Pixel > && other )
        {
                if( data_ )
                {
                        delete[] data_;
                }

                data_   = other.data_;
                width_  = other.width_;
                height_ = other.height_;

                other.data_  = nullptr;
                other.width_ = other.height_ = 0;

                return *this;
        }

        [[ nodiscard ]] constexpr auto     data ()       noexcept { return this->data_    ; }
        [[ nodiscard ]] constexpr auto     data () const noexcept { return this->data_    ; }
        [[ nodiscard ]] constexpr auto    width () const noexcept { return this->width_   ; }
        [[ nodiscard ]] constexpr auto   height () const noexcept { return this->height_  ; }
        [[ nodiscard ]] constexpr auto channels () const noexcept { return Pixel::channels; }

        [[ nodiscard ]] constexpr bool    empty () const noexcept { return this->data_ == nullptr; }

        [[ nodiscard ]] constexpr bool operator== ( image< Pixel > const & other ) const noexcept
        {
                if( width_ != other.width_ || height_ != other.height_ )
                {
                        return false;
                }
                for( std::size_t i = 0; i < height_; ++i )
                {
                        for( std::size_t j = 0; j < width_; ++j )
                        {
                                if( at( i, j ) != other.at( i, j ) )
                                {
                                        return false;
                                }
                        }
                }
                return true;
        }

        [[ nodiscard ]] constexpr auto & at ( std::size_t const row, std::size_t const col ) noexcept
        {
                HAZE_ASSERT( row < height() && col < width(), "image::at: out of bounds" );

                return data_[ row * this->width_ + col ];
        }

        [[ nodiscard ]] constexpr auto const & at ( std::size_t const row, std::size_t const col ) const noexcept
        {
                HAZE_ASSERT( row < height() && col < width(), "image::at: out of bounds" );

                return data_[ row * this->width_ + col ];
        }

        [[ nodiscard ]] image< Pixel > get_subimage ( std::size_t const x1, std::size_t const y1, std::size_t const x2, std::size_t const y2 ) const
        {
                HAZE_ASSERT( x1 <= x2 && y1 <= y2 && x2 < width() && y2 < height(), "image::get_subimage: out of range" );

                image< Pixel > sub( x2 - x1 + 1, y2 - y1 + 1 );

                for( std::size_t i = y1; i <= y2; ++i )
                {
                        for( std::size_t j = x1; j <= x2; ++j )
                        {
                                sub.at( i - y1, j - x1 ) = at( i, j );
                        }
                }
                return sub;
        }

        constexpr void set_subimage ( image< Pixel > const & sub, std::size_t const x0, std::size_t const y0 ) noexcept
        {
                HAZE_ASSERT( x0 < width() && y0 < height(), "image::set_subimage: out of bounds" );

                for( int i = y0; i < sub.height() && i < height_; ++i )
                {
                        for( int j = x0; j < sub.width() && j < width_; ++j )
                        {
                                at( i, j ) = sub.at( i - y0, j - x0 );
                        }
                }
        }


        ~image ()
        {
                if( data_ )
                {
                        delete[] data_;
                }
        }
private:
        Pixel       * data_   { nullptr };
        std::size_t   width_  {       0 };
        std::size_t   height_ {       0 };
};


} // namespace haze
