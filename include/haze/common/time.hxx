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
        { TimeType::factor } -> uti::meta::convertible_to< double > ;
        { time.val         } -> uti::meta::convertible_to< double > ;
} ;


} // namespace meta

////////////////////////////////////////////////////////////////////////////////

template< double Factor >
struct time
{
        static constexpr double factor { Factor } ;
                         double    val            ;

        template< meta::time_like TimeType >
        constexpr operator TimeType () const noexcept
        {
                return TimeType{ val / factor * TimeType::factor } ;
        }
} ;

using  nanoseconds = time< 1000000000.0 > ;
using microseconds = time< 1000000.0    > ;
using milliseconds = time< 1000.0       > ;
using      seconds = time< 1.0          > ;
using      minutes = time< 1.0 /   60   > ;
using        hours = time< 1.0 / 3600   > ;

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

class duration
{
public:
        template< meta::time_like TimeType >
        constexpr duration ( TimeType const & _time_ ) noexcept : nanos_( _time_ ) {}

        constexpr duration ( double _seconds_ ) noexcept : duration( seconds( _seconds_ ) ) {}

        UTI_NODISCARD constexpr  nanoseconds to_nanos  () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr microseconds to_micros () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr milliseconds to_millis () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr      seconds to_secs   () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr      minutes to_mins   () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr        hours to_hours  () const noexcept { return nanos_ ; }
private:
        nanoseconds nanos_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
