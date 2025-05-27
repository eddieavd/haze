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
#include <haze/gfx/texture.hxx>

#include <haze/app/window.hxx>
#include <haze/app/app.hxx>



using texture_type = haze::texture ;

using point_type = typename texture_type::point_type ;
using pixel_type = typename texture_type::pixel_type ;

using image_type = haze::generic_image< pixel_type, point_type > ;

int main ( int argc, char ** argv )
{
        haze::log::init( argc, argv ) ;

        auto image = haze::load_image< image_type >( "image.png" ) ;
        HAZE_INFO( "loaded image %.fx%.f", image.width(), image.height() ) ;

        haze::context ctx ;
        HAZE_INFO( "created context" ) ;

        haze::texture_spec spec =
        {
                .    width = static_cast< ssize_t >( image. width() ) ,
                .   height = static_cast< ssize_t >( image.height() ) ,
                .mipmapped = false
        } ;
        HAZE_INFO( "created texture spec" ) ;

        haze::texture txt = ctx.create_texture< pixel_type, point_type >( spec ) ;
        HAZE_INFO( "created texture %.fx%.f", txt.width(), txt.height() ) ;

        txt.upload( image ) ;
        HAZE_INFO( "uploaded image to texture" ) ;

        auto processed = txt.download() ;
        HAZE_INFO( "downloaded image from texture" ) ;

        haze::store_image( "processed.png", processed ) ;
        HAZE_INFO( "stored image to disk" ) ;


        return 0 ;
}
