//
//
//      haze
//      demo.cxx
//

#include <haze/common/types.hxx>
#include <haze/common/range.hxx>
#include <haze/common/time.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/line.hxx>
#include <haze/geometry/rectangle.hxx>

#include <haze/object/meta.hxx>
#include <haze/object/pixel.hxx>
#include <haze/object/shape.hxx>
#include <haze/object/image.hxx>
#include <haze/object/image_io.hxx>

#include <cstdio>

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


using pixel_t = haze::rgba_pixel ;
using image_t = haze::generic_image< pixel_t, haze::point_2d > ;

constexpr Image as_ray_image ( image_t & _image_ ) noexcept ;

int main ()
{
        static constexpr int fps { 60 } ;

//      auto image = haze::load_image< pixel_t >( "rotpic.png" ) ;

        image_t image( pixel_t{ 18, 18, 18, 255 }, 64, 64 ) ;

        haze::rectangle_2d rect{ { 32, 32 }, { 63, 63 } } ;

        haze::filled_rectangle_2d filled_rect{ rect, pixel_t{ 255, 18, 18, 128 } } ;

        auto img_view = image.subview( rect ) ;

        auto img_iter = img_view.row_begin() ;

        InitWindow( image.width(), image.height(), "haze demo" ) ;
        SetTargetFPS( fps ) ;

        while( !WindowShouldClose() )
        {
                if( img_iter < img_view.row_end() )
                {
                        * img_iter = filled_rect.at( img_iter.point() ) ;
                        ++img_iter ;
                }
                Texture2D texture( LoadTextureFromImage( as_ray_image( image ) ) ) ;

                ClearBackground( BLACK ) ;
                BeginDrawing() ;
                DrawTexture( texture, 0, 0, WHITE ) ;
                EndDrawing() ;

                UnloadTexture( texture ) ;
        }
        CloseWindow() ;


        return 0 ;
}

constexpr Image as_ray_image ( image_t & _image_ ) noexcept
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
