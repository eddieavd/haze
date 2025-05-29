//
//
//      haze
//      app/app_base.hxx
//

#pragma once


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename AppImpl >
class app_base
{
        using _impl = AppImpl ;
public:
        constexpr void run () { static_cast< _impl * >( this )->_run() ; }
protected:
        constexpr  app_base () noexcept = default ;
        constexpr ~app_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
