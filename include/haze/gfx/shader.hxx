//
//
//      haze
//      gfx/shader.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/gfx/shader_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/shader.hxx>
namespace haze::_detail { using shader_impl = ::haze::mtl::shader ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/shader.hxx>
namespace haze::_detail { using shader_impl = ::haze::gl::shader ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/shader.hxx>
namespace haze::_detail { using shader_impl = ::haze::ray::shader ; }

#else
#       error "haze::gfx: no shader implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using shader = _detail::shader_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
