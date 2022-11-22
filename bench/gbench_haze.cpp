//
//
//      HAZElib
//      gbench_haze.cpp
//

#include "gbench_haze.hpp"


namespace haze_bench
{



//BENCHMARK( bm_transform_image_cpu    < haze::bw_pixel, 5, haze::    mean_blur_5 > )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_transform_image_sep_cpu< haze::bw_pixel, 5, haze::sep_mean_blur_5 > )->Unit( benchmark::kMillisecond );

//BENCHMARK( bm_mean_blur_kern       < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,  32 )->Unit( benchmark::kMillisecond );
BENCHMARK( bm_mean_blur_kern_metal < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_mean_blur_sprbl      < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,  64 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_mean_blur_sprbl_metal< haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,  64 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_mean_blur_field      < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );
BENCHMARK( bm_mean_blur_field_metal< haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );

//BENCHMARK( bm_lens_blur_kern_metal        < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,  64 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_lens_blur_field             < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4,  16 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_lens_blur_field_metal       < haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );
//BENCHMARK( bm_lens_blur_field_direct_metal< haze::bw_pixel > )->RangeMultiplier( 2 )->Range( 4, 128 )->Unit( benchmark::kMillisecond );


} // namespace haze_bench
