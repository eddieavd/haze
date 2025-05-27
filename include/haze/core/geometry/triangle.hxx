//
//
//      haze
//      core/geometry/triangle.hxx
//

#pragma once

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::point_like PointType >
struct generic_triangle
{
        using      point_type =          PointType                     ;
        using coordinate_type = typename point_type::coordinate_type   ;
        using      value_type = typename point_type::     value_type   ;
        using  rectangle_type =   generic_rectangle<      point_type > ;

        static constexpr ssize_t       dimensions { point_type::dimensions } ;
        static constexpr ssize_t shape_dimensions {                      2 } ;

        point_type a_ ;
        point_type b_ ;
        point_type c_ ;

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                double d1 { sign( _point_, a_, b_ ) } ;
                double d2 { sign( _point_, b_, c_ ) } ;
                double d3 { sign( _point_, c_, a_ ) } ;

                bool has_neg { ( d1 < 0.0 ) || ( d2 < 0.0 ) || ( d3 < 0.0 ) } ;
                bool has_pos { ( d1 > 0.0 ) || ( d2 > 0.0 ) || ( d3 > 0.0 ) } ;

                return !( has_neg && has_pos ) ;
        }

        UTI_NODISCARD constexpr rectangle_type bounding_box () const noexcept
        {
                rectangle_type box {} ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                box.begin_point()[ Idxs ] = uti::min( a_[ Idxs ], b_[ Idxs ], c_[ Idxs ] ) ;
                                box.  end_point()[ Idxs ] = uti::max( a_[ Idxs ], b_[ Idxs ], c_[ Idxs ] ) ;
                        }() ) ;
                }( uti::make_index_sequence< dimensions >{} ) ;

                return box ;
        }

        UTI_NODISCARD constexpr generic_triangle normalized () const noexcept ;

        static constexpr double sign ( fpoint_2d _x_, fpoint_2d _y_, fpoint_2d _z_ ) noexcept
        {
                return ( _x_.x() - _z_.x() ) * ( _y_.y() - _z_.y() ) -
                       ( _y_.x() - _z_.x() ) * ( _x_.y() - _z_.y() ) ;
        }

        UTI_NODISCARD constexpr bool operator== ( generic_triangle const & _other_ ) const noexcept
        { return a_ == _other_.a_ && b_ == _other_.b_ && c_ == _other_.c_ ; }

        UTI_NODISCARD constexpr bool operator!= ( generic_triangle const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////

using triangle_2d  = generic_triangle<  point_2d > ;
using triangle_2df = generic_triangle< fpoint_2d > ;
using triangle_3d  = generic_triangle<  point_3d > ;
using triangle_3df = generic_triangle< fpoint_3d > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
