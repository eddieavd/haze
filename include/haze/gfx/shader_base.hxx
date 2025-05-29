//
//
//      haze
//      gfx/shader_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename ShaderImpl >
class shader_base
{
        using _impl = ShaderImpl ;
public:
        constexpr void    init () noexcept { static_cast< _impl * >( this )->   _init() ; }
        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }
protected:
        constexpr  shader_base () noexcept = default ;
        constexpr ~shader_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
