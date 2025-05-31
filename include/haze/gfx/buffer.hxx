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
namespace haze::_detail { using        buffer_impl = ::haze::mtl::       buffer ; 
                          using vertex_buffer_impl = ::haze::mtl::vertex_buffer ;
                          using  index_buffer_impl = ::haze::mtl:: index_buffer ;
}
#elif defined( HAZE_USE_OPENGL )

#include <haze/gfx/backend/opengl/buffer.hxx>
namespace haze::_detail { using        buffer_impl = ::haze::gl::       buffer ;
                          using vertex_buffer_impl = ::haze::gl::vertex_buffer ;
                          using  index_buffer_impl = ::haze::gl:: index_buffer ;
}
#elif defined( HAZE_USE_RAYLIB )

#include <haze/gfx/backend/raylib/buffer.hxx>
namespace haze::_detail { using        buffer_impl = ::haze::ray::       buffer ;
                          using vertex_buffer_impl = ::haze::ray::vertex_buffer ;
                          using  index_buffer_impl = ::haze::ray:: index_buffer ;
}
#else
#       error "haze::gfx: no buffer implementation defined"
#endif


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using        buffer = _detail::       buffer_impl ;
using vertex_buffer = _detail::vertex_buffer_impl ;
using  index_buffer = _detail:: index_buffer_impl ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
