//
//
//      haze
//      gfx/vertex_array.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/vertex_array_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/vertex_array.hxx>
namespace haze::_detail { using vertex_array_impl = ::haze::mtl::vertex_array ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/vertex_array.hxx>
namespace haze::_detail { using vertex_array_impl = ::haze::gl::vertex_array ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/vertex_array.hxx>
namespace haze::_detail { using vertex_array_impl = ::haze::ray::vertex_array ; }

#else
#       error "haze::gfx: no vertex_array implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using vertex_array = _detail::vertex_array_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
