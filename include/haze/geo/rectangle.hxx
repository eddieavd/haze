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

        UTI_NODISCARD constexpr coordinate_type begin_row () const noexcept { return begin_.row() ; }
        UTI_NODISCARD constexpr coordinate_type begin_col () const noexcept { return begin_.col() ; }
        UTI_NODISCARD constexpr coordinate_type   end_row () const noexcept { return   end_.row() ; }
        UTI_NODISCARD constexpr coordinate_type   end_col () const noexcept { return   end_.col() ; }

        UTI_NODISCARD constexpr point_type     top_left () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type bottom_right () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr point_type   top_right () const noexcept { return point_type{ begin_.row(),   end_.col() } ; }
        UTI_NODISCARD constexpr point_type bottom_left () const noexcept { return point_type{   end_.row(), begin_.col() } ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        { return begin_.row() <= _point_.row() && _point_.row() <= end_.row() &&
                 begin_.col() <= _point_.col() && _point_.col() <= end_.col()  ; }

        UTI_NODISCARD constexpr value_type  width () const noexcept { return static_cast< value_type >( ( end_ - begin_ ).col() ) ; }
        UTI_NODISCARD constexpr value_type height () const noexcept { return static_cast< value_type >( ( end_ - begin_ ).row() ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////

using  rectangle = generic_rectangle<  i64_t > ;
using frectangle = generic_rectangle< double > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
