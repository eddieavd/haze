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
#include <haze/gfx/texture.hxx>

#include <haze/app/window.hxx>
#include <haze/app/app.hxx>


using texture_type = haze::texture ;

using point_type = typename texture_type::point_type ;
using pixel_type = typename texture_type::pixel_type ;

using image_type = haze::generic_image< pixel_type, point_type > ;


using triangle = haze::filled_shape< pixel_type, haze::generic_triangle< point_type > > ;

int main ( int argc, char ** argv )
{
        haze::log::init( argc, argv ) ;

        static constexpr triangle tri_1 =
        {
                {
                        { -1.0f, +1.0f, 0.0f },
                        {  0.0f, -1.0f, 0.0f },
                        { +1.0f, +1.0f, 0.0f }
                }, 
                pixel_type{ 255, 18, 18, 255 }
        } ;

        HAZE_INFO( "main : creating app..." ) ;

        haze::app app ;

        auto on_update = [ & ]( haze::layer & app_layer )
        {
                if( app_layer.objects().empty() ) app_layer.add_object( tri_1 ) ;

                triangle & tri = app_layer.get_object< triangle >( 0 ) ;

                --tri.fill()[ pixel_type:: RED ] ;
        } ;

        HAZE_INFO( "main : setting window size to 800x800" ) ;
        app.get_window().set_size( 800, 800 ) ;
        HAZE_INFO( "main : setting window title to haze::app" ) ;
        app.get_window().set_title( "haze::app" ) ;

        HAZE_INFO( "main : setting on_update handler..." ) ;

        app.on_update( on_update ) ;

        HAZE_INFO( "main : launching app..." ) ;

        app.run() ;


        return 0 ;
}
