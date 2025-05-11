//
//
//      haze
//      demo.cxx
//

#include <haze/common/types.hxx>
#include <haze/common/range.hxx>
#include <haze/common/time.hxx>

#include <haze/geo/meta.hxx>
#include <haze/geo/point.hxx>
#include <haze/geo/line.hxx>
#include <haze/geo/rectangle.hxx>
#include <haze/geo/circle.hxx>

#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>
#include <haze/image/io.hxx>

#include <haze/filter/downscale.hxx>
#include <haze/filter/transform.hxx>
#include <haze/filter/sort.hxx>

#include <haze/render/draw.hxx>
#include <haze/render/interpolator.hxx>
#include <haze/render/line_animator.hxx>
#include <haze/render/parallel_animator.hxx>
#include <haze/render/sequential_animator.hxx>

#include <raylib.h>

 /////////////////////////////////////
/// raysan please prefix your shit ///
///                                ///
#ifdef RED                         ///
#undef RED                         ///
#endif                             ///
#ifdef GREEN                       ///
#undef GREEN                       ///
#endif                             ///
#ifdef BLUE                        ///
#undef BLUE                        ///
#endif                             ///
///                                ///
/////////////////////////////////////


template< haze::meta::pixel_like PixelType >
constexpr Image as_ray_image ( haze::image< PixelType > & _image_ ) noexcept ;

[[ maybe_unused ]] constexpr auto compare_red = []( auto const & lhs, auto const & rhs ){ return lhs.channels[ lhs.RED ] < rhs.channels[ rhs.RED ] ; } ;

using pixel_t = haze::rgba_pixel ;
using image_t = haze::image< pixel_t > ;

using line_anim_t = haze::line_animator< haze::linear_interpolator, image_t > ;

using parallel_anim_t = haze::parallel_animator< line_anim_t, line_anim_t > ;

using rect_anim_t = haze::parallel_animator< line_anim_t, line_anim_t, line_anim_t, line_anim_t > ;

using seq_anim_t = haze::sequential_animator< parallel_anim_t, parallel_anim_t > ;

using animator_t = haze::parallel_animator< seq_anim_t, line_anim_t > ;

int main ()
{
        static constexpr int fps { 120 } ;

        image_t image( pixel_t{ 0, 0, 0, 255 }, 512, 512 ) ;

        haze::rectangle rect{ { 32, 32 }, { 512 - 32, 512 - 32 } } ;
        haze::circle circ{ { 256, 256 }, 128 } ;

        haze::fill_shape( image, rect, pixel_t{ 255, 255,   0, 32 } ) ;
        haze::fill_shape( image, circ, pixel_t{   0,   0, 255, 32 } ) ;

        [[ maybe_unused ]] haze::line line1{ {                 8,                  8 }, {                 8, image.height() - 8 } } ;
        [[ maybe_unused ]] haze::line line2{ {                 8,                  8 }, { image.width() - 8,                  8 } } ;
        [[ maybe_unused ]] haze::line line3{ {                 8, image.height() - 8 }, { image.width() - 8, image.height() - 8 } } ;
        [[ maybe_unused ]] haze::line line4{ { image.width() - 8,                  8 }, { image.width() - 8, image.height() - 8 } } ;

        [[ maybe_unused ]] haze::line line5{ { 8, 8 }, { image.width() - 8, image.height() - 8 } } ;

        haze::animation_params params{ fps, haze::milliseconds( 500 ) } ;

        animator_t animator ;

        seq_anim_t & seq_anim_1 = animator.get< 0 >() ;
        line_anim_t & line_anim = animator.get< 1 >() ;

        line_anim.set_animation( line5, pixel_t{ 255, 0, 0, 255 }, { fps, params.length * 2 } ) ;

        parallel_anim_t & p_anim_1 = seq_anim_1.get< 0 >() ;
        parallel_anim_t & p_anim_2 = seq_anim_1.get< 1 >() ;

        line_anim_t & line_anim_1 = p_anim_1.get< 0 >() ;
        line_anim_t & line_anim_2 = p_anim_1.get< 1 >() ;
        line_anim_t & line_anim_3 = p_anim_2.get< 0 >() ;
        line_anim_t & line_anim_4 = p_anim_2.get< 1 >() ;

        line_anim_1.set_animation( line1, pixel_t{ 255, 0, 0, 255 }, params ) ;
        line_anim_2.set_animation( line2, pixel_t{ 255, 0, 0, 255 }, params ) ;
        line_anim_3.set_animation( line3, pixel_t{ 255, 0, 0, 255 }, params ) ;
        line_anim_4.set_animation( line4, pixel_t{ 255, 0, 0, 255 }, params ) ;

        haze::vector< image_t > frames( animator.expected_frames() ) ;

        printf( "haze : animation generator initialized\n" ) ;
        printf( "haze : expecting %ld frames\n", frames.capacity() ) ;
        printf( "haze : starting animation...\n" ) ;

        frames.push_back( image ) ;

        animator.set_frame( image ) ;

        while( !animator.finished() )
        {
                animator.generate_next_frame() ;

                frames.push_back( animator.cframe() ) ;
        }
        printf( "haze : generated %ld frames\n", frames.size() - 1 ) ;

        InitWindow( image.width(), image.height(), "haze demo" ) ;
        SetTargetFPS( fps ) ;

        int idx {} ;

        while( !WindowShouldClose() )
        {
                if( idx >= frames.size() ) idx = 0 ;

                auto & frame = frames.at( idx++ ) ;
                Texture2D texture = LoadTextureFromImage( as_ray_image( frame ) ) ;

                ClearBackground( BLACK ) ;

                BeginDrawing() ;
                DrawTexture( texture, 0, 0, WHITE ) ;
                EndDrawing() ;

                UnloadTexture( texture ) ;
        }
        CloseWindow() ;


        return 0 ;
}

template< haze::meta::pixel_like PixelType >
constexpr Image as_ray_image ( haze::image< PixelType > & _image_ ) noexcept
{
        if constexpr( uti::meta::same_as< PixelType, haze::rgb_pixel > )
        {
                return Image
                {
                        _image_.pixels().data() ,
                        static_cast< int >( _image_. width() ) ,
                        static_cast< int >( _image_.height() ) ,
                        1 ,
                        PIXELFORMAT_UNCOMPRESSED_R8G8B8 ,
                } ;
        }
        if constexpr( uti::meta::same_as< PixelType, haze::rgba_pixel > )
        {
                return Image
                {
                        _image_.pixels().data() ,
                        static_cast< int >( _image_. width() ) ,
                        static_cast< int >( _image_.height() ) ,
                        1 ,
                        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 ,
                } ;
        }
}
