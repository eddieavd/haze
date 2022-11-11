//
//
//      HAZElib
//      img_transform.hpp
//

#pragma once

#include <array>
#include <vector>

#include "image.hpp"


namespace haze
{


template< std::size_t SizeX, std::size_t SizeY >
struct kernel
{
        constexpr static std::size_t size      = SizeX;
        constexpr static std::size_t sizey     = SizeY;
        constexpr static bool        is_square = SizeX == SizeY;

        float vals[ SizeX ][ SizeY ];
        float weight;
};

template< std::size_t Size >
struct separable_kernel
{
        float vertical   [ Size ];
        float horizontal [ Size ];

        float weight_v;
        float weight_h;
};

template< typename T >
struct dynamic_mean_blur
{
        using value_type = float;

        dynamic_mean_blur ( std::size_t const size ) : size_( size )
        {
                for( std::size_t i = 0; i < size; ++i )
                {
                        vals.emplace_back();

                        for( std::size_t j = 0; j < size; ++j )
                        {
                                vals[ i ].emplace_back( 1 );
                        }
                }
        }

        std::size_t size_;
        float       weight;
        std::vector< std::vector< float > > vals;
};

template< std::size_t Low, std::size_t High >
struct kernel_group
{
        /*

           list of kernels of same "type" (eg. gaussian blurs)
           with sizes ranging from Low to High

           how tho?

           without nontype template params idiot
           you're locking stuff down unnecessarily
        */
};

constexpr static kernel< 1, 1 > identity_1{ { { 1 } }, 1 };

constexpr static kernel< 3, 3 > identity_3{ { { 0, 0, 0 },
                                              { 0, 1, 0 },
                                              { 0, 0, 0 } }, 1 };

constexpr static kernel< 3, 3 > sobel_h{ { { -1, -2, -1 },
                                           {  0,  0,  0 },
                                           {  1,  2,  1 } }, 9 / 2.55 };

constexpr static kernel< 3, 3 > sobel_v{ { { -1,  0,  1 },
                                           { -2,  0,  2 },
                                           { -1,  0,  1 } }, 9 / 2.55 };

constexpr static kernel< 3, 3 > mean_blur_3{ { { 1, 1, 1 },
                                               { 1, 1, 1 },
                                               { 1, 1, 1 } }, 9 };

constexpr static kernel< 5, 5 > mean_blur_5{ { { 1, 1, 1, 1, 1 },
                                               { 1, 1, 1, 1, 1 },
                                               { 1, 1, 1, 1, 1 },
                                               { 1, 1, 1, 1, 1 },
                                               { 1, 1, 1, 1, 1 } }, 25 };

constexpr static kernel< 3, 3 > gauss_3{ { { 1, 2, 1 },
                                           { 2, 4, 2 },
                                           { 1, 2, 1 } }, 16 };


template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, kernel< KernSize, KernSize > const & kern );

template< typename Pixel >
constexpr image< Pixel > mean_blur_image ( image< Pixel > const & src, dynamic_mean_blur< Pixel > const & blur );


namespace detail
{


template< typename Pixel, std::size_t KernSize >
constexpr void _transform_image_impl ( image< Pixel > const & src, image< Pixel > & dest, kernel< KernSize, KernSize > const & kern )
{
        for( std::size_t i = KernSize / 2; i < src.height() - KernSize / 2 - 1; ++i )
        {
                for( std::size_t j = KernSize / 2; j < src.width() - KernSize / 2 - 1; ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < KernSize; ++ki )
                                {
                                        for( std::size_t kj = 0; kj < KernSize; ++kj )
                                        {
                                                val += src.at( i + ki - KernSize / 2, j + kj - KernSize / 2  ).values[ c ] * kern.vals[ ki ][ kj ];
                                        }
                                }

                                val /= kern.weight;
                                val = std::abs( val );

                                dest.at( i - KernSize / 2, j - KernSize / 2 ).values[ c ] = static_cast< unsigned char >( val );
                        }
                }
        }
}

template< typename Pixel >
constexpr void _mean_blur_image_impl ( image< Pixel > const & src, image< Pixel > & dest, dynamic_mean_blur< Pixel > const & blur )
{
        for( std::size_t i = blur.size_ / 2; i < src.height() - blur.size_ / 2 - 1; ++i )
        {
                for( std::size_t j = blur.size_ / 2; j < src.width() - blur.size_ / 2 - 1; ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < blur.size_; ++ki )
                                {
                                        for( std::size_t kj = 0; kj < blur.size_; ++kj )
                                        {
                                                val += src.at( i + ki - blur.size_ / 2, j + kj - blur.size_ / 2  ).values[ c ] * blur.vals[ ki ][ kj ];
                                        }
                                }

                                val /= blur.weight;
                                val = std::abs( val );

                                dest.at( i - blur.size_ / 2, j - blur.size_ / 2 ).values[ c ] = static_cast< unsigned char >( val );
                        }
                }
        }
}


} // namespace detail


template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, kernel< KernSize, KernSize > const & kern )
{
        image< Pixel > dest( src.width() - KernSize, src.height() - KernSize );

        detail::_transform_image_impl( src, dest, kern );

        return dest;
}

template< typename Pixel >
constexpr image< Pixel > mean_blur_image ( image< Pixel > const & src, dynamic_mean_blur< Pixel > const & blur )
{
        image< Pixel > dest( src.width() - blur.size_, src.height() - blur.size_ );

        detail::_mean_blur_image_impl( src, dest, blur );

        return dest;
}


} // namespace haze
