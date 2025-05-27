//
//
//      haze
//      core/object/pixel.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>

#include <haze/core/object/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

enum class pixel_format
{
        Ru8  = 0 ,
        RGBu8    ,
        RGBAu8   ,
        BGRAu8   ,

        Ri8      ,
        RGBi8    ,
        RGBAi8   ,
        BGRAi8   ,

        Ri16     ,
        RGBi16   ,
        RGBAi16  ,
        BGRAi16  ,

        Ru16     ,
        RGBu16   ,
        RGBAu16  ,
        BGRAu16  ,

        Ri32     ,
        RGBi32   ,
        RGBAi32  ,
        BGRAi32  ,

        Ru32     ,
        RGBu32   ,
        RGBAu32  ,
        BGRAu32  ,

        Ri64     ,
        RGBi64   ,
        RGBAi64  ,
        BGRAi64  ,

        Ru64     ,
        RGBu64   ,
        RGBAu64  ,
        BGRAu64  ,

        R32F     ,
        RGB32F   ,
        RGBA32F  ,
        BGRA32F  ,
        COUNT    ,
} ;

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

        template< uti::meta::arithmetic U >
        UTI_NODISCARD constexpr operator generic_pixel< U, Channels > () const noexcept
        {
                generic_pixel< U, Channels > pixel ;
                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                pixel[ Idxs ] = static_cast< U >( operator[]( Idxs ) ) ;
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;
                return pixel ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T > struct  mono_generic_pixel : public generic_pixel< T, 1 > {                                                    } ;
template< uti::meta::arithmetic T > struct   rgb_generic_pixel : public generic_pixel< T, 3 > { enum { RED = 0, GREEN = 1, BLUE = 2            } ; } ;
template< uti::meta::arithmetic T > struct  rgba_generic_pixel : public generic_pixel< T, 4 > { enum { RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3 } ; } ;
template< uti::meta::arithmetic T > struct  bgra_generic_pixel : public generic_pixel< T, 4 > { enum { RED = 2, GREEN = 1, BLUE = 0, ALPHA = 3 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_u8_pixel : public mono_generic_pixel< u8_t > { static constexpr pixel_format format { pixel_format::   Ru8 } ; } ;
struct  rgb_u8_pixel : public  rgb_generic_pixel< u8_t > { static constexpr pixel_format format { pixel_format:: RGBu8 } ; } ;
struct rgba_u8_pixel : public rgba_generic_pixel< u8_t > { static constexpr pixel_format format { pixel_format::RGBAu8 } ; } ;
struct bgra_u8_pixel : public bgra_generic_pixel< u8_t > { static constexpr pixel_format format { pixel_format::BGRAu8 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_i8_pixel : public mono_generic_pixel< i8_t > { static constexpr pixel_format format { pixel_format::   Ri8 } ; } ;
struct  rgb_i8_pixel : public  rgb_generic_pixel< i8_t > { static constexpr pixel_format format { pixel_format:: RGBi8 } ; } ;
struct rgba_i8_pixel : public rgba_generic_pixel< i8_t > { static constexpr pixel_format format { pixel_format::RGBAi8 } ; } ;
struct bgra_i8_pixel : public bgra_generic_pixel< i8_t > { static constexpr pixel_format format { pixel_format::BGRAi8 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_u16_pixel : public mono_generic_pixel< u16_t > { static constexpr pixel_format format { pixel_format::   Ru16 } ; } ;
struct  rgb_u16_pixel : public  rgb_generic_pixel< u16_t > { static constexpr pixel_format format { pixel_format:: RGBu16 } ; } ;
struct rgba_u16_pixel : public rgba_generic_pixel< u16_t > { static constexpr pixel_format format { pixel_format::RGBAu16 } ; } ;
struct bgra_u16_pixel : public bgra_generic_pixel< u16_t > { static constexpr pixel_format format { pixel_format::BGRAu16 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_i16_pixel : public mono_generic_pixel< i16_t > { static constexpr pixel_format format { pixel_format::   Ri16 } ; } ;
struct  rgb_i16_pixel : public  rgb_generic_pixel< i16_t > { static constexpr pixel_format format { pixel_format:: RGBi16 } ; } ;
struct rgba_i16_pixel : public rgba_generic_pixel< i16_t > { static constexpr pixel_format format { pixel_format::RGBAi16 } ; } ;
struct bgra_i16_pixel : public bgra_generic_pixel< i16_t > { static constexpr pixel_format format { pixel_format::BGRAi16 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_u32_pixel : public mono_generic_pixel< u32_t > { static constexpr pixel_format format { pixel_format::   Ru32 } ; } ;
struct  rgb_u32_pixel : public  rgb_generic_pixel< u32_t > { static constexpr pixel_format format { pixel_format:: RGBu32 } ; } ;
struct rgba_u32_pixel : public rgba_generic_pixel< u32_t > { static constexpr pixel_format format { pixel_format::RGBAu32 } ; } ;
struct bgra_u32_pixel : public bgra_generic_pixel< u32_t > { static constexpr pixel_format format { pixel_format::BGRAu32 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_i32_pixel : public mono_generic_pixel< i32_t > { static constexpr pixel_format format { pixel_format::   Ri32 } ; } ;
struct  rgb_i32_pixel : public  rgb_generic_pixel< i32_t > { static constexpr pixel_format format { pixel_format:: RGBi32 } ; } ;
struct rgba_i32_pixel : public rgba_generic_pixel< i32_t > { static constexpr pixel_format format { pixel_format::RGBAi32 } ; } ;
struct bgra_i32_pixel : public bgra_generic_pixel< i32_t > { static constexpr pixel_format format { pixel_format::BGRAi32 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_u64_pixel : public mono_generic_pixel< u64_t > { static constexpr pixel_format format { pixel_format::   Ru64 } ; } ;
struct  rgb_u64_pixel : public  rgb_generic_pixel< u64_t > { static constexpr pixel_format format { pixel_format:: RGBu64 } ; } ;
struct rgba_u64_pixel : public rgba_generic_pixel< u64_t > { static constexpr pixel_format format { pixel_format::RGBAu64 } ; } ;
struct bgra_u64_pixel : public bgra_generic_pixel< u64_t > { static constexpr pixel_format format { pixel_format::BGRAu64 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_i64_pixel : public mono_generic_pixel< i64_t > { static constexpr pixel_format format { pixel_format::   Ri64 } ; } ;
struct  rgb_i64_pixel : public  rgb_generic_pixel< i64_t > { static constexpr pixel_format format { pixel_format:: RGBi64 } ; } ;
struct rgba_i64_pixel : public rgba_generic_pixel< i64_t > { static constexpr pixel_format format { pixel_format::RGBAi64 } ; } ;
struct bgra_i64_pixel : public bgra_generic_pixel< i64_t > { static constexpr pixel_format format { pixel_format::BGRAi64 } ; } ;

////////////////////////////////////////////////////////////////////////////////

struct mono_f_pixel : public mono_generic_pixel< float > { static constexpr pixel_format format { pixel_format::   R32F } ; } ;
struct  rgb_f_pixel : public  rgb_generic_pixel< float > { static constexpr pixel_format format { pixel_format:: RGB32F } ; } ;
struct rgba_f_pixel : public rgba_generic_pixel< float > { static constexpr pixel_format format { pixel_format::RGBA32F } ; } ;
struct bgra_f_pixel : public bgra_generic_pixel< float > { static constexpr pixel_format format { pixel_format::BGRA32F } ; } ;

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
