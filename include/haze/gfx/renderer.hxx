//
//
//      haze
//      gfx/renderer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/renderer_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/renderer.hxx>
namespace haze::_detail { using renderer_impl = ::haze::mtl::renderer ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/renderer.hxx>
namespace haze::_detail { using renderer_impl = ::haze::gl::renderer ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/renderer.hxx>
namespace haze::_detail { using renderer_impl = ::haze::ray::renderer ; }

#else
#       error "haze::gfx: no renderer implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using renderer = _detail::renderer_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
