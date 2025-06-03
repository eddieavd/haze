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

#include <haze/gfx/context.hxx>

#include <haze/app/window.hxx>
#include <haze/gfx/renderer_api.hxx>
#include <haze/gfx/renderer.hxx>
#include <haze/app/app_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class ns_app : private NS::Application
{
        constexpr void test ()
        {

        }

//      static Application*                sharedApplication();
//      void                                     setDelegate( const ApplicationDelegate* pDelegate );
//      bool                             setActivationPolicy( ActivationPolicy activationPolicy );
//      void                       activateIgnoringOtherApps( bool ignoreOtherApps );
//      void                                     setMainMenu( const class Menu* pMenu );
//      NS::Array*                                   windows() const;
//      void                                             run();
//      void                                       terminate( const Object* pSender );

} ;

////////////////////////////////////////////////////////////////////////////////

struct app_context_data
{
        context      ctx_ {} ;
        window    window_ {} ;
        MTK::View * view_ {} ;

        renderer_api renderer_api_ ;
        layer               layer_ ;

        std::function< void( layer & ) > update_ ;

        NS::Application * application_ {} ;
} ;

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

                constexpr view_delegate ( app_context_data * _appdata_ ) : appdata_( _appdata_ ) {}
                virtual  ~view_delegate (                              )   override              {}

                virtual void drawInMTKView ( MTK::View * _view_ ) override ;
        private:
                app_context_data * appdata_ ;
        } ;

        class app_delegate : public NS::ApplicationDelegate
        {
        public:
                friend app ;

                constexpr app_delegate () noexcept : appdata_{ {}, {}, {}, renderer_api( appdata_.ctx_ ), {}, {}, {} } {}

                ~app_delegate () noexcept ;

                virtual void applicationWillFinishLaunching                  ( NS::Notification * _notification_ ) override ;
                virtual void applicationDidFinishLaunching                   ( NS::Notification * _notification_ ) override ;
                virtual bool applicationShouldTerminateAfterLastWindowClosed ( NS:: Application *       _sender_ ) override ;

                constexpr void on_terminate () noexcept
                {
                        HAZE_CORE_DBG_S( "mtl::app::app_delegate::on_terminate" ) ;

                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : releasing view..." ) ;
                        appdata_.view_->release() ;
                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : view released" ) ;

//                      HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : releasing window..." ) ;
//                      appdata_.window_.release() ;
//                      HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : window released" ) ;

                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : releasing renderer..." ) ;
                        appdata_.renderer_api_.release() ;
                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : renderer released" ) ;

                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : releasing context..." ) ;
                        appdata_.ctx_.release() ;
                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : context released" ) ;

                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : releasing application..." ) ;
                        appdata_.application_->release() ;
                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : application released" ) ;

                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : deleting view delegate..." ) ;
                        delete view_dlgt_ ;
                        HAZE_CORE_DBG( "mtl::app::app_delegate::on_terminate : view delegate deleted" ) ;
                }
        private:
                app_context_data  appdata_ ;
                view_delegate * view_dlgt_ ;

                NS::Menu * _create_menu_bar () ;
        } ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  app () noexcept ;
        constexpr ~app () noexcept ;
private:
        app_delegate     app_delegate_ {} ;
        app_context_data *    appdata_ {} ;

        constexpr void _run () ;

        constexpr auto       & _window ()       noexcept { return appdata_->window_ ; }
        constexpr auto const & _window () const noexcept { return appdata_->window_ ; }

        constexpr void _on_update ( std::function< void( layer & ) > const & _on_update_ )
        { appdata_->update_ = _on_update_ ; }
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline void app::view_delegate::drawInMTKView ( MTK::View * _view_ )
{
        [[ maybe_unused ]]
        static bool init_renderer = [ & ]
        {
                HAZE_CORE_WARN( "mtl::app::view_delegate::drawInMTKView : first draw call, initializing renderer..." ) ;
                appdata_->renderer_api_.init( 64 * 1024 * 1024
                                            , 64 * 1024 * 1024
                                            , 64 * 1024 * 1024
                                            , 64 * 1024 * 1024
                                            , 64 * 1024 * 1024
                                            , 64 * 1024 * 1024
                                            , 64 * 1024 * 1024
                ) ;
                return true ;
        }() ;

        HAZE_CORE_YAP_S( "mtl::app::view_delegate::drawInMTKView : system requested redraw" ) ;
        {
        HAZE_CORE_YAP_S( "mtl::app::view_delegate::drawInMTKView : calling on_update handler..." ) ;
        appdata_->update_( appdata_->layer_ ) ;
        HAZE_CORE_YAP( "mtl::app::view_delegate::drawInMTKView : on_update finished" ) ;
        }
        {
        HAZE_CORE_YAP_S( "mtl::app::view_delegate::drawInMTKView : invoking renderer..." ) ;
        appdata_->renderer_api_.draw( appdata_->layer_, _view_ ) ;
        HAZE_CORE_YAP( "mtl::app::view_delegate::drawInMTKView : finished" ) ;
        }
}

////////////////////////////////////////////////////////////////////////////////

inline app::app_delegate::~app_delegate () noexcept
{

}

inline NS::Menu * app::app_delegate::_create_menu_bar ()
{
        HAZE_CORE_DBG_S( "mtl::app::app_delegate::_create_menu_bar" ) ;

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

        return main_menu->autorelease() ;
}

inline void app::app_delegate::applicationWillFinishLaunching ( NS::Notification * _notification_ )
{
        HAZE_CORE_TRACE_S( "mtl::app::app_delegate::applicationWillFinishLaunching" ) ;

        HAZE_CORE_DBG( "mtl::app::app_delegate::applicationWillFinishLaunching : setting up menu bar..." ) ;

        NS::Menu        * menu = _create_menu_bar() ;
        NS::Application *  app = reinterpret_cast< NS::Application * >( _notification_->object() ) ;

        if( !menu || !app ) HAZE_CORE_FATAL( "mtl::app::app_delegate::applicationWillFinishLaunching : failed creating app menu" ) ;

        app->setMainMenu( menu ) ;
        app->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular ) ;
}

inline void app::app_delegate::applicationDidFinishLaunching ( NS::Notification * _notification_ )
{
        HAZE_CORE_TRACE_S( "mtl::app::app_delegate::applicationDidFinishLaunching" ) ;

        appdata_.   ctx_.init() ;
        appdata_.window_.init() ;

        HAZE_CORE_DBG( "mtl::app::app_delegate::applicationDidFinishLaunching : window created" ) ;

        appdata_.view_ = MTK::View::alloc()->init( appdata_.window_.shape(), appdata_.ctx_.device() ) ;
        appdata_.view_->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB ) ;
        appdata_.view_->setClearColor( MTL::ClearColor::Make( 0.0, 0.0, 0.0, 1.0 ) ) ;

        HAZE_CORE_DBG( "mtl::app::app_delegate::applicationDidFinishLaunching : view initialized" ) ;

        view_dlgt_ = new view_delegate( &appdata_ ) ;
//      view_dlgt_->set_on_update( update_ ) ;
        appdata_.view_->setDelegate( view_dlgt_ ) ;

        HAZE_CORE_DBG( "mtl::app::app_delegate::applicationDidFinishLaunching : view delegate created" ) ;

        appdata_.window_.impl()->setContentView( appdata_.view_ ) ;

        HAZE_CORE_DBG( "mtl::app::app_delegate::applicationDidFinishLaunching : ordering window to front..." ) ;

        appdata_.window_.impl()->makeKeyAndOrderFront( nullptr ) ;

        NS::Application * app = reinterpret_cast< NS::Application * >( _notification_->object() ) ;
        app->activateIgnoringOtherApps( true ) ;
}

inline bool app::app_delegate::applicationShouldTerminateAfterLastWindowClosed ( NS::Application * )
{
        HAZE_CORE_INFO( "app::app_delegate : last window closed, terminating..." ) ;

        on_terminate() ;

        return true ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr app::app () noexcept
        : app_delegate_{}
        , appdata_{ &app_delegate_.appdata_ }
{
        appdata_->application_ = NS::Application::sharedApplication() ;
}

constexpr app::~app () noexcept
{
        HAZE_CORE_TRACE_S( "app::dtor_body" ) ;

        appdata_->application_->release() ;
}

constexpr void app::_run ()
{
        HAZE_CORE_INFO_S( "app::run" ) ;
        HAZE_CORE_DBG( "app::run : setting app delegate..." ) ;

        appdata_->application_->setDelegate( &app_delegate_ ) ;

        HAZE_CORE_INFO( "app::run : launching application..." ) ;

        appdata_->application_->run() ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
