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
        HAZE_CORE_INFO( "app::run : starting main loop..." ) ;

        while( !windows_.empty() )
        {
                HAZE_CORE_YAP_S( "app::run : main loop" ) ;

                HAZE_CORE_YAP( "app::run : glfw polling events..." ) ;
                glfwPollEvents() ;
                HAZE_CORE_YAP( "app::run : glfw polling events done" ) ;

                HAZE_CORE_YAP( "app::run : creating command buffer..." ) ;
                MTL::CommandBuffer * command_buffer = static_cast< MTL::CommandQueue * >( ctx_ )->commandBuffer() ;
                if( !command_buffer ) { HAZE_CORE_ERROR( "app::run : failed creating command buffer" ) ; continue ; }
                HAZE_CORE_YAP( "app::run : command buffer created" ) ;

                for( auto & w : windows_ )
                {
                        if( w.closed() )
                        {
                                continue ;
                        }
                        if( glfwWindowShouldClose( w ) )
                        {
                                w.destroy() ;
                                continue ;
                        }
                        w.on_update() ;

                        NS::AutoreleasePool * pool = NS::AutoreleasePool::alloc()->init() ;

                        HAZE_CORE_YAP( "app::run : fetching next drawable..." ) ;
                        CA::MetalDrawable * drawable = static_cast< CA::MetalLayer * >( w )->nextDrawable() ;
                        if( !drawable ) { HAZE_CORE_ERROR( "app::run : failed fetching drawable" ) ; continue ; }
                        HAZE_CORE_YAP( "app::run : drawable fetched" ) ;

                        HAZE_CORE_YAP( "app::run : creating render pass descriptor..." ) ;
                        MTL::RenderPassDescriptor * render_pass_desc = MTL::RenderPassDescriptor::alloc()->init() ;
                        if( !render_pass_desc ) { HAZE_CORE_ERROR( "app::run : failed creating render pass descriptor" ) ; pool->release() ; continue ; }
                        MTL::RenderPassColorAttachmentDescriptor * color_attachment_desc = render_pass_desc->colorAttachments()->object( 0 ) ;
                        color_attachment_desc->setTexture( drawable->texture() ) ;
                        color_attachment_desc->setLoadAction( MTL::LoadActionClear ) ;
                        color_attachment_desc->setClearColor( MTL::ClearColor( w.clear_color()[ 0 ] / 255.0, w.clear_color()[ 1 ] / 255.0, w.clear_color()[ 2 ] / 255.0, w.clear_color()[ 3 ] / 255.0 ) ) ;
                        color_attachment_desc->setStoreAction( MTL::StoreActionStore ) ;
                        HAZE_CORE_YAP( "app::run : render pass descriptor created" ) ;

                        HAZE_CORE_YAP( "app::run : creating render command encoder..." ) ;
                        MTL::RenderCommandEncoder * encoder = command_buffer->renderCommandEncoder( render_pass_desc ) ;
                        if( !encoder ) { HAZE_CORE_ERROR( "app::run : failed creating render command encoder" ) ; continue ; }
                        HAZE_CORE_YAP( "app::run : render command encoder created" ) ;
                        encoder->endEncoding() ;
                        HAZE_CORE_YAP( "app::run : encoding finished" ) ;

                        HAZE_CORE_YAP( "app::run : asking system to present drawable..." ) ;
                        command_buffer->presentDrawable( drawable ) ;

                        pool->release() ;
                }
                HAZE_CORE_YAP( "app::run : committing GPU workload..." ) ;
                command_buffer->commit() ;
                HAZE_CORE_YAP( "app::run : workload committed" ) ;
//              command_buffer->waitUntilScheduled() ;
//              command_buffer->waitUntilCompleted() ;
                command_buffer->release() ;

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
