//
//
//      HAZElib
//      gbench_haze.hpp
//

#pragma once

#include <benchmark/benchmark.h>

#include "../include/HAZElib.hpp"

#define IMG_PATH "lake.jpeg"


namespace haze_bench
{


template< typename Pixel >
auto pb
{
        []( haze::image< Pixel > const & lhs, haze::image< Pixel > const & rhs )
        {
                return ( lhs.width() * lhs.height() > rhs.width() * rhs.height() ) ? lhs : rhs;
        }
};

/*
template< typename Pixel >
static void bm_segtree_img ( benchmark::State & state )
{
        srand( time( nullptr ) );

        haze::image< Pixel > img( IMG_PATH );

        for( auto _ : state )
        {
                npl::rq::segment_tree< haze::image< Pixel >, pb< Pixel > > segtree;

                for( long long i = 0; i < state.range( 0 ); ++i )
                {
                        std::size_t x1 = rand() %  img.width() / 2;
                        std::size_t x2 = rand() %  img.width() / 2 + img.width() / 2;
                        std::size_t y1 = rand() % img.height() / 2;
                        std::size_t y2 = rand() % img.height() / 2 + img.height() / 2;

                        segtree.emplace_back( img.get_subimage( x1, y1, x2, y2 ) );
                }

                auto res = segtree.range();

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( res );
        }
}
*/

template< typename Pixel >
static void bm_mean_blur_field ( benchmark::State & state )
{
        haze::pixel_field< Pixel > field( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = field.get_blurred_image( state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }

        state.counters[    "iwidth" ] = field.width();
        state.counters[   "iheight" ] = field.height();
        state.counters[ "total_pix" ] = field.width() * field.height();
}

template< typename Pixel >
static void bm_mean_blur_field_metal ( benchmark::State & state )
{
        haze::gpu_ops< Pixel > gpu;

        haze::pixel_field< Pixel > field( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = gpu.mean_blur( field, state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }

        state.counters[    "iwidth" ] = field.width();
        state.counters[   "iheight" ] = field.height();
        state.counters[ "total_pix" ] = field.width() * field.height();
}

template< typename Pixel >
static void bm_mean_blur_kern ( benchmark::State & state )
{
        haze::image< Pixel > img( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = haze::mean_blur_image( img, { static_cast< std::size_t >( state.range( 0 ) ) } );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }
}

template< typename Pixel >
static void bm_mean_blur_kern_metal ( benchmark::State & state )
{
        haze::gpu_ops< Pixel > gpu;

        haze::image< Pixel > img( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = gpu.mean_blur( img, state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }

        state.counters[    "iwidth" ] = img.width();
        state.counters[   "iheight" ] = img.height();
        state.counters[ "total_pix" ] = img.width() * img.height();
}

template< typename Pixel >
static void bm_lens_blur_field ( benchmark::State & state )
{
        haze::pixel_field< Pixel > field( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = field.get_lens_blurred_image( state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }

        state.counters[    "iwidth" ] = field.width();
        state.counters[   "iheight" ] = field.height();
        state.counters[ "total_pix" ] = field.width() * field.height();
}

template< typename Pixel >
static void bm_lens_blur_field_metal ( benchmark::State & state )
{
        haze::gpu_ops< Pixel > gpu;

        haze::pixel_field< Pixel > field( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = gpu.lens_blur( field, state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }

        state.counters[    "iwidth" ] = field.width();
        state.counters[   "iheight" ] = field.height();
        state.counters[ "total_pix" ] = field.width() * field.height();
}

template< typename Pixel >
static void bm_lens_blur_kern_metal ( benchmark::State & state )
{
        haze::gpu_ops< Pixel > gpu;

        haze::image< Pixel > img( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = gpu.lens_blur( img, state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }

        state.counters[    "iwidth" ] = img.width();
        state.counters[   "iheight" ] = img.height();
        state.counters[ "total_pix" ] = img.width() * img.height();
}


} // namespace haze_bench
