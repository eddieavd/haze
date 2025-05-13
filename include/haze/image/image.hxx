//
//
//      haze
//      image/image.hxx
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

template< meta::pixel_like PixelType, meta::point_like PointType >
class generic_image
{
        using _self = generic_image< PixelType, PointType > ;
public:
        using pixel_type =          PixelType                   ;
        using point_type =          PointType                   ;
        using value_type =          pixel_type                  ;
        using  size_type = typename point_type::coordinate_type ;

        using rectangle_type = generic_rectangle< point_type > ;
        using     shape_type =         rectangle_type          ;

        using       image_view_type =       image_view< pixel_type, _self > ;
        using const_image_view_type = const_image_view< pixel_type, _self > ;

        using               row_iterator =   shape_row_iterator< pixel_type      , _self > ;
        using         const_row_iterator =   shape_row_iterator< pixel_type const, _self > ;
        using       reverse_row_iterator = ::uti::reverse_iterator<         row_iterator > ;
        using const_reverse_row_iterator = ::uti::reverse_iterator<   const_row_iterator > ;

        using               column_iterator =   shape_column_iterator< pixel_type      , _self > ;
        using         const_column_iterator =   shape_column_iterator< pixel_type const, _self > ;
        using       reverse_column_iterator = ::uti::reverse_iterator<         column_iterator > ;
        using const_reverse_column_iterator = ::uti::reverse_iterator<   const_column_iterator > ;

        using pixel_storage_type = haze::vector< pixel_type > ;

        static constexpr ssize_t       dimensions { point_type::      dimensions } ;
        static constexpr ssize_t shape_dimensions { shape_type::shape_dimensions } ;

        constexpr generic_image (                    size_type _width_, size_type _height_ ) UTI_NOEXCEPT_UNLESS_BADALLOC : shape_{ {}, { _width_, _height_ } }, pixels_( _width_ * _height_, pixel_type{} ) {}
        constexpr generic_image ( pixel_type _fill_, size_type _width_, size_type _height_ ) UTI_NOEXCEPT_UNLESS_BADALLOC : shape_{ {}, { _width_, _height_ } }, pixels_( _width_ * _height_,       _fill_ ) {}

        template< meta::image_like ImageType >
        constexpr generic_image ( ImageType const & _other_ ) UTI_NOEXCEPT_UNLESS_BADALLOC ;

        constexpr generic_image             ( generic_image const &  _other_ ) UTI_NOEXCEPT_UNLESS_BADALLOC ;
        constexpr generic_image             ( generic_image       && _other_ )     noexcept                 ;
        constexpr generic_image & operator= ( generic_image const &  _other_ ) UTI_NOEXCEPT_UNLESS_BADALLOC ;
        constexpr generic_image & operator= ( generic_image       && _other_ )     noexcept                 ;

        constexpr ~generic_image () noexcept = default ;

        UTI_NODISCARD constexpr ssize_t channels () const noexcept { return pixel_type::num_channels ; }

        UTI_NODISCARD constexpr shape_type shape () const noexcept { return shape_          ; }
        UTI_NODISCARD constexpr size_type  width () const noexcept { return shape_. width() ; }
        UTI_NODISCARD constexpr size_type height () const noexcept { return shape_.height() ; }

        UTI_NODISCARD constexpr pixel_storage_type       & pixels ()       noexcept { return pixels_ ; }
        UTI_NODISCARD constexpr pixel_storage_type const & pixels () const noexcept { return pixels_ ; }

        UTI_NODISCARD constexpr pixel_type       & at ( point_type _point_ )       noexcept { return pixels_.at( _point_.y() * width() + _point_.x() ) ; }
        UTI_NODISCARD constexpr pixel_type const & at ( point_type _point_ ) const noexcept { return pixels_.at( _point_.y() * width() + _point_.x() ) ; }

        UTI_NODISCARD constexpr       image_view_type subview ( shape_type _rect_ )       noexcept { return       image_view_type( *this, _rect_ ) ; }
        UTI_NODISCARD constexpr const_image_view_type subview ( shape_type _rect_ ) const noexcept { return const_image_view_type( *this, _rect_ ) ; }

        UTI_NODISCARD constexpr generic_image subimage ( shape_type _rect_ ) const UTI_NOEXCEPT_UNLESS_BADALLOC { return generic_image( subview( _rect_ ) ) ; }

        UTI_NODISCARD constexpr shape_type bounding_box () const noexcept { return shape_ ; }

        UTI_NODISCARD constexpr       row_iterator  row_begin ()       noexcept { return       row_iterator( *this, typename       row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator  row_begin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator crow_begin () const noexcept { return const_row_iterator( *this, typename const_row_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr       row_iterator  row_end ()       noexcept { return       row_iterator( *this, typename       row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator  row_end () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator crow_end () const noexcept { return const_row_iterator( *this, typename const_row_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr       column_iterator  col_begin ()       noexcept { return       column_iterator( *this, typename       column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator  col_begin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator ccol_begin () const noexcept { return const_column_iterator( *this, typename const_column_iterator::begin_tag{} ) ; }

        UTI_NODISCARD constexpr       column_iterator  col_end ()       noexcept { return       column_iterator( *this, typename       column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator  col_end () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator ccol_end () const noexcept { return const_column_iterator( *this, typename const_column_iterator::end_tag{} ) ; }

        UTI_NODISCARD constexpr       reverse_row_iterator  rrow_begin ()       noexcept { return       reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator  rrow_begin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator crrow_begin () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }

        UTI_NODISCARD constexpr       reverse_row_iterator  rrow_end ()       noexcept { return       reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator  rrow_end () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator crrow_end () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }

        UTI_NODISCARD constexpr       reverse_column_iterator  rcol_begin ()       noexcept { return       reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator  rcol_begin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator crcol_begin () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }

        UTI_NODISCARD constexpr       reverse_column_iterator  rcol_end ()       noexcept { return       reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator  rcol_end () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator crcol_end () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
private:
        shape_type          shape_ {} ;
        pixel_storage_type pixels_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
