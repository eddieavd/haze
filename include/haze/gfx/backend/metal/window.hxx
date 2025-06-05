//
//
//      haze
//      gfx/backend/metal/window.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/app/window_base.hxx>


////////////////////////////////////////////////////////////////////////////////

struct GLFWwindow ;

////////////////////////////////////////////////////////////////////////////////

namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class window : public window_base<   window     >
             , public ns_object< NS::Window     >
             , public ns_object< CA::MetalLayer >
{
        using     _self = window                      ;
        using _win_base = window_base<   window     > ;
        using  _ns_base = ns_object< NS::Window     > ;
        using  _ca_base = ns_object< CA::MetalLayer > ;
public:
        using layer_type = ::haze:: layer                  ;
        using pixel_type = typename layer_type::pixel_type ;

        using  _win_base::_win_base ;
        friend _win_base            ;

        constexpr window ( NS::Window * _window_, CA::MetalLayer * _layer_, GLFWwindow * _glfw_window_ )
                : _win_base()
                , _ns_base( _window_ )
                , _ca_base(  _layer_ )
                , glfw_window_{ _glfw_window_ } {}

        constexpr window ( window const &  _other_ ) ;
        constexpr window ( window       && _other_ ) noexcept
                : _win_base()
                , _ns_base( UTI_MOVE( _other_            ) )
                , _ca_base( UTI_MOVE( _other_            ) )
                , glfw_window_(       _other_.glfw_window_ )
                , clear_color_{       _other_.clear_color_ }
        { _other_.glfw_window_ = nullptr ; }

        constexpr window & operator= ( window const &  _other_ ) ;
        constexpr window & operator= ( window       && _other_ ) noexcept
        {
                _destroy() ;
                _ns_base::operator=( UTI_MOVE( _other_ ) ) ;
                _ca_base::operator=( UTI_MOVE( _other_ ) ) ;

                glfw_window_ = _other_.glfw_window_ ;
                _other_.glfw_window_ = nullptr ;

                clear_color_ = _other_.clear_color_ ;

                return *this ;
        }

        ~window () noexcept ;

        constexpr operator GLFWwindow       * ()       noexcept { return glfw_window_ ; }
        constexpr operator GLFWwindow const * () const noexcept { return glfw_window_ ; }
private:
        GLFWwindow * glfw_window_ ;

        layer_type       layer_ ;
        pixel_type clear_color_ ;

        std::function< void( _self & ) > on_update_ { []( auto & ){} } ;

        constexpr bool _closed () const noexcept { return glfw_window_ == nullptr ; }

        constexpr pixel_type       & _clear_color ()       noexcept { return clear_color_ ; }
        constexpr pixel_type const & _clear_color () const noexcept { return clear_color_ ; }

        constexpr layer_type       & _layer ()       noexcept { return layer_ ; }
        constexpr layer_type const & _layer () const noexcept { return layer_ ; }

        constexpr string_view _title () const noexcept { return "unimplemented" ; }

        constexpr void _set_on_update ( std::function< void( _self & ) > const & _on_update_ ) noexcept { on_update_ = _on_update_ ; }

        constexpr void _on_update () { on_update_( *this ) ; }

        void _destroy () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
