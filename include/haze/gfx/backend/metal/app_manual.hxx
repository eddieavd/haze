//
//
//      haze
//      gfx/backend/metal/app_manual.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>
#include <haze/gfx/backend/metal/basic_renderer.hxx>

#include <haze/gfx/context.hxx>

#include <haze/app/app_base.hxx>
#include <haze/app/window.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class app : public app_base< app >
{
        using _self = app ;
        using _base = app_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        using window_type =          window            ;
        using   size_type = typename window::size_type ;

        constexpr app (                                                            ) noexcept ;
        constexpr app ( size_type _width_, size_type _height_, string_view _title_ ) noexcept ;

        constexpr void init_window () noexcept ;

        constexpr context       & get_context ()       noexcept { return ctx_ ; }
        constexpr context const & get_context () const noexcept { return ctx_ ; }

        constexpr window       & get_window ()       noexcept { return window_ ; }
        constexpr window const & get_window () const noexcept { return window_ ; }
private:
        NS::Application * application_ {} ;
        MTK::View       *       _view_ {} ;
        context                   ctx_ {} ;
        window                 window_ {} ;

        constexpr void _run () ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr app::app () noexcept
        : application_{ NS::Application::sharedApplication() }
{}

////////////////////////////////////////////////////////////////////////////////

constexpr app::app ( size_type _width_, size_type _height_, string_view _title_ ) noexcept
        : application_{ NS::Application::sharedApplication() }
{
        window_.set_size ( _width_, _height_ ) ;
        window_.set_title( _title_           ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void app::init_window () noexcept
{

}

////////////////////////////////////////////////////////////////////////////////

constexpr void app::_run ()
{
        HAZE_CORE_INFO( "app::run : starting up..." ) ;
        ctx_.init() ;
        HAZE_CORE_INFO( "app::run : gfx context initialized" ) ;
        window_.init() ;
        HAZE_CORE_INFO( "app::run : window initialized" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
