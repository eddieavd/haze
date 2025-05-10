//
//
//      haze
//      geo/circle.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/geo/point.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_circle
{
        using      value_type = T                                    ;
        using      point_type = generic_point_2d< value_type >       ;
        using coordinate_type = typename point_type::coordinate_type ;

        point_type center ;
        value_type radius ;

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                return center.distance( _point_ ) <= radius ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
