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
#include <haze/gfx/buffer.hxx>
#include <haze/gfx/allocator.hxx>
#include <haze/gfx/renderer_api.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

static renderer_api_config default_renderer_api_config =
{
        .dynamic_alloc_config =
        {
                .name_                 = string( "dynamic_shared" ) ,
                .placement_strategy_   = placement_strategy::automatic ,
                .cpu_cache_mode_       = cpu_cache_mode::dflt ,
                .storage_mode_         = storage_mode::shared ,
                .hazard_tracking_mode_ = hazard_tracking_mode::untracked ,
                .sparse_page_size_     = sparse_page_size::size_16 ,
        }
} ;

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

        renderer_api renderer_api_ ;

        i32_t _run ( i32_t _argc_, char ** _argv_ ) ;

        window & _create_window ( window_options _opts_ ) ;

        double   _target_fps (              ) const noexcept ;
        void _set_target_fps ( double _fps_ )                ;

        constexpr vector< window >       & _windows ()       noexcept { return windows_ ; }
        constexpr vector< window > const & _windows () const noexcept { return windows_ ; }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
