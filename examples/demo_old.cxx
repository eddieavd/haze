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


using pixel_t = haze::rgb_pixel ;
using image_t = haze::image< pixel_t > ;

using  rect_t = haze::rectangle ;
using point_t = typename rect_t::point_type ;

using circle_t = haze::generic_circle< typename point_t::value_type > ;

using  line_t = haze::           line ;
using hline_t = haze::horizontal_line ;
using vline_t = haze::  vertical_line ;

constexpr Image as_ray_image ( haze::image< haze::rgba_pixel > & _image_ ) noexcept
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

[[ maybe_unused ]] constexpr auto compare_red   = []( auto const & lhs, auto const & rhs ){ return lhs.channels[ lhs.RED ] < rhs.channels[ rhs.RED ] ; } ;

int main ()
{
        using animator_t = haze::line_animator< image_t > ;

        using p_animator_t = haze::parallel_animator< animator_t, animator_t > ;

        haze::string export_path( "anim.png" ) ;

        printf( "haze : starting...\n" ) ;

        image_t image( pixel_t{ 255, 255, 255 }, 200, 200 ) ;

        printf( "haze : image created, resolution is %ldx%ld\n", image.width(), image.height() ) ;

        haze::line_animator animator1( image ) ;
        haze::line_animator animator2( image ) ;

        printf( "haze : animators created\n" ) ;

        haze::animation_params params = { .frame_rate = 30, .length = haze::milliseconds( 500 ) } ;

        haze::line line1{ { 16, 16 }, { image.width() - 16,                  16 } } ;
        haze::line line2{ { 16, 16 }, {                 16, image.height() - 16 } } ;

        printf( "haze : created line { %ldx%ld } -> { %ldx%ld }, len %ld\n", line1.begin_.x(), line1.begin_.y(), line1.end_.x(), line1.end_.y(), line1.length() ) ;
        printf( "haze : created line { %ldx%ld } -> { %ldx%ld }, len %ld\n", line2.begin_.x(), line2.begin_.y(), line2.end_.x(), line2.end_.y(), line2.length() ) ;

        animator1.set_animation( line1, pixel_t{ 255, 0, 0 }, params ) ;
        animator2.set_animation( line2, pixel_t{ 255, 0, 0 }, params ) ;

        p_animator_t panim( UTI_MOVE( animator1 ), UTI_MOVE( animator2 ) ) ;

        printf( "haze : animators initialized\n" ) ;
        printf( "haze : starting animation generation...\n" ) ;

        uti::vector< image_t > frames ;

        while( !panim.finished() )
        {
                printf( "haze : generating next frame...\n" ) ;
                panim.generate_next_frame() ;
/*
                animator1.generate_next_frame() ;
                animator2.set_frame( UTI_FWD( animator1.release() ) ) ;
                animator2.generate_next_frame() ;
                animator1.set_frame( UTI_FWD( animator2.release() ) ) ;
*/
                printf( "haze : frame generated\n" ) ;

                printf( "haze : storing frame...\n" ) ;
                frames.push_back( panim.cframe() ) ;
                printf( "haze : frame stored\n" ) ;
        }

        haze::line line3{ {                 16, image.height() - 16 }, { image.width() - 16, image.height() - 16 } } ;
        haze::line line4{ { image.width() - 16,                  16 }, { image.width() - 16, image.height() - 16 } } ;

        printf( "haze : created line { %ldx%ld } -> { %ldx%ld }, len %ld\n", line3.begin_.x(), line3.begin_.y(), line3.end_.x(), line3.end_.y(), line3.length() ) ;
        printf( "haze : created line { %ldx%ld } -> { %ldx%ld }, len %ld\n", line4.begin_.x(), line4.begin_.y(), line4.end_.x(), line4.end_.y(), line4.length() ) ;

        panim.get< 0 >().set_animation( line3, pixel_t{ 255, 0, 0 }, params ) ;
        panim.get< 1 >().set_animation( line4, pixel_t{ 255, 0, 0 }, params ) ;

        printf( "haze : animators initialized\n" ) ;
        printf( "haze : starting animation generation...\n" ) ;

        while( !panim.finished() )
        {
                printf( "haze : generating next frame...\n" ) ;
                panim.generate_next_frame() ;
                printf( "haze : frame generated\n" ) ;

                printf( "haze : storing frame...\n" ) ;
                frames.push_back( panim.cframe() ) ;
                printf( "haze : frame stored\n" ) ;
        }

        InitWindow( image.width(), image.height(), "haze demo" ) ;
        SetTargetFPS( params.frame_rate ) ;

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
/*
        static constexpr int fps { 30 } ;
//      static constexpr int step { 8 } ;

        image_t image( pixel_t{ 0, 0, 0 }, 800, 800 ) ;

//      point_t start{  8, image.height() / 2 } ;
//      point_t   end{ 16, image.height() / 2 } ;

//      hline_t line{ start.col(), start.row(), end.row() } ;

        InitWindow( image.width(), image.height(), "haze demo" ) ;
        SetTargetFPS( fps ) ;

        Texture2D texture( LoadTextureFromImage( as_ray_image( image ) ) ) ;

        while( !WindowShouldClose() )
        {
//              {
//                      haze::draw_shape( image, line, pixel_t{ 255, 0, 0 } ) ;
//                      start = end ;
//                      end.coords[ 0 ] += step ;
//              }
//              UnloadTexture( texture ) ;
//              texture = LoadTextureFromImage( as_ray_image( image ) ) ;

                BeginDrawing() ;
                ClearBackground( WHITE ) ;
                DrawTexture( texture, 0, 0, WHITE ) ;
                EndDrawing() ;
        }
        UnloadTexture( texture ) ;
        CloseWindow() ;
*/
/*
        static constexpr int fps { 30 } ;

        haze::string export_name( "anim.png" ) ;

//      image_t image( pixel_t{ 255, 255, 255, 255 }, 800, 800 ) ;
        auto source_image = haze::load_image< pixel_t >( "rotpic.png" ) ;

        printf( "haze : loaded image with resolution: %ldx%ld\n", source_image.width(), source_image.height() ) ;

        auto image = haze::downscale_linear( source_image, 8 ) ;

        printf( "haze : scaled image resolution: %ldx%ld\n", image.width(), image.height() ) ;

        haze::line_animator animator( UTI_MOVE( image ) ) ;

        [[ maybe_unused ]] hline_t line0{ animator.cframe().height() / 2 + 0, 8, animator.cframe().width() - 8 } ;
        [[ maybe_unused ]] hline_t line1{ animator.cframe().height() / 2 + 1, 8, animator.cframe().width() - 8 } ;
        [[ maybe_unused ]] hline_t line2{ animator.cframe().height() / 2 + 2, 8, animator.cframe().width() - 8 } ;
        [[ maybe_unused ]] hline_t line3{ animator.cframe().height() / 2 + 3, 8, animator.cframe().width() - 8 } ;

        animator.animate( line0, fps, haze::duration( haze::seconds( 1 ) ) ) ;

        printf( "haze : animator initialized\n" ) ;

        InitWindow( animator.cframe().width(), animator.cframe().height(), "haze demo" ) ;
        SetTargetFPS( fps ) ;

        printf( "haze : window initialized\n" ) ;

        image_t frame( animator.cframe() ) ;
        Texture2D texture = LoadTextureFromImage( as_ray_image( frame ) ) ;

        printf( "haze : texture loaded\n" ) ;

        while( !WindowShouldClose() )
        {
//              animator.generate_frame() ;
//              image_t frame( animator.cframe() ) ;
//              UnloadTexture( texture ) ;
//              texture = LoadTextureFromImage( as_ray_image( frame ) ) ;

                BeginDrawing() ;
                DrawTexture( texture, 0, 0, WHITE ) ;
                EndDrawing() ;
        }
        UnloadTexture( texture ) ;
        CloseWindow() ;
*/


        return 0 ;
}
