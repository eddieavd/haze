//
//
//      haze
//      gfx/backend/metal/window.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/app/window_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class window : public window_base< window >
{
        using _self = window ;
        using _base = window_base< _self > ;
public:
        using point_type = typename _base::point_type ;
        using shape_type = typename _base::shape_type ;
        using  size_type = typename _base:: size_type ;

        using  _base::_base ;
        friend _base        ;

        constexpr window (                                                            ) noexcept = default ;
        constexpr window ( size_type _width_, size_type _height_, string_view _title_ ) noexcept           ;

        constexpr window             ( window const &  _other_ ) noexcept ;
        constexpr window             ( window       && _other_ ) noexcept ;
        constexpr window & operator= ( window const &  _other_ ) noexcept ;
        constexpr window & operator= ( window       && _other_ ) noexcept ;

        constexpr ~window () noexcept { _release() ; }

        constexpr void set_title ( string_view _title_                     ) noexcept ;
        constexpr void set_size  ( size_type   _width_, size_type _height_ ) noexcept ;
private:
        CGRect        shape_ { { 100, 100 }, { static_cast< CGFloat >( 800 ), static_cast< CGFloat >( 600 ) } } ;
        string        title_ {} ;
        NS::Window * window_ {} ;

        constexpr size_type   _width () const noexcept { return shape_.size. width ; }
        constexpr size_type  _height () const noexcept { return shape_.size.height ; }
        constexpr string_view _title () const noexcept { return title_ ; }
        constexpr auto        _shape () const noexcept { return shape_ ; }

        constexpr NS::Window       * _impl ()       noexcept { return window_ ; }
        constexpr NS::Window const * _impl () const noexcept { return window_ ; }

        constexpr void _init () noexcept ;

        constexpr void _release () noexcept { if( window_ ) { window_->release() ; window_ = nullptr ; } }
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr window::window ( size_type _width_, size_type _height_, string_view _title_ ) noexcept
        : shape_{ { 100, 100 }, { static_cast< CGFloat >( _width_ ), static_cast< CGFloat >( _height_ ) } }
{
        HAZE_CORE_TRACE_S( "mtl::window::ctor_body" ) ;
        set_size ( _width_, _height_ ) ;
        set_title( _title_           ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr window::window ( window const & _other_ ) noexcept
        : shape_ { _other_. shape_ }
        , title_ ( _other_. title_ )
        , window_{ _other_.window_ }
{
        HAZE_CORE_TRACE_S( "mtl::window : ctor_copy_body" ) ;
        window_->retain() ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr window::window ( window && _other_ ) noexcept
        : shape_ { _other_.shape_ }
        , title_ ( UTI_MOVE( _other_.title_ ) )
        , window_{ _other_.window_ }
{
        HAZE_CORE_TRACE_S( "mtl::window : ctor_move_body" ) ;
        _other_.window_ = nullptr ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr window & window::operator= ( window const & _other_ ) noexcept
{
        HAZE_CORE_TRACE_S( "mtl::window : assign_copy_body" ) ;
        _release() ;

        shape_  = _other_. shape_ ;
        title_  = _other_. title_ ;
        window_ = _other_.window_ ;

        window_->retain() ;

        return *this ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr window & window::operator= ( window && _other_ ) noexcept
{
        HAZE_CORE_TRACE_S( "mtl::window : assign_move_body" ) ;
        _release() ;

        shape_  = _other_. shape_ ;
        title_  = _other_. title_ ;
        window_ = _other_.window_ ;

        _other_.shape_  =      {} ;
        _other_.title_  =      {} ;
        _other_.window_ = nullptr ;

        return *this ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void window::set_title ( string_view _title_ ) noexcept
{
        title_ = string( _title_ ) ;

        if( window_ ) window_->setTitle( NS::String::string( title_.c_str(), NS::StringEncoding::UTF8StringEncoding ) ) ;

        HAZE_CORE_DBG( "mtl::window::set_title : window title set to '" SV_FMT "'", SV_ARG( title_ ) ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void window::set_size ( size_type _width_, size_type _height_ ) noexcept
{
        if( window_ != nullptr ) { HAZE_CORE_ERROR( "mtl::window::set_size : can not set size on existing window" ) ; return ; }

        shape_  = { { 100, 100 }, { static_cast< CGFloat >( _width_ ), static_cast< CGFloat >( _height_ ) } } ;

        HAZE_CORE_DBG( "mtl::window::set_size : window size set to %ldx%ld", _width_, _height_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void window::_init () noexcept
{
        window_ = NS::Window::alloc()->init(
                shape_,
                NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
                NS::BackingStoreBuffered,
                false
        ) ;
        if( !window_ ) HAZE_CORE_FATAL( "mtl::window::init : failed to initialize window!" ) ;

        window_->setTitle( NS::String::string( title_.c_str(), NS::StringEncoding::UTF8StringEncoding ) ) ;

        HAZE_CORE_TRACE( "mtl::window::init : initialized window '" SV_FMT "' with resolution %.fx%.f", SV_ARG( title_ ), shape_.size.width, shape_.size.height ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
