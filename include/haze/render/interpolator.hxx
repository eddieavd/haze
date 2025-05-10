//
//
//      haze
//      render/interpolator.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/geo/meta.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

class linear_interpolator
{
public:
        template< meta::point_like PointType >
        static constexpr PointType interpolate ( PointType _begin_, PointType _end_, ssize_t _step_count_, ssize_t _step_ ) noexcept
        {
                static constexpr ssize_t dimensions { PointType::dimensions } ;

                PointType result {} ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                double diff = _end_.coords[ Idxs ] - _begin_.coords[ Idxs ] ;
                                diff /= _step_count_ ;
                                diff *= _step_ ;
                                result.coords[ Idxs ] = _begin_.coords[ Idxs ] + diff ;
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;

                return result ;
        }

        template< meta::pixel_like PixelType >
        static constexpr PixelType interpolate ( PixelType _begin_, PixelType _end_, ssize_t _step_count_, ssize_t _step_ ) noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
