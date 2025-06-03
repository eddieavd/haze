//
//
//      haze
//      gfx/backend/metal/app.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>

#include <haze/app/app_base.hxx>
#include <haze/gfx/context.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class app : public app_base< app >
{
        using _base = app_base< app > ;
public:
        using  _base::_base ;
        friend _base        ;

         app ()          ;
        ~app () noexcept ;
private:
        context              ctx_ ;
        vector< window > windows_ ;

        i32_t _run ( i32_t _argc_, char ** _argv_ ) ;

        window & _create_window ( window_options _opts_ ) ;

        double   _target_fps (              ) const noexcept ;
        void _set_target_fps ( double _fps_ )                ;

        constexpr vector< window >       & _windows ()       noexcept { return windows_ ; }
        constexpr vector< window > const & _windows () const noexcept { return windows_ ; }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
