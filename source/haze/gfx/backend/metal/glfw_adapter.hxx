//
//
//      haze
//      source/gfx/backend/metal/glfw_adapter.hxx
//

#pragma once

#include <uti/core/util/config.hxx>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

UTI_DIAGS_PUSH()
UTI_DIAGS_DISABLE( -Wunused-parameter )
UTI_DIAGS_DISABLE( -Wgnu-anonymous-struct )

#include <AppKit/AppKit.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

UTI_DIAGS_POP()


namespace haze::mtl::adapter
{


NS::Window * get_ns_window ( GLFWwindow* _glfw_window_, CA::MetalLayer * _layer_ ) ;

void set_ns_window_layer ( NS::Window * _window_, CA::MetalLayer * _layer_ ) ;


} // namespace haze::mtl
