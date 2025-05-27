//
//
//      haze
//      demo.cxx
//

#include <haze/core/common/types.hxx>
#include <haze/core/common/range.hxx>
#include <haze/core/common/time.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/line.hxx>
#include <haze/core/geometry/rectangle.hxx>
#include <haze/core/geometry/triangle.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>
#include <haze/core/object/shape.hxx>
#include <haze/core/object/image.hxx>
#include <haze/core/object/image_io.hxx>

#include <haze/cpugfx/draw/meta.hxx>
#include <haze/cpugfx/draw/line.hxx>
#include <haze/cpugfx/draw/fill.hxx>
#include <haze/cpugfx/draw/blender.hxx>
#include <haze/cpugfx/draw/interpolator.hxx>

#include <haze/core/scene/meta.hxx>
#include <haze/core/scene/layer.hxx>

#include <haze/cpugfx/retro/win95.hxx>

#include <haze/gfx/backend/metal/render.hxx>
#include <haze/gfx/backend/metal/window.hxx>

#include <thread>

// #include <cstdio>
// #include <ctime>

// #include <raylib.h>

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


int main ()
{
        NS::AutoreleasePool * pool = NS::AutoreleasePool::alloc()->init() ;

        haze::mtl::app_delegate app_dlgt ;

        std::thread(
        [ & ]
        {
                std::this_thread::sleep_for( std::chrono::milliseconds( 1500 ) ) ;

                haze::filled_triangle_3df< haze::rgba_pixel > filled_tri{ { { 0.0, 0.9, 0.0 }, { 0.8, 0.0, 0.0 }, { -0.9, -0.9, 0.0 } }, { 255, 8, 8, 128 } } ;

                app_dlgt.view_dlgt()->renderer()->triangles().push_back( filled_tri ) ;
        }
        ).detach() ;

        NS::Application * shared_app = NS::Application::sharedApplication() ;

        shared_app->setDelegate( &app_dlgt ) ;
        shared_app->run() ;

        pool->release() ;


        return 0 ;
}

/*
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
*/
