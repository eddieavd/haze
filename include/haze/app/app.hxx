//
//
//      haze
//      app/app.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/app/app_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/app.hxx>
namespace haze::_detail { using app_impl = ::haze::mtl::app ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/app.hxx>
namespace haze::_detail { using app_impl = ::haze::gl::app ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/app.hxx>
namespace haze::_detail { using app_impl = ::haze::ray::app ; }

#else
#       error "haze::gfx: no app implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using app = _detail::app_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
