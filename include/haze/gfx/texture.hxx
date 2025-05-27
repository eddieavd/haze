//
//
//      haze
//      gfx/texture.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/gfx/texture_base.hxx>

#if defined( HAZE_USE_METAL )

#include <haze/gfx/backend/metal/texture.hxx>
namespace haze::_detail { template< meta::pixel_like PixelType, meta::point_like PointType > using texture_impl = ::haze::mtl::generic_texture< PixelType, PointType > ; }

#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/texture.hxx>
namespace haze::_detail { using texture_impl = ::haze::gl::texture ; }

#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/texture.hxx>
namespace haze::_detail { using texture_impl = ::haze::ray::texture ; }

#else
#       error "haze::gfx: no texture implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType >
using generic_texture = _detail::texture_impl< PixelType, PointType > ;

using texture = generic_texture< rgba_u8_pixel, fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
