//
//
//      haze
//      gfx/backend/metal/app.cxx
//

#include <haze/gfx/backend/metal/app.hxx>

#include "glfw_adapter.hxx"


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

window::~window () noexcept
{
        HAZE_CORE_INFO_S( "window : dtor" ) ;

        if( glfw_window_ )
        {
                HAZE_CORE_INFO( "window::destroy : releasing glfw window..." ) ;
                glfwDestroyWindow( glfw_window_ ) ;
                glfw_window_ = nullptr ;
                HAZE_CORE_INFO( "window::destroy : glfw window released" ) ;
        }
}

////////////////////////////////////////////////////////////////////////////////

void window::_destroy () noexcept
{
        HAZE_CORE_INFO_S( "window::destroy" ) ;

        if( glfw_window_ )
        {
                HAZE_CORE_INFO( "window::destroy : releasing glfw window..." ) ;
                glfwDestroyWindow( glfw_window_ ) ;
                glfw_window_ = nullptr ;
                HAZE_CORE_INFO( "window::destroy : glfw window released" ) ;
        }
        _ca_base::_release() ;
        _ns_base::_release() ;
}

////////////////////////////////////////////////////////////////////////////////

app::app ()
        : renderer_api_( ctx_, default_renderer_api_config )
{
        HAZE_CORE_INFO_S( "app : ctor" ) ;
        HAZE_CORE_INFO( "app : initializing glfw..." ) ;
        glfwInit() ;
        HAZE_CORE_INFO( "app : glfw initialized" ) ;
}

////////////////////////////////////////////////////////////////////////////////

app::~app ()
{
        HAZE_CORE_INFO_S( "app : dtor" ) ;
        HAZE_CORE_INFO( "app : terminating glfw..." ) ;
        glfwTerminate() ;
        HAZE_CORE_INFO( "app : glfw terminated" ) ;
}

////////////////////////////////////////////////////////////////////////////////

i32_t app::_run ( i32_t _argc_, char ** _argv_ )
{
        HAZE_CORE_INFO_S( "app::run" ) ;

        if( windows_.empty() )
        {
                HAZE_CORE_ERROR( "app::run : no window available. use create_window() to create one" ) ;
                return 1 ;
        }

        HAZE_CORE_INFO( "app::run : initializing renderer..." ) ;
        renderer_api_.init() ;
        HAZE_CORE_INFO( "app::run : renderer initialized" ) ;

        HAZE_CORE_INFO( "app::run : starting main loop..." ) ;

        while( !windows_.empty() )
        {
                HAZE_CORE_YAP_S( "app::run : main loop" ) ;

                HAZE_CORE_YAP( "app::run : glfw polling events..." ) ;
                glfwPollEvents() ;
                HAZE_CORE_YAP( "app::run : glfw polling events done" ) ;

                for( window & w : windows_ )
                {
                        HAZE_CORE_YAP_S( "app::run : window " SV_FMT, SV_ARG( w.title() ) ) ;

                        if( w.closed() )
                        {
                                HAZE_CORE_YAP( "app::run : window closed" ) ;
                                continue ;
                        }
                        if( glfwWindowShouldClose( w ) )
                        {
                                HAZE_CORE_YAP( "app::run : window should close" ) ;
                                w.destroy() ;
                                continue ;
                        }
                        HAZE_CORE_YAP( "app::run : running window on_update..." ) ;
                        w.on_update() ;
                        HAZE_CORE_YAP( "app::run : on_update finished" ) ;

                        HAZE_CORE_YAP( "app::run : loading vertices into renderer..." ) ;
                        renderer_api_.load_vertices( w.layer() ) ;
                        HAZE_CORE_YAP( "app::run : issuing draw call..." ) ;
                        renderer_api_.draw( w ) ;
                }
                ssize_t swap_idx { windows_.size() - 1 } ;

                for( ssize_t i = windows_.size() - 1; i >= 1; --i )
                {
                        if( windows_.at( i ).closed() )
                        {
                                if( i != swap_idx )
                                {
                                        uti::swap( windows_.at( i ), windows_.at( swap_idx ) ) ;
                                }
                                --swap_idx ;
                        }
                }
                while( !windows_.empty() && windows_.back().closed() )
                {
                        windows_.pop_back() ;
                }
                ++frame_count ;

//              if( frame_count > 5 ) break ;
        }
        HAZE_CORE_INFO( "app::run : dropped out of main loop" ) ;

        return 0 ;
}

////////////////////////////////////////////////////////////////////////////////

window & app::_create_window ( window_options _opts_ )
{
        HAZE_CORE_INFO_S( "app::create_window" ) ;

        HAZE_CORE_INFO( "app::create_window :  width : { %ld }", _opts_. width ) ;
        HAZE_CORE_INFO( "app::create_window : height : { %ld }", _opts_.height ) ;
        HAZE_CORE_INFO( "app::create_window :  title : { " SV_FMT " }", SV_ARG( _opts_.title ) ) ;

        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API ) ;
        HAZE_CORE_INFO( "app::create_window : GLFW_CLIENT_API : GLFW_NO_API" ) ;
        glfwWindowHint( GLFW_RESIZABLE , GLFW_FALSE  ) ;
        HAZE_CORE_INFO( "app::create_window : GLFW_RESIZABLE  : GLFW_FALSE" ) ;

        GLFWwindow * glfw_window = glfwCreateWindow( _opts_.width, _opts_.height, _opts_.title.c_str(), nullptr, nullptr ) ;

        CA::MetalLayer * metal_layer = CA::MetalLayer::layer()->retain() ;

        metal_layer->setDevice( ctx_ ) ;
        metal_layer->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm ) ;

        NS::Window * ns_window = adapter::get_ns_window( glfw_window, metal_layer ) ;

        windows_.emplace_back( ns_window, metal_layer, glfw_window ) ;

        return windows_.back() ;
}

////////////////////////////////////////////////////////////////////////////////

double app::_target_fps () const noexcept
{
        HAZE_CORE_WARN( "app::target_fps : unimplemented" ) ;
        return 0 ;
}

////////////////////////////////////////////////////////////////////////////////

void app::_set_target_fps ( double )
{
        HAZE_CORE_WARN( "app::set_target_fps : unimplemented" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
