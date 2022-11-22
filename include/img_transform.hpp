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


constexpr static kernel< 1, 1 > identity_1{ { { 1 } }, 1 };

constexpr static kernel< 3, 3 > identity_3{ { { 0, 0, 0 },
                                              { 0, 1, 0 },
                                              { 0, 0, 0 } }, 1 };

constexpr static kernel< 3, 3 > sobel_h{ { { -1, -2, -1 },
                                           {  0,  0,  0 },
                                           {  1,  2,  1 } }, 4 };

constexpr static kernel< 3, 3 > sobel_v{ { { -1,  0,  1 },
                                           { -2,  0,  2 },
                                           { -1,  0,  1 } }, 4 };

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

constexpr static kernel< 3, 3 > sharpen{ { {  0, -1,  0 },
                                           { -1,  5, -1 },
                                           {  0, -1,  0 } }, 1 };

constexpr static separable_kernel< 3 > sep_mean_blur_3{ { 1, 1, 1 },
                                                        { 1, 1, 1 }, 3, 3 };

constexpr static separable_kernel< 5 > sep_mean_blur_5{ { 1, 1, 1, 1, 1 },
                                                        { 1, 1, 1, 1, 1 }, 5, 5 };


template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, kernel< KernSize, KernSize > const & kern );

template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, separable_kernel< KernSize > const & sep_kern );

template< typename Pixel >
constexpr image< Pixel > mean_blur_image ( image< Pixel > const & src, std::size_t const blur_radius );

template< typename Pixel >
constexpr image< Pixel > mean_blur_image_sep ( image< Pixel > const & src, std::size_t const blur_radius );


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

template< typename Pixel, std::size_t KernSize >
constexpr void _transform_image_impl ( image< Pixel > const & src, image< Pixel > & dest, separable_kernel< KernSize > const & sep_kern )
{
        image< Pixel > tmp( src.width(), dest.height() );

        for( std::size_t i = KernSize / 2; i < src.height() - KernSize / 2 - 1; ++i )
        {
                for( std::size_t j = 0; j < src.width(); ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < KernSize; ++ki )
                                {
                                        val += src.at( i + ki - KernSize / 2, j ).values[ c ] * sep_kern.vertical[ ki ];
                                }

                                val /= sep_kern.weight_v;
                                val = std::abs( val );

                                tmp.at( i - KernSize / 2, j ).values[ c ] = static_cast< unsigned char >( val );
                        }
                }
        }

        for( std::size_t i = 0; i < tmp.height(); ++i )
        {
                for( std::size_t j = KernSize / 2; j < tmp.width() - KernSize / 2 - 1; ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < KernSize; ++ki )
                                {
                                        val += tmp.at( i, j + ki - KernSize / 2 ).values[ c ] * sep_kern.horizontal[ ki ];
                                }

                                val /= sep_kern.weight_h;
                                val = std::abs( val );

                                dest.at( i, j - KernSize / 2 ).values[ c ] = static_cast< unsigned char >( val );
                        }
                }
        }
}

template< typename Pixel >
constexpr void _mean_blur_image_impl ( image< Pixel > const & src, image< Pixel > & dest, std::size_t const blur_radius )
{
        for( std::size_t i = blur_radius / 2; i < src.height() - blur_radius / 2 - 1; ++i )
        {
                for( std::size_t j = blur_radius / 2; j < src.width() - blur_radius / 2 - 1; ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < blur_radius; ++ki )
                                {
                                        for( std::size_t kj = 0; kj < blur_radius; ++kj )
                                        {
                                                val += src.at( i + ki - blur_radius / 2, j + kj - blur_radius / 2  ).values[ c ];
                                        }
                                }

                                val /= blur_radius * blur_radius;
                                val = std::abs( val );

                                dest.at( i - blur_radius / 2, j - blur_radius / 2 ).values[ c ] = static_cast< unsigned char >( val );
                        }
                }
        }
}

template< typename Pixel >
constexpr void _mean_blur_image_sep_impl ( image< Pixel > const & src, image< Pixel > & dest, std::size_t const blur_radius )
{
        image< Pixel > tmp( src.width(), dest.height() );

        for( std::size_t i = blur_radius / 2; i < src.height() - blur_radius / 2 - 1; ++i )
        {
                for( std::size_t j = 0; j < src.width(); ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < blur_radius; ++ki )
                                {
                                        val += src.at( i + ki - blur_radius / 2, j  ).values[ c ];
                                }

                                val /= blur_radius;
                                val = std::abs( val );

                                tmp.at( i - blur_radius / 2, j ).values[ c ] = static_cast< unsigned char >( val );
                        }
                }
        }

        for( std::size_t i = 0; i < dest.height(); ++i )
        {
                for( std::size_t j = blur_radius / 2; j < tmp.width() - blur_radius / 2 - 1; ++j )
                {
                        for( std::size_t c = 0; c < Pixel::channels; ++c )
                        {
                                double val = 0;

                                for( std::size_t ki = 0; ki < blur_radius; ++ki )
                                {
                                        val += tmp.at( i, j + ki - blur_radius / 2 ).values[ c ];
                                }

                                val /= blur_radius;
                                val = std::abs( val );

                                dest.at( i, j - blur_radius / 2 ).values[ c ] = static_cast< unsigned char >( val );
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

template< typename Pixel, std::size_t KernSize >
constexpr image< Pixel > transform_image ( image< Pixel > const & src, separable_kernel< KernSize > const & sep_kern )
{
        image< Pixel > dest( src.width() - KernSize, src.height() - KernSize );

        detail::_transform_image_impl( src, dest, sep_kern );

        return dest;
}

template< typename Pixel >
constexpr image< Pixel > mean_blur_image ( image< Pixel > const & src, std::size_t const blur_radius )
{
        image< Pixel > dest( src.width() - blur_radius, src.height() - blur_radius );

        detail::_mean_blur_image_impl( src, dest, blur_radius );

        return dest;
}

template< typename Pixel >
constexpr image< Pixel > mean_blur_image_sep ( image< Pixel > const & src, std::size_t const blur_radius )
{
        image< Pixel > dest( src.width() - blur_radius, src.height() - blur_radius );

        detail::_mean_blur_image_sep_impl( src, dest, blur_radius );

        return dest;
}


} // namespace haze
