//
//
//      haze
//      app/app_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/cpugfx/scene/layer.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/app/window.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using layer = generic_layer< rgba_u8_pixel, fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////

template< typename AppImpl >
class app_base
{
        using _impl = AppImpl ;
public:
        constexpr void run () { static_cast< _impl * >( this )->_run() ; }

        constexpr auto       & window ()       noexcept { return static_cast< _impl       * >( this )->_window() ; }
        constexpr auto const & window () const noexcept { return static_cast< _impl const * >( this )->_window() ; }

        constexpr void on_update ( std::function< void( layer & ) > const & _on_update_ )
        { static_cast< _impl * >( this )->_on_update( _on_update_ ) ; }
protected:
        constexpr  app_base () noexcept = default ;
        constexpr ~app_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
