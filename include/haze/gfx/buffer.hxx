//
//
//      haze
//      gfx/buffer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/gfx/buffer_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/buffer.hxx>
namespace haze::_detail { using buffer_impl = ::haze::mtl::buffer ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/buffer.hxx>
namespace haze::_detail { using buffer_impl = ::haze::gl::buffer ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/buffer.hxx>
namespace haze::_detail { using buffer_impl = ::haze::ray::buffer ; }

#else
#       error "haze::gfx: no buffer implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using buffer = _detail::buffer_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
