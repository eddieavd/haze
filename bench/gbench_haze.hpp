//
//
//      HAZElib
//      gbench_haze.hpp
//

#pragma once

#include <benchmark/benchmark.h>

#include "../include/HAZElib.hpp"

#define IMG_PATH "image.png"


namespace haze_bench
{


template< typename Pixel >
static void bm_blur_cpu_kern ( [[ maybe_unused ]] benchmark::State & state )
{

}

template< typename Pixel >
static void bm_blur_cpu_prefix ( benchmark::State & state )
{
        haze::pixel_field< Pixel > field( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = field.get_blurred_image( state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }
}

template< typename Pixel >
static void bm_blur_gpu_kern ( benchmark::State & state )
{
        haze::gpu_ops< Pixel > gpu_ops;
        haze::image< Pixel > img( IMG_PATH );

        for( auto _ : state )
        {
                auto blurred = gpu_ops.blur_image( img, state.range( 0 ) );

                benchmark::ClobberMemory();
                benchmark::DoNotOptimize( blurred );
        }
}

template< typename Pixel >
static void bm_blur_gpu_prefix ( [[ maybe_unused ]] benchmark::State & state )
{

}


} // haze_bench
