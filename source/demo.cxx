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
#include <haze/render/basic_animator.hxx>
#include <haze/render/parallel_animator.hxx>
#include <haze/render/sequential_animator.hxx>

#include <raylib.h>

#ifdef RED
#undef RED
#endif
#ifdef GREEN
#undef GREEN
#endif
#ifdef BLUE
#undef BLUE
#endif


constexpr Image as_ray_image ( haze::image< haze::rgb_pixel > & _image_ ) noexcept
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

[[ maybe_unused ]] constexpr auto compare_red = []( auto const & lhs, auto const & rhs ){ return lhs.channels[ lhs.RED ] < rhs.channels[ rhs.RED ] ; } ;

using pixel_t = haze::rgb_pixel ;
using image_t = haze::image< pixel_t > ;

using line_anim_t = haze::basic_line_animator< haze::linear_interpolator, image_t > ;

using parallel_anim_t = haze::parallel_animator< line_anim_t, line_anim_t > ;

using rect_anim_t = haze::parallel_animator< line_anim_t, line_anim_t, line_anim_t, line_anim_t > ;

using seq_anim_t = haze::sequential_animator< parallel_anim_t, parallel_anim_t > ;

int main ()
{
        static constexpr int fps { 30 } ;

        image_t image( pixel_t{ 0, 0, 0 }, 512, 512 ) ;

        [[ maybe_unused ]] haze::line line1{ {                 8,                  8 }, {                 8, image.height() - 8 } } ;
        [[ maybe_unused ]] haze::line line2{ {                 8,                  8 }, { image.width() - 8,                  8 } } ;
        [[ maybe_unused ]] haze::line line3{ {                 8, image.height() - 8 }, { image.width() - 8, image.height() - 8 } } ;
        [[ maybe_unused ]] haze::line line4{ { image.width() - 8,                  8 }, { image.width() - 8, image.height() - 8 } } ;

        haze::animation_params params{ fps, haze::milliseconds( 1000 ) } ;

        rect_anim_t animator( image ) ;

//      seq_anim_t animator( image ) ;

//      parallel_anim_t & p_anim_1 = animator.get< 0 >() ;
//      parallel_anim_t & p_anim_2 = animator.get< 1 >() ;

        line_anim_t & line_anim_1 = animator.get< 0 >() ;
        line_anim_t & line_anim_2 = animator.get< 1 >() ;
        line_anim_t & line_anim_3 = animator.get< 2 >() ;
        line_anim_t & line_anim_4 = animator.get< 3 >() ;

        line_anim_1.set_animation( line1, pixel_t{ 255, 0, 0 }, params ) ;
        line_anim_2.set_animation( line3, pixel_t{ 255, 0, 0 }, params ) ;
        line_anim_3.set_animation( line2, pixel_t{ 255, 0, 0 }, params ) ;
        line_anim_4.set_animation( line4, pixel_t{ 255, 0, 0 }, params ) ;

        haze::vector< image_t > frames( animator.expected_frames() ) ;

        printf( "haze : expecting %ld frames\n", frames.capacity() ) ;

        frames.push_back( image ) ;

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

                BeginDrawing() ;
                DrawTexture( texture, 0, 0, WHITE ) ;
                EndDrawing() ;

                UnloadTexture( texture ) ;
        }
        CloseWindow() ;
/*
        image_t image( pixel_t{ 0, 0, 0 }, 512, 512 ) ;

        [[ maybe_unused ]] haze::line line1{ {                 8,                  8 }, {                 8, image.height() - 8 } } ;
        [[ maybe_unused ]] haze::line line2{ {                 8,                  8 }, { image.width() - 8,                  8 } } ;
        [[ maybe_unused ]] haze::line line3{ {                 8, image.height() - 8 }, { image.width() - 8, image.height() - 8 } } ;
        [[ maybe_unused ]] haze::line line4{ { image.width() - 8,                  8 }, { image.width() - 8, image.height() - 8 } } ;

        [[ maybe_unused ]] haze::line line5{ {                 8,                  8 }, { image.width() / 2, image.height() / 2 } } ;
        [[ maybe_unused ]] haze::line line6{ { image.width() / 2, image.height() / 2 }, { image.width() - 8, image.height() - 8 } } ;

        [[ maybe_unused ]] haze::line linex{ {                 8, image.height() - 8 }, { image.width() - 8,                  8 } } ;

        haze::animation_params params{ 30, haze::milliseconds( 500 ) } ;

        pixel_t stroke{ 255, 0, 0 } ;

        anim_t animator( image ) ;

        seq_anim_t & seq_anim_1 = animator.get< 0 >() ;
        line_anim_t & line_anim_z = animator.get< 1 >() ;

        parallel_anim_t & p_anim_1 = seq_anim_1.get< 0 >() ;
        parallel_anim_t & p_anim_2 = seq_anim_1.get< 1 >() ;

        line_anim_t & line_anim_1 = p_anim_1.get< 0 >() ;
        line_anim_t & line_anim_2 = p_anim_1.get< 1 >() ;
        line_anim_t & line_anim_x = p_anim_1.get< 2 >() ;
        line_anim_t & line_anim_3 = p_anim_2.get< 0 >() ;
        line_anim_t & line_anim_4 = p_anim_2.get< 1 >() ;
        line_anim_t & line_anim_y = p_anim_2.get< 2 >() ;

        line_anim_1.set_animation( line1, stroke, params ) ;
        line_anim_2.set_animation( line2, stroke, params ) ;
        line_anim_x.set_animation( line5, stroke, params ) ;
        line_anim_3.set_animation( line3, stroke, params ) ;
        line_anim_4.set_animation( line4, stroke, params ) ;
        line_anim_y.set_animation( line6, stroke, params ) ;

        line_anim_z.set_animation( linex, stroke, { 30, haze::seconds( 1 ) } ) ;

        printf( "haze :   vertical line len is %ld\n", line1.length() ) ;
        printf( "haze : horizontal line len is %ld\n", line2.length() ) ;
        printf( "haze :   diagonal line len is %ld\n", line5.length() ) ;

//      auto frame = animator.get_frame_ref() ;

        haze::vector< image_t > frames( seq_anim_1.expected_frames() + 1 ) ;
        frames.push_back( image ) ;

        printf( "haze : expecting %ld frames\n", frames.capacity() ) ;

        while( !seq_anim_1.finished() )
        {
                seq_anim_1.generate_next_frame() ;

                frames.push_back( seq_anim_1.cframe() ) ;
        }
//      frames.erase_stable( frames.size() / 2 - 1 ) ;

        printf( "haze : generated %ld frames\n", frames.size() ) ;

        haze::string export_path( "anim.png" ) ;

        for( auto const & frame : frames )
        {
                haze::store_image( export_path, frame ) ;
                export_path.insert( 'x', 4 ) ;
        }

        InitWindow( image.width(), image.height(), "haze demo" ) ;
        SetTargetFPS( 30 ) ;

        int idx {} ;

        while( !WindowShouldClose() )
        {
                if( idx >= frames.size() ) idx = 0 ;

                auto & frame = frames.at( idx++ ) ;
                Texture2D texture( LoadTextureFromImage( as_ray_image( frame ) ) ) ;

                BeginDrawing() ;
                DrawTexture( texture, 0, 0, WHITE ) ;
                EndDrawing() ;

                UnloadTexture( texture ) ;
        }
        CloseWindow() ;
*/


        return 0 ;
}
