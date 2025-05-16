//
//
//      haze
//      geometry/line.hxx
//

#pragma once

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/rectangle.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::point_like PointType >
struct generic_line
{
        using      point_type =          PointType                     ;
        using coordinate_type = typename point_type::coordinate_type   ;
        using      value_type = typename point_type::     value_type   ;
        using  rectangle_type =   generic_rectangle<      point_type > ;

        static constexpr ssize_t       dimensions { point_type::dimensions } ;
        static constexpr ssize_t shape_dimensions {                      1 } ;

        point_type begin_ ;
        point_type   end_ ;

        UTI_NODISCARD constexpr point_type       & begin_point ()       noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type const & begin_point () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type       &   end_point ()       noexcept { return   end_ ; }
        UTI_NODISCARD constexpr point_type const &   end_point () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr coordinate_type       & begin_x ()       noexcept                             { return begin_.x() ; }
        UTI_NODISCARD constexpr coordinate_type const & begin_x () const noexcept                             { return begin_.x() ; }
        UTI_NODISCARD constexpr coordinate_type       & begin_y ()       noexcept requires( dimensions >= 2 ) { return begin_.y() ; }
        UTI_NODISCARD constexpr coordinate_type const & begin_y () const noexcept requires( dimensions >= 2 ) { return begin_.y() ; }
        UTI_NODISCARD constexpr coordinate_type       & begin_z ()       noexcept requires( dimensions >= 3 ) { return begin_.z() ; }
        UTI_NODISCARD constexpr coordinate_type const & begin_z () const noexcept requires( dimensions >= 3 ) { return begin_.z() ; }

        UTI_NODISCARD constexpr coordinate_type       & end_x ()       noexcept                             { return end_.x() ; }
        UTI_NODISCARD constexpr coordinate_type const & end_x () const noexcept                             { return end_.x() ; }
        UTI_NODISCARD constexpr coordinate_type       & end_y ()       noexcept requires( dimensions >= 2 ) { return end_.y() ; }
        UTI_NODISCARD constexpr coordinate_type const & end_y () const noexcept requires( dimensions >= 2 ) { return end_.y() ; }
        UTI_NODISCARD constexpr coordinate_type       & end_z ()       noexcept requires( dimensions >= 3 ) { return end_.z() ; }
        UTI_NODISCARD constexpr coordinate_type const & end_z () const noexcept requires( dimensions >= 3 ) { return end_.z() ; }

        UTI_NODISCARD constexpr value_type length () const noexcept { return begin_.distance( end_ ) ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                if( _point_ == begin_ || _point_ == end_ ) return true ;

                value_type dist_diff = begin_.distance( _point_ ) + end_.distance( _point_ ) - begin_.distance( end_ ) ;
                return dist_diff == value_type( 0 ) ;
        }

        UTI_NODISCARD constexpr rectangle_type bounding_box () const noexcept
        {
                return begin_ < end_ ? rectangle_type{ begin_,   end_ }
                                     : rectangle_type{   end_, begin_ } ;
        }

        UTI_NODISCARD constexpr generic_line normalized () const noexcept
        {
                return { {}, { end_point() - begin_point() } } ;
        }

        UTI_NODISCARD constexpr bool operator== ( generic_line const & _other_ ) const noexcept
        { return begin_ == _other_.begin_ && end_ == _other_.end_ ; }

        UTI_NODISCARD constexpr bool operator!= ( generic_line const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////

using line_2d  = generic_line<  point_2d > ;
using line_2df = generic_line< fpoint_2d > ;
using line_3d  = generic_line<  point_3d > ;
using line_3df = generic_line< fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
