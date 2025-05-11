//
//
//      haze
//      geo/rectangle.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/geo/point.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_rectangle
{
        using      value_type = T                                    ;
        using      point_type = generic_point_2d< value_type >       ;
        using coordinate_type = typename point_type::coordinate_type ;

        point_type begin_ ;
        point_type   end_ ;

        UTI_NODISCARD constexpr coordinate_type begin_x () const noexcept { return begin_.x() ; }
        UTI_NODISCARD constexpr coordinate_type begin_y () const noexcept { return begin_.y() ; }
        UTI_NODISCARD constexpr coordinate_type   end_x () const noexcept { return   end_.x() ; }
        UTI_NODISCARD constexpr coordinate_type   end_y () const noexcept { return   end_.y() ; }

        UTI_NODISCARD constexpr point_type     top_left () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type bottom_right () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr point_type   top_right () const noexcept { return point_type{   end_.x(), begin_.y() } ; }
        UTI_NODISCARD constexpr point_type bottom_left () const noexcept { return point_type{ begin_.x(),   end_.y() } ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        { return begin_.row() <= _point_.row() && _point_.row() <= end_.row() &&
                 begin_.col() <= _point_.col() && _point_.col() <= end_.col()  ; }

        UTI_NODISCARD constexpr generic_rectangle bounding_box () const noexcept { return *this ; }

        UTI_NODISCARD constexpr value_type  width () const noexcept { return static_cast< value_type >( ( end_ - begin_ ).col() ) ; }
        UTI_NODISCARD constexpr value_type height () const noexcept { return static_cast< value_type >( ( end_ - begin_ ).row() ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////

using  rectangle = generic_rectangle<  i64_t > ;
using frectangle = generic_rectangle< double > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
