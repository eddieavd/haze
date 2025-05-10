//
//
//      haze
//      common/time,hxx
//

#pragma once

#include <haze/common/types.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

namespace meta
{


template< typename TimeType >
concept time_like = requires( TimeType time )
{
        typename TimeType::time_tag ;
        { TimeType::factor } -> uti::meta::convertible_to< double > ;
        { time.val       } -> uti::meta::convertible_to< double > ;
} ;


} // namespace meta

////////////////////////////////////////////////////////////////////////////////

struct  nanoseconds { struct time_tag {} ; static constexpr double factor { 1000000000.0      } ; double val ; template< meta::time_like T > constexpr operator T () const noexcept { return T{ val / factor * T::factor } ; } } ;
struct microseconds { struct time_tag {} ; static constexpr double factor { 1000000.0         } ; double val ; template< meta::time_like T > constexpr operator T () const noexcept { return T{ val / factor * T::factor } ; } } ;
struct milliseconds { struct time_tag {} ; static constexpr double factor { 1000.0            } ; double val ; template< meta::time_like T > constexpr operator T () const noexcept { return T{ val / factor * T::factor } ; } } ;
struct      seconds { struct time_tag {} ; static constexpr double factor { 1.0               } ; double val ; template< meta::time_like T > constexpr operator T () const noexcept { return T{ val / factor * T::factor } ; } } ;
struct      minutes { struct time_tag {} ; static constexpr double factor { 1.0 / ( 60      ) } ; double val ; template< meta::time_like T > constexpr operator T () const noexcept { return T{ val / factor * T::factor } ; } } ;
struct        hours { struct time_tag {} ; static constexpr double factor { 1.0 / ( 60 * 60 ) } ; double val ; template< meta::time_like T > constexpr operator T () const noexcept { return T{ val / factor * T::factor } ; } } ;

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T, meta::time_like U >
UTI_NODISCARD constexpr T operator+ ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return T{ ( _lhs_.val / T::factor + _rhs_.val / U::factor ) * T::factor } ;
}

template< meta::time_like T, meta::time_like U >
UTI_NODISCARD constexpr T operator- ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return T{ ( _lhs_.val / T::factor - _rhs_.val / U::factor ) * T::factor } ;
}

template< meta::time_like T, meta::time_like U >
UTI_NODISCARD constexpr T operator* ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return T{ ( ( _lhs_.val / T::factor ) * ( _rhs_.val / U::factor ) ) * T::factor } ;
}

template< meta::time_like T, meta::time_like U >
UTI_NODISCARD constexpr T operator/ ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return T{ ( ( _lhs_.val / T::factor ) / ( _rhs_.val / U::factor ) ) * T::factor } ;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

class duration
{
public:
        template< meta::time_like TimeType >
        constexpr duration ( TimeType const & _time_ ) noexcept : nanos_( _time_ ) {}

        constexpr duration ( double _seconds_ ) noexcept : duration( seconds( _seconds_ ) ) {}

        UTI_NODISCARD constexpr  nanoseconds to_nanos  () const noexcept { return               nanos_   ; }
        UTI_NODISCARD constexpr microseconds to_micros () const noexcept { return microseconds( nanos_ ) ; }
        UTI_NODISCARD constexpr milliseconds to_millis () const noexcept { return milliseconds( nanos_ ) ; }
        UTI_NODISCARD constexpr      seconds to_secs   () const noexcept { return      seconds( nanos_ ) ; }
        UTI_NODISCARD constexpr      minutes to_mins   () const noexcept { return      minutes( nanos_ ) ; }
        UTI_NODISCARD constexpr        hours to_hours  () const noexcept { return        hours( nanos_ ) ; }
private:
        nanoseconds nanos_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
