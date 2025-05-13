//
//
//      haze
//      image/shape.hxx
//

#pragma once

#include <uti/core/iterator/reverse_iterator.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/rectangle.hxx>

#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/iterator.hxx>
#include <haze/image/image_view.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::shape_like ShapeType >
class filled_shape
{
public:
        using pixel_type =          PixelType              ;
        using shape_type =          ShapeType              ;
        using point_type = typename shape_type::point_type ;

        UTI_NODISCARD constexpr shape_type shape (                    ) const noexcept { return shape_ ; }
        UTI_NODISCARD constexpr pixel_type    at ( point_type _point_ ) const noexcept
        {
                return shape_.contains( _point_ ) ? fill_ : pixel_type{} ;
        }

        shape_type shape_ ;
        pixel_type  fill_ ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > using filled_rectangle_2d  = filled_shape< PixelType, rectangle_2d  > ;
template< meta::pixel_like PixelType > using filled_rectangle_2df = filled_shape< PixelType, rectangle_2df > ;
template< meta::pixel_like PixelType > using filled_rectangle_3d  = filled_shape< PixelType, rectangle_3d  > ;
template< meta::pixel_like PixelType > using filled_rectangle_3df = filled_shape< PixelType, rectangle_3df > ;

////////////////////////////////////////////////////////////////////////////////

// template< meta::pixel_like PixelType > using filled_circle_2d  = filled_shape< PixelType, circle_2d  > ;
// template< meta::pixel_like PixelType > using filled_circle_2df = filled_shape< PixelType, circle_2df > ;
// template< meta::pixel_like PixelType > using filled_circle_3d  = filled_shape< PixelType, circle_3d  > ;
// template< meta::pixel_like PixelType > using filled_circle_3df = filled_shape< PixelType, circle_3df > ;

////////////////////////////////////////////////////////////////////////////////

// template< meta::pixel_like PixelType > using filled_triangle_2d  = filled_shape< PixelType, triangle_2d  > ;
// template< meta::pixel_like PixelType > using filled_triangle_2df = filled_shape< PixelType, triangle_2df > ;
// template< meta::pixel_like PixelType > using filled_triangle_3d  = filled_shape< PixelType, triangle_3d  > ;
// template< meta::pixel_like PixelType > using filled_triangle_3df = filled_shape< PixelType, triangle_3df > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
