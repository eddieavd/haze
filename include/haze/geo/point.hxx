//
//
//      haze
//      geo/point.hxx
//

#pragma once

#include <haze/common/types.hxx>

#include <numeric>
#include <cmath>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T > using generic_coordinate = T ;

////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T, ssize_t D >
struct generic_point_nd
{
        using coordinate_type = generic_coordinate< T > ;
        using      value_type =         coordinate_type ;

        static constexpr ssize_t dimensions { D } ;

        static_assert( dimensions >= 1, "haze::generic_point_nd::error: number of dimensions has to be >= 1" ) ;

        uti::array< coordinate_type, dimensions > coords ;

        constexpr generic_point_nd () noexcept = default ;

        template< uti::meta::arithmetic... Args >
                requires( sizeof...( Args ) == dimensions &&
                          uti::conjunction_v< uti::is_convertible< Args, value_type >... > )
        constexpr generic_point_nd ( Args... _args_ ) noexcept
                : coords{ static_cast< value_type >( _args_ )... }
        {}

        template< typename U >
        constexpr operator generic_point_nd< U, D > () const noexcept
        {
                return [ & ]
                {
                        generic_point_nd< U, D > point ;
                        for_each( [ & ]( ssize_t idx, auto & other ){ other.coords[ idx ] = coords[ idx ] ; }, point ) ;
                        return point ;
                }() ;
        }

        UTI_NODISCARD constexpr coordinate_type & x () noexcept requires( dimensions >= 1 ) { return coords[ 0 ] ; }
        UTI_NODISCARD constexpr coordinate_type & y () noexcept requires( dimensions >= 2 ) { return coords[ 1 ] ; }
        UTI_NODISCARD constexpr coordinate_type & z () noexcept requires( dimensions >= 3 ) { return coords[ 2 ] ; }
        UTI_NODISCARD constexpr coordinate_type & w () noexcept requires( dimensions >= 4 ) { return coords[ 3 ] ; }

        UTI_NODISCARD constexpr coordinate_type const & x () const noexcept requires( dimensions >= 1 ) { return coords[ 0 ] ; }
        UTI_NODISCARD constexpr coordinate_type const & y () const noexcept requires( dimensions >= 2 ) { return coords[ 1 ] ; }
        UTI_NODISCARD constexpr coordinate_type const & z () const noexcept requires( dimensions >= 3 ) { return coords[ 2 ] ; }
        UTI_NODISCARD constexpr coordinate_type const & w () const noexcept requires( dimensions >= 4 ) { return coords[ 3 ] ; }

        UTI_NODISCARD constexpr coordinate_type & col () noexcept requires( dimensions >= 1 ) { return x() ; }
        UTI_NODISCARD constexpr coordinate_type & row () noexcept requires( dimensions >= 2 ) { return y() ; }

        UTI_NODISCARD constexpr coordinate_type const & col () const noexcept requires( dimensions >= 1 ) { return x() ; }
        UTI_NODISCARD constexpr coordinate_type const & row () const noexcept requires( dimensions >= 2 ) { return y() ; }

        template< typename Self, typename Callable, typename... Args >
                requires( uti::meta::invocable< Callable, ssize_t, Args&&... > &&
                          uti::meta::same_as< decltype( uti::declval< Callable & >()( ssize_t{}, uti::declval< Args&& >()... ) ), void > )
        constexpr void for_each ( this Self &&, Callable _fn_, Args&&... _args_ ) noexcept
        {
                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                _fn_( Idxs, UTI_FWD( _args_ )... ) ;
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;
        }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr value_type distance ( generic_point_nd const & _other_ ) const noexcept
        {
                if constexpr( dimensions == 1 )
                {
                        return _other_.x() - x() ;
                }
                if constexpr( dimensions == 2 )
                {
                        auto diff_x = _other_.x() - x() ;
                        auto diff_y = _other_.y() - y() ;
                        return std::sqrt( ( diff_x * diff_x ) + ( diff_y * diff_y ) ) ;
                }
                else
                {
                        uti::array< value_type, dimensions > diffs ;
                        for_each(
                                [ & ]( ssize_t idx, auto const & other, auto & diffs )
                                { diffs[ idx ] = other.coords[ idx ] - coords[ idx ] ; diffs[ idx ] *= diffs[ idx ] ; },
                        _other_, diffs ) ;

                        return std::sqrt( std::accumulate( diffs.begin(), diffs.end(), value_type{} ) ) ;
                }
        }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr bool operator== ( generic_point_nd const & _other_ ) const noexcept
        {
                bool eq { true } ;
                for_each( [ & ]( ssize_t idx, generic_point_nd const & other ){ if( coords[ idx ] != other.coords[ idx ] ) eq = false ; }, _other_ ) ;
                return eq ;
        }
        UTI_NODISCARD constexpr bool operator!= ( generic_point_nd const & _other_ ) const noexcept
        {
                return !operator==( _other_ ) ;
        }

////////////////////////////////////////////////////////////////////////////////

        constexpr generic_point_nd & operator+= ( generic_point_nd const & _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, generic_point_nd const & other ){ coords[ idx ] += other.coords[ idx ] ; }, _other_ ) ;
                return *this ;
        }
        constexpr generic_point_nd & operator-= ( generic_point_nd const & _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, generic_point_nd const & other ){ coords[ idx ] -= other.coords[ idx ] ; }, _other_ ) ;
                return *this ;
        }
        constexpr generic_point_nd & operator*= ( generic_point_nd const & _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, generic_point_nd const & other ){ coords[ idx ] *= other.coords[ idx ] ; }, _other_ ) ;
                return *this ;
        }
        constexpr generic_point_nd & operator/= ( generic_point_nd const & _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, generic_point_nd const & other ){ coords[ idx ] /= other.coords[ idx ] ; }, _other_ ) ;
                return *this ;
        }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr generic_point_nd operator+ ( generic_point_nd const & _other_ ) const noexcept { auto val = *this ; val += _other_ ; return val ; }
        UTI_NODISCARD constexpr generic_point_nd operator- ( generic_point_nd const & _other_ ) const noexcept { auto val = *this ; val -= _other_ ; return val ; }
        UTI_NODISCARD constexpr generic_point_nd operator* ( generic_point_nd const & _other_ ) const noexcept { auto val = *this ; val *= _other_ ; return val ; }
        UTI_NODISCARD constexpr generic_point_nd operator/ ( generic_point_nd const & _other_ ) const noexcept { auto val = *this ; val /= _other_ ; return val ; }

////////////////////////////////////////////////////////////////////////////////

        constexpr generic_point_nd & operator+= ( value_type _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, value_type other ){ coords[ idx ] += other ; }, _other_ ) ;
                return *this ;
        }
        constexpr generic_point_nd & operator-= ( value_type _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, value_type other ){ coords[ idx ] -= other ; }, _other_ ) ;
                return *this ;
        }
        constexpr generic_point_nd & operator*= ( value_type _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, value_type other ){ coords[ idx ] *= other ; }, _other_ ) ;
                return *this ;
        }
        constexpr generic_point_nd & operator/= ( value_type _other_ ) noexcept
        {
                for_each( [ & ]( ssize_t idx, value_type other ){ coords[ idx ] /= other ; }, _other_ ) ;
                return *this ;
        }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD constexpr generic_point_nd operator+ ( value_type _other_ ) const noexcept { auto val = *this ; val += _other_ ; return val ; }
        UTI_NODISCARD constexpr generic_point_nd operator- ( value_type _other_ ) const noexcept { auto val = *this ; val -= _other_ ; return val ; }
        UTI_NODISCARD constexpr generic_point_nd operator* ( value_type _other_ ) const noexcept { auto val = *this ; val *= _other_ ; return val ; }
        UTI_NODISCARD constexpr generic_point_nd operator/ ( value_type _other_ ) const noexcept { auto val = *this ; val /= _other_ ; return val ; }

////////////////////////////////////////////////////////////////////////////////

        UTI_NODISCARD friend constexpr generic_point_nd operator+ ( value_type _val_, generic_point_nd const & _point_ ) noexcept { return _point_ + _val_ ; }
        UTI_NODISCARD friend constexpr generic_point_nd operator- ( value_type _val_, generic_point_nd const & _point_ ) noexcept { return _point_ - _val_ ; }
        UTI_NODISCARD friend constexpr generic_point_nd operator* ( value_type _val_, generic_point_nd const & _point_ ) noexcept { return _point_ * _val_ ; }
        UTI_NODISCARD friend constexpr generic_point_nd operator/ ( value_type _val_, generic_point_nd const & _point_ ) noexcept { return _point_ / _val_ ; }
} ;

template< uti::meta::arithmetic T > using generic_point_1d = generic_point_nd< T, 1 > ;
template< uti::meta::arithmetic T > using generic_point_2d = generic_point_nd< T, 2 > ;
template< uti::meta::arithmetic T > using generic_point_3d = generic_point_nd< T, 3 > ;
template< uti::meta::arithmetic T > using generic_point_4d = generic_point_nd< T, 4 > ;

////////////////////////////////////////////////////////////////////////////////

using coordinate = generic_coordinate< i64_t > ;

using point_1d = generic_point_1d< i64_t > ;
using point_2d = generic_point_2d< i64_t > ;
using point_3d = generic_point_3d< i64_t > ;
using point_4d = generic_point_4d< i64_t > ;

using fcoordinate = generic_coordinate< double > ;

using fpoint_1d = generic_point_1d< double > ;
using fpoint_2d = generic_point_2d< double > ;
using fpoint_3d = generic_point_3d< double > ;
using fpoint_4d = generic_point_4d< double > ;


////////////////////////////////////////////////////////////////////////////////


} // namespace haze
