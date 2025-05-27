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

        constexpr window ( size_type _width_, size_type _height_ ) noexcept ;

        constexpr ~window () noexcept ;
private:
        constexpr size_type  _width () const noexcept ;
        constexpr size_type _height () const noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
