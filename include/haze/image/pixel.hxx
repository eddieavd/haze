//
//
//      haze
//      image/pixel.hxx
//

#pragma once

#include <haze/common/types.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>

#include <haze/image/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T, ssize_t Channels >
struct generic_pixel
{
        using value_type = T ;

        static constexpr ssize_t   dimensions { Channels } ;
        static constexpr ssize_t num_channels { Channels } ;

        value_type channels[ num_channels ] ;

        UTI_NODISCARD constexpr value_type       & operator[] ( ssize_t _idx_ )       noexcept { return channels[ _idx_ ] ; }
        UTI_NODISCARD constexpr value_type const & operator[] ( ssize_t _idx_ ) const noexcept { return channels[ _idx_ ] ; }
} ;

////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T > struct  mono_generic_pixel : public generic_pixel< T, 1 > {                                                    } ;
template< uti::meta::arithmetic T > struct   rgb_generic_pixel : public generic_pixel< T, 3 > { enum { RED = 0, GREEN = 1, BLUE = 2            } ; } ;
template< uti::meta::arithmetic T > struct  rgba_generic_pixel : public generic_pixel< T, 4 > { enum { RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3 } ; } ;
template< uti::meta::arithmetic T > struct  bgra_generic_pixel : public generic_pixel< T, 4 > { enum { RED = 2, GREEN = 1, BLUE = 0, ALPHA = 3 } ; } ;

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
        [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
        {
                ( ... ,
                [ & ]
                {
                        _lhs_.channels[ Idxs ] += _rhs_.channels[ Idxs ] ;
                }() ) ;
        }( uti::make_index_sequence< PixelType::num_channels >{} ) ;

        return _lhs_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator-= ( PixelType & _lhs_, PixelType _rhs_ ) noexcept
{
        [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
        {
                ( ... ,
                [ & ]
                {
                        _lhs_.channels[ Idxs ] -= _rhs_.channels[ Idxs ] ;
                }() ) ;
        }( uti::make_index_sequence< PixelType::num_channels >{} ) ;

        return _lhs_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator*= ( PixelType & _pixel_, ssize_t _scale_ ) noexcept
{
        [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
        {
                ( ... ,
                [ & ]
                {
                        _pixel_.channels[ Idxs ] *= _scale_ ;
                }() ) ;
        }( uti::make_index_sequence< PixelType::num_channels >{} ) ;

        return _pixel_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr PixelType & operator/= ( PixelType & _pixel_, ssize_t _scale_ ) noexcept
{
        [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
        {
                ( ... ,
                [ & ]
                {
                        _pixel_.channels[ Idxs ] /= _scale_ ;
                }() ) ;
        }( uti::make_index_sequence< PixelType::num_channels >{} ) ;

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
