//
//
//      haze
//      gfx/renderer_api_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/common.hxx>
#include <haze/gfx/allocator.hxx>
#include <haze/app/window.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

struct renderer_api_config
{
        allocator_config dynamic_alloc_config ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename RenderApiImpl >
class renderer_api_base
{
        using _impl = RenderApiImpl ;
public:
        constexpr void clear () { static_cast< _impl * >( this )->_clear() ; }

        constexpr void   load_vertices ( layer const & _layer_                ) { static_cast< _impl * >( this )->  _load_vertices( _layer_        ) ; }
        constexpr void update_vertices ( layer const & _layer_, ssize_t _idx_ ) { static_cast< _impl * >( this )->_update_vertices( _layer_, _idx_ ) ; }

        constexpr void draw ( window & _window_ ) { static_cast< _impl * >( this )->_draw( _window_ ) ; } // I_CAN_FIX_HER: decouple from metal, take a texture

        constexpr void     init ()          { static_cast< _impl * >( this )->    _init() ; }
        constexpr void shutdown () noexcept { static_cast< _impl * >( this )->_shutdown() ; }
protected:
        constexpr  renderer_api_base () noexcept = default ;
        constexpr ~renderer_api_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
