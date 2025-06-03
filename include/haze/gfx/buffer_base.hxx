//
//
//      haze
//      gfx/buffer.hxx
//

#pragma once


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename BufferImpl >
class buffer_base
{
        using _impl = BufferImpl ;
public:
        constexpr auto       * owner ()       noexcept { return static_cast< _impl       * >( this )->_owner() ; }
        constexpr auto const * owner () const noexcept { return static_cast< _impl const * >( this )->_owner() ; }
protected:
        constexpr  buffer_base () noexcept = default ;
        constexpr ~buffer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
