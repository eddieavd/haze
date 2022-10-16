//
//
//      HAZElib
//      gbench_haze.cpp
//

#include "gbench_haze.hpp"


namespace haze_bench
{


BENCHMARK( bm_blur_cpu_prefix< haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 2, 128 )->Unit( benchmark::kMillisecond );
BENCHMARK( bm_blur_gpu_kern  < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 2, 128 )->Unit( benchmark::kMillisecond );

BENCHMARK( bm_blur_cpu_prefix< haze::rgb_pixel > )->RangeMultiplier( 2 )->Range( 2, 128 )->Unit( benchmark::kMillisecond );
BENCHMARK( bm_blur_gpu_kern  < haze::rgb_pixel > )->RangeMultiplier( 2 )->Range( 2, 128 )->Unit( benchmark::kMillisecond );


} // namespace haze_bench
