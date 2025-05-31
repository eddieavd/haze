//
//
//      haze
//      gfx/backend/metal/app.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/cpugfx/scene/layer.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>
#include <haze/gfx/backend/metal/basic_renderer.hxx>

#include <haze/gfx/context.hxx>

#include <haze/app/window.hxx>
#include <haze/gfx/renderer.hxx>
#include <haze/app/app_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

using layer = generic_layer< rgba_u8_pixel, fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////

class app : public app_base< app >
{
        using _self = app ;
        using _base = app_base< _self > ;

        class view_delegate : public MTK::ViewDelegate
        {
                using _base = MTK::ViewDelegate ;
        public:
                friend app ;

                constexpr view_delegate ( context & _ctx_ )          ;
                virtual  ~view_delegate (                 ) override ;

                constexpr void set_on_update ( std::function< void( layer & ) > const & _update_ ) noexcept { update_ = _update_ ; }

                virtual void drawInMTKView ( MTK::View * _view_ ) override ;

                constexpr void release () noexcept { renderer_.release() ; }
        private:
                renderer renderer_ ;
                layer       layer_ ;

                std::function< void( layer & ) > update_ ;
        } ;

        class app_delegate : public NS::ApplicationDelegate
        {
        public:
                friend app ;

                constexpr app_delegate () noexcept { ctx_.init() ; }

                ~app_delegate () noexcept ;

                constexpr void set_on_update ( std::function< void( layer & ) > const & _update_ ) noexcept { update_ = _update_ ; }

                virtual void applicationWillFinishLaunching                  ( NS::Notification * _notification_ ) override ;
                virtual void applicationDidFinishLaunching                   ( NS::Notification * _notification_ ) override ;
                virtual bool applicationShouldTerminateAfterLastWindowClosed ( NS:: Application *       _sender_ ) override ;

                constexpr void on_terminate () noexcept
                {
//                      HAZE_CORE_WARN( "app::app_delegate::on_terminate : taking a leak" ) ;

                        HAZE_CORE_DBG( "app::app_delegate::on_terminate : releasing view..." ) ;
                        view_->release() ;
//                      HAZE_CORE_DBG( "app::app_delegate::on_terminate : releasing window..." ) ;
//                      window_.release() ;
                        HAZE_CORE_DBG( "app::app_delegate::on_terminate : releasing context..." ) ;
                        ctx_.release() ;

                        HAZE_CORE_DBG( "app::app_delegate::on_terminate : deleting view delegate..." ) ;
                        delete view_dlgt_ ;
                }
        private:
                context               ctx_ {} ;
                window             window_ {} ;
                MTK::    View *      view_ {} ;
                view_delegate * view_dlgt_ {} ;

                std::function< void( layer & ) > update_ ;

                NS::Menu * _create_menu_bar () ;
        } ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  app () noexcept ;
        constexpr ~app () noexcept ;

        constexpr void on_update ( std::function< void( layer & ) > const & _update_fn_ ) noexcept
        { app_delegate_.set_on_update( UTI_FWD( _update_fn_ ) ) ; }

        constexpr window       & get_window ()       noexcept { return app_delegate_.window_ ; }
        constexpr window const & get_window () const noexcept { return app_delegate_.window_ ; }

        constexpr context       & get_context ()       noexcept { return app_delegate_.ctx_ ; }
        constexpr context const & get_context () const noexcept { return app_delegate_.ctx_ ; }
private:
        NS::Application * application_ {} ;
        app_delegate     app_delegate_ {} ;

        constexpr void _run () ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr app::view_delegate::view_delegate ( context & _ctx_ )
        : _base()
        , renderer_( _ctx_ )
{
        HAZE_CORE_DBG( "app::view_delegate : created" ) ;
}

inline app::view_delegate::~view_delegate () noexcept
{
        release() ;
        HAZE_CORE_DBG( "app::view_delegate : destroyed" ) ;
}

inline void app::view_delegate::drawInMTKView ( MTK::View * _view_ )
{
        [[ maybe_unused ]]
        static bool init_renderer = [ & ]
        {
                renderer_.init() ;
                return true ;
        }() ;

        HAZE_CORE_YAP_S( "app::view_delegate::drawInMTKView : system requested redraw" ) ;

        HAZE_CORE_YAP_S( "app::view_delegate::drawInMTKView : calling on_update handler..." ) ;
        update_( layer_ ) ;
        HAZE_CORE_YAP_S( "app::view_delegate::drawInMTKView : on_update finished" ) ;

        HAZE_CORE_YAP_S( "app::view_delegate::drawInMTKView : invoking renderer..." ) ;
        renderer_.draw( layer_, _view_ ) ;
        HAZE_CORE_YAP_S( "app::view_delegate::drawInMTKView : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////

inline app::app_delegate::~app_delegate () noexcept
{
        if( view_ ) view_ ->release() ;
        window_.release() ;
        if( view_dlgt_ ) delete view_dlgt_ ;
        ctx_.release() ;

        HAZE_CORE_DBG( "app::app_delegate : destroyed" ) ;
}

inline NS::Menu * app::app_delegate::_create_menu_bar ()
{
        HAZE_CORE_DBG( "app::app_delegate::_create_menu_bar : creating menu bar..." ) ;

        using NS::StringEncoding::UTF8StringEncoding ;

        NS::Menu     * main_menu      = NS::Menu    ::alloc()->init() ;
        NS::MenuItem *  app_menu_item = NS::MenuItem::alloc()->init() ;
        NS::Menu     *  app_menu      = NS::Menu    ::alloc()->init( NS::String::string( "haze::appname", UTF8StringEncoding ) ) ;

        NS::String * app_name = NS::RunningApplication::currentApplication()->localizedName() ;
        NS::String * quit_item_name = NS::String::string( "quit ", UTF8StringEncoding )->stringByAppendingString( app_name ) ;

        SEL quit_cb = NS::MenuItem::registerActionCallback( "appQuit",
                []( void *, SEL, NS::Object const * _sender_ )
                {
                        auto app = NS::Application::sharedApplication() ;
                        app->terminate( _sender_ ) ;
                }
        ) ;
        NS::MenuItem * app_quit_item = app_menu->addItem( quit_item_name, quit_cb, NS::String::string( "q", UTF8StringEncoding ) ) ;

        app_quit_item->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand ) ;
        app_menu_item->setSubmenu( app_menu ) ;

        NS::MenuItem * window_menu_item = NS::MenuItem::alloc()->init() ;
        NS::Menu     * window_menu = NS::Menu::alloc()->init( NS::String::string( "window", UTF8StringEncoding ) ) ;

        SEL close_window_cb = NS::MenuItem::registerActionCallback( "windowClose",
                []( void *, SEL, NS::Object const * )
                {
                        auto app = NS::Application::sharedApplication() ;
                        app->windows()->object< NS::Window >( 0 )->close() ;
                }
        ) ;
        NS::MenuItem * close_window_item = window_menu->addItem( NS::String::string( "close window", UTF8StringEncoding ), close_window_cb, NS::String::string( "w", UTF8StringEncoding ) ) ;
        close_window_item->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand ) ;

        window_menu_item->setSubmenu( window_menu ) ;

        main_menu->addItem(    app_menu_item ) ;
        main_menu->addItem( window_menu_item ) ;

        app_menu_item   ->release() ;
        window_menu_item->release() ;
        app_menu        ->release() ;
        window_menu     ->release() ;

        HAZE_CORE_DBG( "app::app_delegate::_create_menu_bar : finished" ) ;

        return main_menu->autorelease() ;
}

inline void app::app_delegate::applicationWillFinishLaunching ( NS::Notification * _notification_ )
{
        HAZE_CORE_INFO( "app::app_delegate::applicationWillFinishLaunching : setting up menu bar..." ) ;

        NS::Menu        * menu = _create_menu_bar() ;
        NS::Application *  app = reinterpret_cast< NS::Application * >( _notification_->object() ) ;

        if( !menu || !app ) HAZE_CORE_FATAL( "app::app_delegate::applicationWillFinishLaunching : failed creating app menu" ) ;

        app->setMainMenu( menu ) ;
        app->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular ) ;

        HAZE_CORE_INFO( "app::app_delegate::applicationWillFinishLaunching : finished" ) ;
}

inline void app::app_delegate::applicationDidFinishLaunching ( NS::Notification * _notification_ )
{
        HAZE_CORE_INFO_S( "app::app_delegate::applicationDidFinishLaunching"                      ) ;
        HAZE_CORE_INFO  ( "app::app_delegate::applicationDidFinishLaunching : creating window..." ) ;

        window_.init() ;

        HAZE_CORE_INFO( "app::app_delegate::applicationDidFinishLaunching : initializing view..." ) ;

        view_ = MTK::View::alloc()->init( window_.shape(), ctx_.device() ) ;
        view_->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB ) ;
        view_->setClearColor( MTL::ClearColor::Make( 0.0, 0.0, 0.0, 1.0 ) ) ;

        HAZE_CORE_INFO( "app::app_delegate::applicationDidFinishLaunching : creating view delegate..." ) ;

        view_dlgt_ = new view_delegate( ctx_ ) ;
        view_dlgt_->set_on_update( update_ ) ;
        view_->setDelegate( view_dlgt_ ) ;

        HAZE_CORE_INFO( "app::app_delegate::applicationDidFinishLaunching : setting window content view..." ) ;
        window_.impl()->setContentView( view_ ) ;

        HAZE_CORE_INFO( "app::app_delegate::applicationDidFinishLaunching : ordering window to front..." ) ;

        window_.impl()->makeKeyAndOrderFront( nullptr ) ;

        NS::Application * app = reinterpret_cast< NS::Application * >( _notification_->object() ) ;
        app->activateIgnoringOtherApps( true ) ;

        HAZE_CORE_INFO( "app::app_delegate::applicationDidFinishLaunching : finished" ) ;
}

inline bool app::app_delegate::applicationShouldTerminateAfterLastWindowClosed ( NS::Application * )
{
        HAZE_CORE_INFO( "app::app_delegate::applicationShouldTerminateAfterLastWindowClosed : terminating..." ) ;

        on_terminate() ;

        return true ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr app::app () noexcept
        : application_ { NS ::Application::sharedApplication() }
{
        HAZE_CORE_DBG( "app : created" ) ;
}

constexpr app::~app () noexcept
{
        HAZE_CORE_INFO( "app : destroyed" ) ;

        application_->release() ;
}

constexpr void app::_run ()
{
        HAZE_CORE_INFO_S( "app::run" ) ;
        HAZE_CORE_INFO( "app::run : setting app delegate..." ) ;

        application_->setDelegate( &app_delegate_ ) ;

        HAZE_CORE_INFO( "app::run : launching application..." ) ;

        application_->run() ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
