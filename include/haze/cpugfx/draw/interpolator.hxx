//
//
//      haze
//      cpugfx/draw/interpolator.hxx
//

#pragma once

#include <haze/core/common/types.hxx>
#include <haze/core/common/time.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/object/meta.hxx>

#include <haze/cpugfx/draw/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

class null_interpolator
{
public:
        template< meta::point_like PointType >
        UTI_NODISCARD static constexpr PointType operator() ( PointType, PointType _end_, duration, duration ) noexcept { return _end_ ; }
} ;

////////////////////////////////////////////////////////////////////////////////

class nearest_neighbor_interpolator
{
public:
        template< meta::point_like PointType >
        UTI_NODISCARD static constexpr PointType operator() ( PointType _start_, PointType _end_, duration _length_, duration _dt_ ) noexcept
        {
                static constexpr ssize_t dimensions { PointType::dimensions } ;

                PointType result {} ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                if( _dt_ <= _length_ / 2 )
                                {
                                        result[ Idxs ] = _start_[ Idxs ] ;
                                }
                                else
                                {
                                        result[ Idxs ] = _end_[ Idxs ] ;
                                }
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;

                return result ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////

class linear_interpolator
{
public:
        template< meta::point_like PointType >
        UTI_NODISCARD static constexpr PointType operator() ( PointType _start_, PointType _end_, duration _length_, duration _dt_ ) noexcept
        {
                static constexpr ssize_t dimensions { PointType::dimensions } ;

                PointType result {} ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                double diff = _end_[ Idxs ] - _start_[ Idxs ] ;
                                diff *= ( static_cast< double >( _dt_.to_nanos().val ) / static_cast< double >( _length_.to_nanos().val ) ) ;
                                result[ Idxs ] = _start_[ Idxs ] + diff ;
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;

                return result ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
