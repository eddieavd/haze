//
//
//      haze
//      geometry/solver.hxx
//

#pragma once

#include <haze/common/types.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/line.hxx>
#include <haze/geometry/rectangle.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::point_like PointType >
class solver
{
public:
        using      point_type = PointType                       ;
        using       line_type = generic_line     < point_type > ;
        using       rect_type = generic_rectangle< point_type > ;

        using coordinate_type = typename point_type::coordinate_type ;
        using       size_type =                      coordinate_type ;

        UTI_NODISCARD constexpr static double angle ( point_type _a_, point_type _b_, point_type _c_ ) noexcept ;

        UTI_NODISCARD constexpr static bool    do_intersect    ( line_type _line_1_, line_type _line_2_ ) noexcept ;
        UTI_NODISCARD constexpr static point_type intersection ( line_type _line_1_, line_type _line_2_ ) noexcept ;

        UTI_NODISCARD constexpr static point_type project ( point_type _point_, line_type _line_ ) noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
