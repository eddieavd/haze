//
//
//      haze
//      source/gfx/backend/metal/glfw_adapter.mm
//

#include "glfw_adapter.hxx"


namespace haze::mtl::adapter
{


NS::Window * get_ns_window ( GLFWwindow * _glfw_window_, CA::MetalLayer * _layer_ )
{
        CALayer  * objc_layer  = ( __bridge CALayer * ) _layer_ ;
        NSWindow * objc_window = glfwGetCocoaWindow( _glfw_window_ ) ;

        objc_window.contentView.layer = objc_layer ;
        objc_window.contentView.wantsLayer = YES ;

        return ( __bridge NS::Window * ) objc_window ;
}

void set_ns_window_layer ( NS::Window * _window_, CA::MetalLayer * _layer_ )
{
        CALayer  * objc_layer  = ( __bridge CALayer  * )  _layer_ ;
        NSWindow * objc_window = ( __bridge NSWindow * ) _window_ ;

        objc_window.contentView.layer = objc_layer ;
        objc_window.contentView.wantsLayer = YES ;
}


} // namespace haze::mtl::adapter
