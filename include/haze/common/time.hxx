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

template< meta::time_like T >
constexpr T & operator+= ( T & _lhs_, T const & _rhs_ ) noexcept
{
        _lhs_.val += _rhs_.val ;
        return _lhs_ ;
}

template< meta::time_like T >
constexpr T & operator-= ( T & _lhs_, T const & _rhs_ ) noexcept
{
        _lhs_.val -= _rhs_.val ;
        return _lhs_ ;
}

template< meta::time_like T >
constexpr T & operator*= ( T & _lhs_, T const & _rhs_ ) noexcept
{
        _lhs_.val *= _rhs_.val ;
        return _lhs_ ;
}

template< meta::time_like T >
constexpr T & operator/= ( T & _lhs_, T const & _rhs_ ) noexcept
{
        _lhs_.val /= _rhs_.val ;
        return _lhs_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T >
UTI_NODISCARD constexpr T operator+ ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time += _rhs_ ;
        return time ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr T operator- ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time -= _rhs_ ;
        return time ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr T operator* ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time *= _rhs_ ;
        return time ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr T operator/ ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time /= _rhs_ ;
        return time ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T >
UTI_NODISCARD constexpr bool operator== ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        return _lhs_.val == _rhs_.val ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr bool operator!= ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        return _lhs_.val != _rhs_.val ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr bool operator< ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        return _lhs_.val < _rhs_.val ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr bool operator<= ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        return _lhs_.val <= _rhs_.val ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr bool operator> ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        return _lhs_.val > _rhs_.val ;
}

template< meta::time_like T >
UTI_NODISCARD constexpr bool operator>= ( T const & _lhs_, T const & _rhs_ ) noexcept
{
        return _lhs_.val >= _rhs_.val ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
constexpr T & operator+= ( T & _lhs_, U const & _rhs_ ) noexcept
{
        _lhs_.val = ( _lhs_.val / T::factor + _rhs_.val / U::factor ) * T::factor ;
        return _lhs_ ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
constexpr T & operator-= ( T & _lhs_, U const & _rhs_ ) noexcept
{
        _lhs_.val = ( _lhs_.val / T::factor - _rhs_.val / U::factor ) * T::factor ;
        return _lhs_ ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
constexpr T & operator*= ( T & _lhs_, U const & _rhs_ ) noexcept
{
        _lhs_.val = ( ( _lhs_.val / T::factor ) * ( _rhs_.val / U::factor ) ) * T::factor ;
        return _lhs_ ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
constexpr T & operator/= ( T & _lhs_, U const & _rhs_ ) noexcept
{
        _lhs_.val = ( ( _lhs_.val / T::factor ) / ( _rhs_.val / U::factor ) ) * T::factor ;
        return _lhs_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr T operator+ ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time += _rhs_ ;
        return time ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr T operator- ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time -= _rhs_ ;
        return time ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr T operator* ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time *= _rhs_ ;
        return time ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr T operator/ ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        auto time = _lhs_ ;
        time /= _rhs_ ;
        return time ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr bool operator== ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return ( _lhs_.val / T::factor ) == ( _rhs_.val / U::factor ) ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr bool operator!= ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return ( _lhs_.val / T::factor ) != ( _rhs_.val / U::factor ) ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr bool operator< ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return ( _lhs_.val / T::factor ) < ( _rhs_.val / U::factor ) ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr bool operator<= ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return ( _lhs_.val / T::factor ) <= ( _rhs_.val / U::factor ) ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr bool operator> ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return ( _lhs_.val / T::factor ) > ( _rhs_.val / U::factor ) ;
}

template< meta::time_like T, meta::time_like U >
        requires( !uti::meta::same_as< T, U > )
UTI_NODISCARD constexpr bool operator>= ( T const & _lhs_, U const & _rhs_ ) noexcept
{
        return ( _lhs_.val / T::factor ) >= ( _rhs_.val / U::factor ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::time_like T, uti::meta::arithmetic U >
UTI_NODISCARD constexpr T operator* ( T const & _time_, U _scale_ ) noexcept
{
        return T{ _time_.val * _scale_ } ;
}

template< meta::time_like T, uti::meta::arithmetic U >
UTI_NODISCARD constexpr T operator/ ( T const & _time_, U _scale_ ) noexcept
{
        return T{ _time_.val / _scale_ } ;
}

////////////////////////////////////////////////////////////////////////////////

class duration
{
public:
        constexpr duration (                  ) noexcept = default ;
        constexpr duration ( double _seconds_ ) noexcept : duration( seconds( _seconds_ ) ) {}

        template< meta::time_like TimeType >
        constexpr duration ( TimeType const & _time_ ) noexcept : nanos_( _time_ ) {}

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr operator  nanoseconds () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr operator microseconds () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr operator milliseconds () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr operator      seconds () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr operator      minutes () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr operator        hours () const noexcept { return nanos_ ; }

        UTI_NODISCARD constexpr  nanoseconds to_nanos  () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr microseconds to_micros () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr milliseconds to_millis () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr      seconds to_secs   () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr      minutes to_mins   () const noexcept { return nanos_ ; }
        UTI_NODISCARD constexpr        hours to_hours  () const noexcept { return nanos_ ; }

////////////////////////////////////////////////////////////////////////////////

        constexpr duration & operator+= ( duration const & _other_ ) noexcept
        {
                nanos_ = nanos_ + _other_.nanos_ ;
                return *this ;
        }

        constexpr duration & operator-= ( duration const & _other_ ) noexcept
        {
                nanos_ = nanos_ - _other_.nanos_ ;
                return *this ;
        }

        constexpr duration & operator*= ( duration const & _other_ ) noexcept
        {
                nanos_ = nanos_ * _other_.nanos_ ;
                return *this ;
        }

        constexpr duration & operator/= ( duration const & _other_ ) noexcept
        {
                nanos_ = nanos_ / _other_.nanos_ ;
                return *this ;
        }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr duration operator+ ( duration const & _other_ ) const noexcept
        {
                auto time = *this ;
                time += _other_ ;
                return time ;
        }

        UTI_NODISCARD constexpr duration operator- ( duration const & _other_ ) const noexcept
        {
                auto time = *this ;
                time -= _other_ ;
                return time ;
        }

        UTI_NODISCARD constexpr duration operator* ( duration const & _other_ ) const noexcept
        {
                auto time = *this ;
                time *= _other_ ;
                return time ;
        }

        UTI_NODISCARD constexpr duration operator/ ( duration const & _other_ ) const noexcept
        {
                auto time = *this ;
                time /= _other_ ;
                return time ;
        }

////////////////////////////////////////////////////////////////////////////////

        template< meta::time_like TimeType >
        constexpr duration & operator+= ( TimeType const & _other_ ) noexcept
        {
                nanos_ = nanos_ + _other_ ;
                return *this ;
        }

        template< meta::time_like TimeType >
        constexpr duration & operator-= ( TimeType const & _other_ ) noexcept
        {
                nanos_ = nanos_ - _other_ ;
                return *this ;
        }

        template< meta::time_like TimeType >
        constexpr duration & operator*= ( TimeType const & _other_ ) noexcept
        {
                nanos_ = nanos_ * _other_ ;
                return *this ;
        }

        template< meta::time_like TimeType >
        constexpr duration & operator/= ( TimeType const & _other_ ) noexcept
        {
                nanos_ = nanos_ / _other_ ;
                return *this ;
        }

////////////////////////////////////////////////////////////////////////////////

        template< meta::time_like TimeType >
        UTI_NODISCARD constexpr duration operator+ ( TimeType const & _other_ ) const noexcept
        {
                auto time = *this ;
                time += _other_ ;
                return time ;
        }

        template< meta::time_like TimeType >
        UTI_NODISCARD constexpr duration operator- ( TimeType const & _other_ ) const noexcept
        {
                auto time = *this ;
                time -= _other_ ;
                return time ;
        }

        template< meta::time_like TimeType >
        UTI_NODISCARD constexpr duration operator* ( TimeType const & _other_ ) const noexcept
        {
                auto time = *this ;
                time *= _other_ ;
                return time ;
        }

        template< meta::time_like TimeType >
        UTI_NODISCARD constexpr duration operator/ ( TimeType const & _other_ ) const noexcept
        {
                auto time = *this ;
                time /= _other_ ;
                return time ;
        }

////////////////////////////////////////////////////////////////////////////////

        template< uti::meta::arithmetic Scalar >
        constexpr duration & operator*= ( Scalar _scale_ ) noexcept
        {
                nanos_.val *= _scale_ ;
                return *this ;
        }

        template< uti::meta::arithmetic Scalar >
        constexpr duration & operator/= ( Scalar _scale_ ) noexcept
        {
                nanos_.val /= _scale_ ;
                return *this ;
        }

////////////////////////////////////////////////////////////////////////////////

        template< uti::meta::arithmetic Scalar >
        UTI_NODISCARD constexpr duration operator* ( Scalar _scale_ ) const noexcept
        {
                auto time = *this ;
                time *= _scale_ ;
                return time ;
        }

        template< uti::meta::arithmetic Scalar >
        UTI_NODISCARD constexpr duration operator/ ( Scalar _scale_ ) const noexcept
        {
                auto time = *this ;
                time /= _scale_ ;
                return time ;
        }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr bool operator== ( duration const & _other_ ) const noexcept
        {
                return nanos_ == _other_.nanos_ ;
        }

        UTI_NODISCARD constexpr bool operator!= ( duration const & _other_ ) const noexcept
        {
                return nanos_ != _other_.nanos_ ;
        }

        UTI_NODISCARD constexpr bool operator< ( duration const & _other_ ) const noexcept
        {
                return nanos_ < _other_.nanos_ ;
        }

        UTI_NODISCARD constexpr bool operator<= ( duration const & _other_ ) const noexcept
        {
                return nanos_ <= _other_.nanos_ ;
        }

        UTI_NODISCARD constexpr bool operator> ( duration const & _other_ ) const noexcept
        {
                return nanos_ > _other_.nanos_ ;
        }

        UTI_NODISCARD constexpr bool operator>= ( duration const & _other_ ) const noexcept
        {
                return nanos_ >= _other_.nanos_ ;
        }
private:
        nanoseconds nanos_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
