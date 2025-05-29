//
//
//      haze
//      gfx/backend/metal/shader.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/shader_base.hxx>

#include <haze/gfx/backend/metal/common.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class shader : public shader_base< shader >
{
        using _self = shader ;
        using _base = shader_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  shader () noexcept = default ;
        constexpr ~shader () noexcept ;
private:
        constexpr void    _init ()          ;
        constexpr void _release () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
