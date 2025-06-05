//
//
//      haze
//      source/demo.cxx
//

#include <haze/core/common/log.hxx>

#include <haze/app/app.hxx>


////////////////////////////////////////////////////////////////////////////////
/// this can be constexpr in c++26 for short titles (up to and including 23 chars)
haze::window_options window_opts =
{
        800, 600,
        haze::string( "haze::demo" )
} ;

using pixel_type = haze::rgba_f_pixel ;

using         triangle = haze::generic_triangle<   haze::fpoint_3d > ;
using  filled_triangle = haze:: filled_shape< pixel_type, triangle > ;
using colored_triangle = haze::colored_shape< pixel_type, triangle > ;

static constexpr pixel_type colors[ 3 ] =
{
        { 255 / 255.0,  18 / 255.0,  18 / 255.0, 255 / 255.0 } ,
        {  18 / 255.0, 255 / 255.0,  18 / 255.0, 255 / 255.0 } ,
        {  18 / 255.0,  18 / 255.0, 255 / 255.0, 255 / 255.0 } ,
} ;

static constexpr colored_triangle tri_central =
{
        {
                {  0.000f, +1.0f, 0.0f } ,
                { -0.866f, -0.5f, 0.0f } ,
                { +0.866f, -0.5f, 0.0f } ,
        },
        {
                colors[ 0 ] ,
                colors[ 1 ] ,
                colors[ 2 ] ,
        }
} ;

static constexpr colored_triangle tri_1
{
        {
                { -1.0f, -1.0f, 0.0f } ,
                { -1.0f,  1.0f, 0.0f } ,
                {  1.0f, -1.0f, 0.0f } ,
        },
        {
                colors[ 0 ] ,
                colors[ 1 ] ,
                colors[ 2 ] ,
        }
} ;

static constexpr colored_triangle tri_2
{
        {
                { -1.0f,  1.0f, 0.0f } ,
                {  1.0f, -1.0f, 0.0f } ,
                {  1.0f,  1.0f, 0.0f } ,
        },
        {
                colors[ 1 ] ,
                colors[ 2 ] ,
                colors[ 0 ] ,
        }
} ;


int main ( int argc, char ** argv )
{
        haze::log::init( argc, argv ) ;

        auto on_update_1 = [ & ]( haze::window & window )
        {
                if( window.layer().objects().empty() )
                {
                        window.layer().add_object( tri_1 ) ;
                        window.layer().add_object( tri_2 ) ;
                }
        } ;

        haze::app app ;

        app.create_window( window_opts )
           .set_on_update( on_update_1 ) ;

        app.run( argc, argv ) ;


        return 0 ;
}
