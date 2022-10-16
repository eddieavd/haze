//
//
//      HAZElib
//      gbench_haze.hpp
//

#pragma once

#include <benchmark/benchmark.h>

#include "../include/HAZElib.hpp"

#define IMG_PATH "city2.jpeg"


namespace haze_bench
{


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

/*
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
*/

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

/*
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
*/


} // haze_bench
