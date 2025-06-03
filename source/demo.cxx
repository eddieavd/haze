//
//
//      haze
//      source/demo.cxx
//

#include <haze/core/common/log.hxx>

#include <haze/gfx/context.hxx>
#include <haze/app/window.hxx>
#include <haze/app/app.hxx>


////////////////////////////////////////////////////////////////////////////////
/// this can be constexpr in c++26 for short titles (up to and including 23 chars)
haze::window_options window_opts =
{
        800, 600,
        haze::string( "haze::demo" )
} ;

int main ( int argc, char ** argv )
{
        haze::log::init( argc, argv ) ;

        int status {} ;

        HAZE_INFO( "main : started" ) ;
        {
                HAZE_INFO( "main : initializing haze app..." ) ;
                haze::app app ;

                HAZE_INFO( "main : creating windows..." ) ;

                window_opts. width = 200 ;
                window_opts.height = 200 ;

                app.create_window( window_opts ).clear_color() = { 255,  18,  18, 255 } ;
                app.create_window( window_opts ).clear_color() = {  18, 255,  18, 255 } ;
                app.create_window( window_opts ).clear_color() = {  18,  18, 255, 255 } ;
                app.create_window( window_opts ).clear_color() = { 128, 128, 128, 255 } ;

                HAZE_INFO( "main : starting haze app..." ) ;
                status = app.run( argc, argv ) ;
        }
        HAZE_INFO( "main : exiting with status code { %d }", status ) ;


        return status ;
}
