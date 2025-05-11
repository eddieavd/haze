//
//
//      haze
//      geo/line.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/geo/point.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_line
{
        using      value_type = T                                    ;
        using      point_type = generic_point_2d< value_type >       ;
        using     fpoint_type = fpoint_2d                            ;
        using coordinate_type = typename point_type::coordinate_type ;

        point_type begin_ ;
        point_type   end_ ;

        UTI_NODISCARD constexpr point_type begin_point () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type   end_point () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr coordinate_type begin_x () const noexcept { return begin_.x() ; }
        UTI_NODISCARD constexpr coordinate_type begin_y () const noexcept { return begin_.y() ; }
        UTI_NODISCARD constexpr coordinate_type   end_x () const noexcept { return   end_.x() ; }
        UTI_NODISCARD constexpr coordinate_type   end_y () const noexcept { return   end_.y() ; }

        UTI_NODISCARD constexpr value_type length () const noexcept { return begin_.distance( end_ ) ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                if( _point_ == begin_ || _point_ == end_ ) return true ;

                fpoint_type begin( begin_ ) ;
                fpoint_type   end(   end_ ) ;

                double dist_diff = begin.distance( _point_ ) + end.distance( _point_ ) - begin.distance( end ) ;
                return -1.0 <= dist_diff && dist_diff <= 1.0 ;
        }
} ;

using  line = generic_line<  i64_t > ;
using fline = generic_line< double > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
