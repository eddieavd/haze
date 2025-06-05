//
//
//      haze
//      core/object/shape.hxx
//

#pragma once

#include <uti/core/iterator/reverse_iterator.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>
#include <haze/core/geometry/triangle.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>
#include <haze/core/object/iterator.hxx>
#include <haze/core/object/object_view.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::shape_like ShapeType >
class filled_shape
{
        using _self = filled_shape ;
public:
        using pixel_type =          PixelType              ;
        using shape_type =          ShapeType              ;
        using point_type = typename shape_type::point_type ;

        using         const_row_iterator =   shape_row_iterator< pixel_type const, _self > ;
        using const_reverse_row_iterator = ::uti::reverse_iterator<   const_row_iterator > ;

        using         const_column_iterator =   shape_column_iterator< pixel_type const, _self > ;
        using const_reverse_column_iterator = ::uti::reverse_iterator<   const_column_iterator > ;

        UTI_NODISCARD constexpr shape_type shape (                    ) const noexcept { return shape_ ; }
        UTI_NODISCARD constexpr pixel_type    at ( point_type _point_ ) const noexcept
        {
                return shape_.contains( _point_ + shape_.bounding_box().begin_point() ) ? fill_ : pixel_type{} ;
        }

        UTI_NODISCARD constexpr pixel_type       & fill ()       noexcept { return fill_ ; }
        UTI_NODISCARD constexpr pixel_type const & fill () const noexcept { return fill_ ; }

        UTI_NODISCARD constexpr const_row_iterator  row_begin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cbegin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr const_row_iterator  row_end () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cend () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr const_column_iterator  col_begin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cbegin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr const_column_iterator  col_end () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cend () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr const_reverse_row_iterator  row_rbegin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crbegin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }

        UTI_NODISCARD constexpr const_reverse_row_iterator  row_rend () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crend () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }

        UTI_NODISCARD constexpr const_reverse_column_iterator  col_rbegin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crbegin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }

        UTI_NODISCARD constexpr const_reverse_column_iterator  col_rend () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crend () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }

        shape_type shape_ ;
        pixel_type  fill_ ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::shape_like ShapeType >
class colored_shape
{
        using _self = colored_shape ;
public:
        using pixel_type =          PixelType              ;
        using shape_type =          ShapeType              ;
        using point_type = typename shape_type::point_type ;

        using         const_row_iterator =   shape_row_iterator< pixel_type const, _self > ;
        using const_reverse_row_iterator = ::uti::reverse_iterator<   const_row_iterator > ;

        using         const_column_iterator =   shape_column_iterator< pixel_type const, _self > ;
        using const_reverse_column_iterator = ::uti::reverse_iterator<   const_column_iterator > ;

        UTI_NODISCARD constexpr shape_type shape (                    ) const noexcept { return shape_ ; }
//      UTI_NODISCARD constexpr pixel_type    at ( point_type _point_ ) const noexcept
//      {
//              return shape_.contains( _point_ + shape_.bounding_box().begin_point() ) ? fill_ : pixel_type{} ;
//      }

//      UTI_NODISCARD constexpr pixel_type       & fill ()       noexcept { return fill_ ; }
//      UTI_NODISCARD constexpr pixel_type const & fill () const noexcept { return fill_ ; }

        UTI_NODISCARD constexpr const_row_iterator  row_begin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cbegin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr const_row_iterator  row_end () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cend () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr const_column_iterator  col_begin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cbegin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr const_column_iterator  col_end () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cend () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr const_reverse_row_iterator  row_rbegin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crbegin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }

        UTI_NODISCARD constexpr const_reverse_row_iterator  row_rend () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crend () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }

        UTI_NODISCARD constexpr const_reverse_column_iterator  col_rbegin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crbegin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }

        UTI_NODISCARD constexpr const_reverse_column_iterator  col_rend () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crend () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }

        shape_type  shape_ ;
        pixel_type  fills_ [ shape_type::vertex_count ] ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > using filled_rectangle_2d  = filled_shape< PixelType, rectangle_2d  > ;
template< meta::pixel_like PixelType > using filled_rectangle_2df = filled_shape< PixelType, rectangle_2df > ;
template< meta::pixel_like PixelType > using filled_rectangle_3d  = filled_shape< PixelType, rectangle_3d  > ;
template< meta::pixel_like PixelType > using filled_rectangle_3df = filled_shape< PixelType, rectangle_3df > ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > using filled_triangle_2d  = filled_shape< PixelType, triangle_2d  > ;
template< meta::pixel_like PixelType > using filled_triangle_2df = filled_shape< PixelType, triangle_2df > ;
template< meta::pixel_like PixelType > using filled_triangle_3d  = filled_shape< PixelType, triangle_3d  > ;
template< meta::pixel_like PixelType > using filled_triangle_3df = filled_shape< PixelType, triangle_3df > ;

////////////////////////////////////////////////////////////////////////////////

// template< meta::pixel_like PixelType > using filled_circle_2d  = filled_shape< PixelType, circle_2d  > ;
// template< meta::pixel_like PixelType > using filled_circle_2df = filled_shape< PixelType, circle_2df > ;
// template< meta::pixel_like PixelType > using filled_circle_3d  = filled_shape< PixelType, circle_3d  > ;
// template< meta::pixel_like PixelType > using filled_circle_3df = filled_shape< PixelType, circle_3df > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
