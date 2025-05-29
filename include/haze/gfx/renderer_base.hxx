//
//
//      haze
//      gfx/renderer_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/cpugfx/scene/layer.hxx>

#include <haze/gfx/texture.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

/*
struct vertex
{
        simd::float3   position ;
        simd::float4      color ;
        simd::float2 tex_coords ;
              float  tex_index  ;
} ;
*/

////////////////////////////////////////////////////////////////////////////////

using layer = generic_layer< rgba_u8_pixel, fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////

template< typename RendererImpl >
class renderer_base
{
        using _impl = RendererImpl ;
public:
        constexpr void    init ()          { static_cast< _impl * >( this )->   _init() ; }
        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }

        constexpr void draw ( layer const & _layer_, void * _view_ ) // abstract away the view into a higher level thing
        { static_cast< _impl * >( this )->_draw( _layer_, _view_ ) ; }
protected:
        constexpr  renderer_base () noexcept = default ;
        constexpr ~renderer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
