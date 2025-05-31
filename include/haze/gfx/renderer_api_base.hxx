//
//
//      haze
//      gfx/renderer_api_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/cpugfx/scene/layer.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using layer = generic_layer< rgba_u8_pixel, fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////

template< typename RendererImpl >
class renderer_api_base
{
        using _impl = RendererImpl ;
public:
        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }

        constexpr void init ( ssize_t       _dynamic_alloc_init_    , ssize_t       _static_alloc_init_    ,
                              ssize_t _dynamic_write_alloc_init_ = 0, ssize_t _static_write_alloc_init_ = 0,
                              ssize_t      _sparse16_alloc_init_ = 0, ssize_t     _sparse64_alloc_init_ = 0, ssize_t _sparse256_alloc_init_ = 0 )
        {
                static_cast< _impl * >( this )->_init
                (
                              _dynamic_alloc_init_,       _static_alloc_init_,
                        _dynamic_write_alloc_init_, _static_write_alloc_init_,
                             _sparse16_alloc_init_,     _sparse64_alloc_init_, _sparse256_alloc_init_
                ) ;
        }

        constexpr void draw ( layer const & _layer_, void * _view_ ) // abstract away the view into a higher level thing
        { static_cast< _impl * >( this )->_draw( _layer_, _view_ ) ; }
protected:
        constexpr  renderer_api_base () noexcept = default ;
        constexpr ~renderer_api_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
