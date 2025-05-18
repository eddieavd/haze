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
#include <haze/geometry/triangle.hxx>

#include <haze/object/meta.hxx>
#include <haze/object/pixel.hxx>
#include <haze/object/shape.hxx>
#include <haze/object/image.hxx>
#include <haze/object/image_io.hxx>

#include <haze/draw/meta.hxx>
#include <haze/draw/line.hxx>
#include <haze/draw/fill.hxx>
#include <haze/draw/blender.hxx>
#include <haze/draw/interpolator.hxx>

#include <haze/scene/meta.hxx>
#include <haze/scene/layer.hxx>

#include <haze/retro/win95.hxx>

#include <cstdio>
#include <ctime>

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
using point_t = haze::  point_2d ;

using image_t = haze::generic_image< pixel_t, haze::point_2d > ;

constexpr Image as_ray_image ( image_t & _image_ ) noexcept ;
constexpr void rotate_line ( auto & line, auto const & image ) ;

constexpr image_t create_window ( int width, int height ) noexcept ;

constexpr haze::filled_rectangle_2d< pixel_t > rand_rect ( int width, int height ) noexcept ;
constexpr haze::filled_triangle_2d < pixel_t > rand_tri  ( int width, int height ) noexcept ;
int main ()
{
        srand( time( nullptr ) ) ;

        static constexpr int    fps { 120 } ;
        static constexpr int  width { 800 } ;
        static constexpr int height { 600 } ;

//      haze::fast_layer< pixel_t, point_t, image_t > layer( { { 0, 0 }, { width, height } }, 1.0 ) ;

        InitWindow( width, height, "haze demo" ) ;
        SetTargetFPS( fps ) ;

        int frame_count { 120 } ;
//      int  rect_count {} ;
//      int   tri_count {} ;

        haze::vector< haze::filled_rectangle_2d< pixel_t > > rects ;
        haze::vector< haze::filled_triangle_2d < pixel_t > >  tris ;

        while( !WindowShouldClose() )
        {
                if( frame_count <= 0 )
                {
                        rects.push_back( rand_rect( width, height ) ) ;
                        tris .push_back( rand_tri ( width, height ) ) ;
                        frame_count = 120 ;
                }
                --frame_count ;
/*
                auto frame = layer.render() ;

                if( layer.opacity() >= 1.0 ) layer.opacity()  = 0.00 ;
                else                         layer.opacity() += 0.01 ;

                --frame_count ;
                if( frame_count <= 0 )
                {
                        layer.add_object( rand_rect( width, height ) ) ;
                        layer.add_object( rand_tri ( width, height ) ) ;
                        frame_count = 120 ;
                        ++rect_count ;
                        ++ tri_count ;
                }

                Texture2D texture = LoadTextureFromImage( as_ray_image( frame ) ) ;
*/
                ClearBackground( BLACK ) ;
                BeginDrawing() ;
//              DrawTexture( texture, 0, 0, WHITE ) ;
                for( auto & rect : rects )
                {
                        DrawRectangle( rect.shape().begin_x(), rect.shape().begin_y(), rect.shape().width(), rect.shape().height(), *( Color * ) &rect.fill() ) ;

                        ++rect.fill()[ pixel_t::ALPHA ] ;
                }
                for( auto & tri : tris )
                {
                        DrawTriangle( Vector2{ static_cast< float >( tri.shape().a_.x() ), static_cast< float >( tri.shape().a_.y() ) },
                                      Vector2{ static_cast< float >( tri.shape().b_.x() ), static_cast< float >( tri.shape().b_.y() ) },
                                      Vector2{ static_cast< float >( tri.shape().c_.x() ), static_cast< float >( tri.shape().c_.y() ) },
                                        *( Color * ) &tri.fill() ) ;
                        ++tri.fill()[ pixel_t::ALPHA ] ;
                }

                DrawText( TextFormat( "FPS: %d", GetFPS() ), 32, 32, 16, Color{ 0, 255, 0, 255 } ) ;
                DrawText( TextFormat( "Frame time : %.3fms", 1000.0 / GetFPS() ), 32, 64, 16, Color{ 0, 255, 0, 255 } ) ;
                DrawText( TextFormat( "Rectangles : %d", rects.size() ), 32, 96, 16, Color{ 0, 255, 0, 255 } ) ;
                DrawText( TextFormat( "Triangles  : %d",  tris.size() ), 32, 96 + 32, 16, Color{ 0, 255, 0, 255 } ) ;
                EndDrawing() ;

//              UnloadTexture( texture ) ;
        }
        CloseWindow() ;


        return 0 ;
}

constexpr image_t create_window ( int width, int height ) noexcept
{
        pixel_t color_background { 128, 128, 128, 255 } ;
        pixel_t color_border     {  64,  64,  64, 255 } ;
        pixel_t color_dark       {   8,   8,   8, 255 } ;
        pixel_t color_bright     { 245, 245, 235, 255 } ;
        pixel_t color_accent     {  16,  16,  48, 255 } ;

        haze::layer< pixel_t, point_t > layer( haze::rectangle_2d{ {}, { width, height } } ) ;

        haze::filled_rectangle_2d border_top  { { { 0, 0 }, { width - 1,          4 } }, color_border } ;
        haze::filled_rectangle_2d border_left { { { 0, 0 }, {         4, height - 1 } }, color_border } ;

        haze::filled_rectangle_2d border_right{ { { width - 4,          0 }, { width - 1, height - 1 } }, color_border } ;
        haze::filled_rectangle_2d border_bot  { { {         0, height - 4 }, { width - 1, height - 1 } }, color_border } ;

        haze::filled_rectangle_2d shadow_right{ { { width - 2,          2 }, { width - 1, height - 1 } }, color_dark } ;
        haze::filled_rectangle_2d shadow_bot  { { {         2, height - 2 }, { width - 1, height - 1 } }, color_dark } ;

        haze::filled_rectangle_2d window{ { { 4, 4 }, { width - 5, height - 5 } }, color_background } ;

        haze::filled_rectangle_2d menu{ { { 8, 8 }, { width - 8 - 1, 16 } }, color_accent } ;

        haze::filled_rectangle_2d userspace{ { { 8, 20 }, { width - 8 - 1, height - 8 - 1 } }, color_bright } ;

        haze::filled_rectangle_2d btn_x{ { {}, {} }, color_background } ;

        layer.add_object( border_top ) ;
        layer.add_object( border_left ) ;
        layer.add_object( border_right ) ;
        layer.add_object( border_bot ) ;
        layer.add_object( shadow_right ) ;
        layer.add_object( shadow_bot ) ;
        layer.add_object( window ) ;
        layer.add_object( menu ) ;
        layer.add_object( userspace ) ;

        return layer.render() ;
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

constexpr void rotate_line ( auto & line, auto const & image )
{
        if( line.begin_point().y() == 8 && line.begin_point().x() < image.width() - 8 )
        {
                ++line.begin_point().x() ;
                --line.  end_point().x() ;
        }
        else if( line.begin_point().x() == image.width() - 8 && line.begin_point().y() < image.height() - 8 )
        {
                ++line.begin_point().y() ;
                --line.  end_point().y() ;
        }
        else if( line.begin_point().y() == image.height() - 8 && line.begin_point().x() > 8 )
        {
                --line.begin_point().x() ;
                ++line.  end_point().x() ;
        }
        else if( line.begin_point().x() == 8 && line.begin_point().y() > 8 )
        {
                --line.begin_point().y() ;
                ++line.  end_point().y() ;
        }
}

constexpr haze::filled_rectangle_2d< pixel_t > rand_rect ( int width, int height ) noexcept
{
        auto begin_x = rand() %  width ;
        auto begin_y = rand() % height ;

        auto end_x = begin_x + ( rand() % (  width - begin_x ) ) ;
        auto end_y = begin_y + ( rand() % ( height - begin_y ) ) ;

        haze::u8_t r = rand() % 255 ;
        haze::u8_t g = rand() % 255 ;
        haze::u8_t b = rand() % 255 ;

        return haze::filled_rectangle_2d{ { { begin_x, begin_y }, { end_x, end_y } }, pixel_t{ r, g, b, 255 } } ;
}

constexpr haze::filled_triangle_2d< pixel_t > rand_tri ( int width, int height ) noexcept
{
        haze::u8_t r = rand() % 255 ;
        haze::u8_t g = rand() % 255 ;
        haze::u8_t b = rand() % 255 ;

        return haze::filled_triangle_2d{ { { rand() % width, rand() % height }, { rand() % width, rand() % height }, { rand() % width, rand() % height } }, pixel_t{ r, g, b, 255 } } ;
}
