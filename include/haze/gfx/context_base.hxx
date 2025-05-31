//
//
//      haze
//      gfx/context_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/common.hxx>
#include <haze/gfx/buffer.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename ContextImpl >
class context_base
{
        using _impl = ContextImpl ;
public:
        static constexpr auto create_initialized () { _impl ctx ; ctx.init() ; return ctx ; }

        constexpr void    init ()          { static_cast< _impl * >( this )->   _init() ; }
        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }
protected:
        constexpr  context_base () noexcept = default ;
        constexpr ~context_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
