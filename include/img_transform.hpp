//
//
//      HAZElib
//      img_transform.hpp
//

#pragma once

#include <array>

#include "image.hpp"


namespace haze
{


template< std::size_t SizeX, std::size_t SizeY >
struct kernel
{
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

template< std::size_t Low, std::size_t High >
struct kernel_group
{
        /*

           list of kernels of same "type" (eg. gaussian blurs)
           with sizes ranging from Low to High

           how tho?

        */
};

//constexpr static kernel< 1, 1 > identity_1{ { { 1 } }, 1 };

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

constexpr static kernel< 5, 5 > gauss_5{ { {  1,  4,  6,  4,  1 },
                                           {  4, 16, 24, 16,  4 },
                                           {  6, 24, 36, 24,  6 },
                                           {  4, 16, 24, 16,  4 },
                                           {  1,  4,  6,  4,  1 } }, 25 };


template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, kernel< KernSize, KernSize > const & kern );


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


} // namespace detail


template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, kernel< KernSize, KernSize > const & kern )
{
        image< Pixel > dest( src.width() - KernSize, src.height() - KernSize );

        detail::_transform_image_impl( src, dest, kern );

        return dest;
}


} // namespace haze
