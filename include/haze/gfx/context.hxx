//
//
//      haze
//      gfx/context.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/context_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/context.hxx>
namespace haze::_detail { using context_impl = ::haze::mtl::context ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/context.hxx>
namespace haze::_detail { using context_impl = ::haze::gl::context ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/context.hxx>
namespace haze::_detail { using context_impl = ::haze::ray::context ; }

#else
#       error "haze::gfx: no context implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using context = _detail::context_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
