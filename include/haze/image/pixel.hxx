//
//
//      haze
//      image/pixel.hxx
//

#pragma once

#include <uti/core/type/traits.hxx>
#include <uti/core/meta/concepts.hxx>

#include <haze/common/types.hxx>


namespace haze
{


template< uti::meta::arithmetic T, ssize_t Channels >
struct generic_pixel
{
        static constexpr ssize_t num_channels { Channels } ;

        T channels[ num_channels ] ;
} ;

template< uti::meta::arithmetic T > struct mono_generic_pixel : public generic_pixel< T, 1 > {                                                    } ;
template< uti::meta::arithmetic T > struct  rgb_generic_pixel : public generic_pixel< T, 3 > { enum { RED = 0, GREEN = 1, BLUE = 2            } ; } ;
template< uti::meta::arithmetic T > struct rgba_generic_pixel : public generic_pixel< T, 4 > { enum { RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3 } ; } ;
template< uti::meta::arithmetic T > struct bgra_generic_pixel : public generic_pixel< T, 4 > { enum { RED = 2, GREEN = 1, BLUE = 0, ALPHA = 3 } ; } ;

////////////////////////////////////////////////////////////////////////////////

namespace meta
{

template< typename PixelType >
concept pixel_like = requires( PixelType pixel )
{
        { PixelType::num_channels } -> uti::meta::convertible_to< ssize_t > ;
        { pixel.channels[ 0 ]     } ;
        uti::meta::arithmetic< uti::remove_reference_t< decltype( pixel.channels[ 0 ] ) > > ;
} ;

}

////////////////////////////////////////////////////////////////////////////////

using mono_pixel = mono_generic_pixel< u8_t > ;
using  rgb_pixel =  rgb_generic_pixel< u8_t > ;
using rgba_pixel = rgba_generic_pixel< u8_t > ;
using bgra_pixel = bgra_generic_pixel< u8_t > ;

////////////////////////////////////////////////////////////////////////////////

using mono_thicc_pixel = mono_generic_pixel< i64_t > ;
using  rgb_thicc_pixel =  rgb_generic_pixel< i64_t > ;
using rgba_thicc_pixel = rgba_generic_pixel< i64_t > ;
using bgra_thicc_pixel = bgra_generic_pixel< i64_t > ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator+= ( PixelType & _lhs_, PixelType _rhs_ ) noexcept
{
        for( ssize_t i = 0; i < PixelType::num_channels; ++i )
        {
                _lhs_.channels[ i ] += _rhs_.channels[ i ] ;
        }
        return _lhs_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator-= ( PixelType & _lhs_, PixelType _rhs_ ) noexcept
{
        for( ssize_t i = 0; i < PixelType::num_channels; ++i )
        {
                _lhs_.channels[ i ] -= _rhs_.channels[ i ] ;
        }
        return _lhs_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator*= ( PixelType & _pixel_, ssize_t _scale_ ) noexcept
{
        for( ssize_t i = 0; i < PixelType::num_channels; ++i )
        {
                _pixel_.channels[ i ] *= _scale_ ;
        }
        return _pixel_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator/= ( PixelType & _pixel_, ssize_t _scale_ ) noexcept
{
        for( ssize_t i = 0; i < PixelType::num_channels; ++i )
        {
                _pixel_.channels[ i ] /= _scale_ ;
        }
        return _pixel_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType operator+ ( PixelType _lhs_, PixelType _rhs_ ) noexcept
{
        auto sum = _lhs_ ;
        sum += _rhs_ ;
        return sum ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType operator- ( PixelType _lhs_, PixelType _rhs_ ) noexcept
{
        auto diff = _lhs_ ;
        diff -= _rhs_ ;
        return diff ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType operator* ( PixelType _pixel_, ssize_t _scale_ ) noexcept
{
        auto scaled = _pixel_ ;
        scaled *= _scale_ ;
        return scaled ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType operator/ ( PixelType _pixel_, ssize_t _scale_ ) noexcept
{
        auto scaled = _pixel_ ;
        scaled /= _scale_ ;
        return scaled ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
