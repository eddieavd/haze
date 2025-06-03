//
//
//      haze
//      gfx/context_base.hxx
//

#pragma once


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename ContextImpl >
class context_base
{
        using _impl = ContextImpl ;
public:

protected:
        constexpr  context_base () noexcept = default ;
        constexpr ~context_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
