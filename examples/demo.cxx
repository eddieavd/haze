//
//
//      haze
//      demo.cxx
//

#include <haze/core/common/types.hxx>
#include <haze/core/common/range.hxx>
#include <haze/core/common/time.hxx>

#include <haze/core/common/log.hxx>

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

#include <haze/cpugfx/draw/interpolator.hxx>

#include <haze/gfx/context.hxx>
#include <haze/gfx/buffer.hxx>

#include <haze/app/window.hxx>
#include <haze/app/app.hxx>

#include <raylib.h>


using pixel_type = haze::rgba_u8_pixel ;
using   triangle = haze::filled_shape< pixel_type, haze::generic_triangle< haze::fpoint_3d > > ;

[[ maybe_unused ]]
static constexpr triangle tri =
{
        {
                {  0.000f, +1.0f, 0.0f } ,
                { -0.866f, -0.5f, 0.0f } ,
                { +0.866f, -0.5f, 0.0f } ,
        } ,
        pixel_type{ 255, 18, 18, 255 } ,
} ;

[[ maybe_unused ]]
static constexpr auto on_update =
        []( haze::layer & app_layer )
        {
                if( app_layer.objects().empty() ) app_layer.add_object( tri ) ;
        } ;

int main ( int argc, char ** argv )
{
/*
        InitWindow( 800, 800, "haze::ray" ) ;
        SetTargetFPS( 60 ) ;

        while( !WindowShouldClose() )
        {
                BeginDrawing() ;
                ClearBackground( BLACK ) ;
                EndDrawing() ;
        }
        CloseWindow() ;
*/
        haze::log::init( argc, argv ) ;

        HAZE_INFO( "main : initializing app..." ) ;

        haze::app app ;

        app.get_window().set_size( 800, 800 ) ;
        app.get_window().set_title( "haze::demo" ) ;

        app.on_update( on_update ) ;

        HAZE_INFO( "main : launching app..." ) ;

        app.run() ;

        HAZE_INFO( "main : done" ) ;


        return 0 ;
}
























