//
//
//      haze
//      geometry/rectangle.hxx
//

#pragma once

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::point_like PointType >
        requires( PointType::dimensions >= 2 )
struct generic_rectangle
{
        using      point_type =          PointType                   ;
        using coordinate_type = typename point_type::coordinate_type ;
        using      value_type = typename point_type::     value_type ;

        static constexpr ssize_t       dimensions { point_type::dimensions } ;
        static constexpr ssize_t shape_dimensions {                      2 } ;

        point_type begin_ ;
        point_type   end_ ;

        UTI_NODISCARD constexpr point_type       & begin_point ()       noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type const & begin_point () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type       &   end_point ()       noexcept { return   end_ ; }
        UTI_NODISCARD constexpr point_type const &   end_point () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr coordinate_type       & begin_x ()       noexcept                             { return begin_.x() ; }
        UTI_NODISCARD constexpr coordinate_type const & begin_x () const noexcept                             { return begin_.x() ; }
        UTI_NODISCARD constexpr coordinate_type       & begin_y ()       noexcept                             { return begin_.y() ; }
        UTI_NODISCARD constexpr coordinate_type const & begin_y () const noexcept                             { return begin_.y() ; }
        UTI_NODISCARD constexpr coordinate_type       & begin_z ()       noexcept requires( dimensions >= 3 ) { return begin_.z() ; }
        UTI_NODISCARD constexpr coordinate_type const & begin_z () const noexcept requires( dimensions >= 3 ) { return begin_.z() ; }

        UTI_NODISCARD constexpr coordinate_type       & end_x ()       noexcept                             { return end_.x() ; }
        UTI_NODISCARD constexpr coordinate_type const & end_x () const noexcept                             { return end_.x() ; }
        UTI_NODISCARD constexpr coordinate_type       & end_y ()       noexcept                             { return end_.y() ; }
        UTI_NODISCARD constexpr coordinate_type const & end_y () const noexcept                             { return end_.y() ; }
        UTI_NODISCARD constexpr coordinate_type       & end_z ()       noexcept requires( dimensions >= 3 ) { return end_.z() ; }
        UTI_NODISCARD constexpr coordinate_type const & end_z () const noexcept requires( dimensions >= 3 ) { return end_.z() ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                return [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        return ( ... &&
                        [ & ]
                        {
                                return begin_[ Idxs ] <= _point_[ Idxs ] && _point_[ Idxs ] <= end_[ Idxs ] ;
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;
        }

        UTI_NODISCARD constexpr generic_rectangle bounding_box () const noexcept
        {
                return { { uti::min( begin_x(), end_x() ), uti::min( begin_y(), end_y() ) } ,
                         { uti::max( begin_x(), end_x() ), uti::max( begin_y(), end_y() ) } } ;
        }

        UTI_NODISCARD constexpr generic_rectangle normalized () const noexcept
        {
                return { {}, { end_point() - begin_point() } } ;
        }

        UTI_NODISCARD constexpr value_type  width () const noexcept { return end_x() - begin_x() ; }
        UTI_NODISCARD constexpr value_type height () const noexcept { return end_y() - begin_y() ; }

        UTI_NODISCARD constexpr bool operator== ( generic_rectangle const & _other_ ) const noexcept
        { return begin_ == _other_.begin_ && end_ == _other_.end_ ; }

        UTI_NODISCARD constexpr bool operator!= ( generic_rectangle const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////

using rectangle_2d  = generic_rectangle<  point_2d > ;
using rectangle_2df = generic_rectangle< fpoint_2d > ;
using rectangle_3d  = generic_rectangle<  point_3d > ;
using rectangle_3df = generic_rectangle< fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
