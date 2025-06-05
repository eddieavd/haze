//
//
//      haze
//      gfx/renderer_api.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/renderer_api_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/renderer_api.hxx>
namespace haze::_detail { using renderer_api_impl = ::haze::mtl::renderer_api ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/renderer_api.hxx>
namespace haze::_detail { using renderer_api_impl = ::haze::gl::renderer_api ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/renderer_api.hxx>
namespace haze::_detail { using renderer_api_impl = ::haze::ray::renderer_api ; }

#else
#       error "haze::gfx: no renderer_api implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using renderer_api = _detail::renderer_api_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
