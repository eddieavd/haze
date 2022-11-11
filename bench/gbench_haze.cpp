//
//
//      HAZElib
//      gbench_haze.cpp
//

#include "gbench_haze.hpp"


namespace haze_bench
{


//BENCHMARK( bm_mean_blur_kern       < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,  16 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_mean_blur_kern_metal < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_mean_blur_field      < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_mean_blur_field_metal< haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );

//BENCHMARK( bm_lens_blur_kern_metal < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_lens_blur_field      < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,   4 )->Unit( benchmark::kMillisecond );
BENCHMARK( bm_lens_blur_field_metal< haze::bw_pixel > )->RangeMultiplier( 3 )->Range( 9, 127 )->Unit( benchmark::kMillisecond );



} // namespace haze_bench
