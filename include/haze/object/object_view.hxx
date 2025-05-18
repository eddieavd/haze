//
//
//      haze
//      object/object_view.hxx
//

#pragma once

#include <uti/core/iterator/reverse_iterator.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/rectangle.hxx>

#include <haze/object/meta.hxx>
#include <haze/object/pixel.hxx>
#include <haze/object/iterator.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, typename ShapeType >
class image_view ;

template< meta::pixel_like PixelType, typename ShapeType >
using const_image_view = image_view< PixelType const, ShapeType > ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, typename ShapeObject >
class image_view
{
        static_assert( meta::shape_object< ShapeObject >, "haze::image_view::error : object not a shape object" ) ;

        using _self = image_view ;
public:
        using     image_type =                       ShapeObject ;
        using     pixel_type =                       PixelType   ;
        using raw_pixel_type = uti::remove_cv_t<    pixel_type > ;
        using     value_type =                      pixel_type   ;
        using     point_type = typename image_type::point_type   ;
        using     shape_type = typename image_type::shape_type   ;
        using      size_type = typename image_type:: size_type   ;

        using               row_iterator =   shape_row_iterator< pixel_type      , _self > ;
        using         const_row_iterator =   shape_row_iterator< pixel_type const, _self > ;
        using       reverse_row_iterator = ::uti::reverse_iterator<         row_iterator > ;
        using const_reverse_row_iterator = ::uti::reverse_iterator<   const_row_iterator > ;

        using               column_iterator =   shape_column_iterator< pixel_type      , _self > ;
        using         const_column_iterator =   shape_column_iterator< pixel_type const, _self > ;
        using       reverse_column_iterator = ::uti::reverse_iterator<         column_iterator > ;
        using const_reverse_column_iterator = ::uti::reverse_iterator<   const_column_iterator > ;

        using image_pointer   = uti::add_pointer_t         < uti::copy_cv_t< pixel_type, image_type > > ;
        using image_reference = uti::add_lvalue_reference_t< uti::copy_cv_t< pixel_type, image_type > > ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        constexpr image_view (                                            ) noexcept = default ;
        constexpr image_view ( image_reference _image_, shape_type _view_ ) noexcept : view_( _view_ ), image_( &_image_ ) {}

        UTI_NODISCARD constexpr ssize_t channels () const noexcept { return pixel_type::num_channels ; }

        UTI_NODISCARD constexpr shape_type shape () const noexcept { return view_          ; }
        UTI_NODISCARD constexpr size_type  width () const noexcept { return view_. width() ; }
        UTI_NODISCARD constexpr size_type height () const noexcept { return view_.height() ; }

        UTI_NODISCARD constexpr pixel_type       & at ( point_type _point_ )       noexcept { return image_->at( _point_ + view_.begin_point() ) ; }
        UTI_NODISCARD constexpr pixel_type const & at ( point_type _point_ ) const noexcept { return image_->at( _point_ + view_.begin_point() ) ; }

        UTI_NODISCARD constexpr image_view subview ( shape_type _rect_ ) const noexcept ;

        UTI_NODISCARD constexpr       row_iterator  row_begin ()       noexcept { return       row_iterator( *this, typename       row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator  row_begin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cbegin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr       row_iterator  row_end ()       noexcept { return       row_iterator( *this, typename       row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator  row_end () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cend () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr       column_iterator  col_begin ()       noexcept { return       column_iterator( *this, typename       column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator  col_begin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cbegin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr       column_iterator  col_end ()       noexcept { return       column_iterator( *this, typename       column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator  col_end () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cend () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr       reverse_row_iterator  row_rbegin ()       noexcept { return       reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator  row_rbegin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crbegin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }

        UTI_NODISCARD constexpr       reverse_row_iterator  row_rend ()       noexcept { return       reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator  row_rend () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crend () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }

        UTI_NODISCARD constexpr       reverse_column_iterator  col_rbegin ()       noexcept { return       reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator  col_rbegin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crbegin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }

        UTI_NODISCARD constexpr       reverse_column_iterator  col_rend ()       noexcept { return       reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator  col_rend () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crend () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
private:
        shape_type     view_ ;
        image_pointer image_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
