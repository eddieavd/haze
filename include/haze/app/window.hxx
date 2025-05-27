//
//
//      haze
//      app/window.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/app/window_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/window.hxx>
namespace haze::_detail { using window_impl = ::haze::mtl::window ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/window.hxx>
namespace haze::_detail { using window_impl = ::haze::gl::window ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/window.hxx>
namespace haze::_detail { using window_impl = ::haze::ray::window ; }

#else
#       error "haze::gfx: no window implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using window = _detail::window_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
