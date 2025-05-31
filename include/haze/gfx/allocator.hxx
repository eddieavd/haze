//
//
//      haze
//      gfx/allocator.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/allocator_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/allocator.hxx>
namespace haze::_detail { using gpu_memory_resource_impl = ::haze::mtl::gpu_memory_resource ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/allocator.hxx>
namespace haze::_detail { using gpu_memory_resource_impl = ::haze::gl::opengl_memory_resource ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/allocator.hxx>
namespace haze::_detail { using gpu_memory_resource_impl = ::haze::ray::raylib_memory_resource ; }

#else
#       error "haze::gfx: no allocator implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using gpu_memory_resource = _detail::gpu_memory_resource_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
