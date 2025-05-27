//
//
//      haze
//      core/geometry/circle.hxx
//

#pragma once

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::point_like PointType >
struct generic_circle
{
        using      point_type =          PointType                     ;
        using coordinate_type = typename point_type::coordinate_type   ;
        using      value_type = typename point_type::     value_type   ;
        using  rectangle_type =   generic_rectangle<      point_type > ;

        static constexpr ssize_t       dimensions { point_type::dimensions } ;
        static constexpr ssize_t shape_dimensions {                      2 } ;

        point_type      center_ ;
        coordinate_type radius_ ;

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                return _point_.distance( center_ ) <= radius_ ;
        }

        UTI_NODISCARD constexpr rectangle_type bounding_box () const noexcept
        {
                return rectangle_type{ { center_ - radius_ } ,
                                       { center_ + radius_ } } ;
        }

        UTI_NODISCARD constexpr generic_circle normalized () const noexcept ;

        UTI_NODISCARD constexpr bool operator== ( generic_circle const & _other_ ) const noexcept
        { return center_ == _other_.center_ && radius_ == _other_.radius_ ; }

        UTI_NODISCARD constexpr bool operator!= ( generic_circle const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////

using circle_2d  = generic_circle<  point_2d > ;
using circle_2df = generic_circle< fpoint_2d > ;
using circle_3d  = generic_circle<  point_3d > ;
using circle_3df = generic_circle< fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
