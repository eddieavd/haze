//
//
//      haze
//      app/app_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/app/window.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename AppImpl >
class app_base
{
        using _impl = AppImpl ;
public:
        constexpr i32_t run ( i32_t _argc_, char ** _argv_ ) { return static_cast< _impl * >( this )->_run( _argc_, _argv_ ) ; }

        constexpr window & create_window ( window_options _opts_ ) { return static_cast< _impl * >( this )->_create_window( _opts_ ) ; }

        UTI_NODISCARD constexpr double   target_fps (              ) const noexcept { return static_cast< _impl const * >( this )->    _target_fps(       ) ; }
                      constexpr void set_target_fps ( double _fps_ )                {        static_cast< _impl       * >( this )->_set_target_fps( _fps_ ) ; }

        UTI_NODISCARD constexpr vector< window >       & windows ()       noexcept { return static_cast< _impl       * >( this )->_windows() ; }
        UTI_NODISCARD constexpr vector< window > const & windows () const noexcept { return static_cast< _impl const * >( this )->_windows() ; }
protected:
        constexpr  app_base () noexcept = default ;
        constexpr ~app_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
