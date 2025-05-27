//
//
//      haze
//      core/object/image.hxx
//

#pragma once

#include <uti/core/iterator/reverse_iterator.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>
#include <haze/core/object/iterator.hxx>
#include <haze/core/object/object_view.hxx>

#include <haze/cpugfx/draw/meta.hxx>


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

        constexpr generic_image (                    shape_type _shape_ ) UTI_NOEXCEPT_UNLESS_BADALLOC : shape_{ _shape_ }, pixels_( shape_.width() * shape_.height(), pixel_type{} ) {}
        constexpr generic_image ( pixel_type _fill_, shape_type _shape_ ) UTI_NOEXCEPT_UNLESS_BADALLOC : shape_{ _shape_ }, pixels_( shape_.width() * shape_.height(),       _fill_ ) {}

        template< meta::image_like ImageType >
        constexpr generic_image ( ImageType const & _other_ ) UTI_NOEXCEPT_UNLESS_BADALLOC
                : shape_ { _other_.shape().normalized().begin_, _other_.shape().normalized().end_ }
                , pixels_( _other_.width() * _other_.height() )
        {
                for( auto iter = _other_.row_begin(); iter < _other_.row_end(); ++iter )
                {
                        pixels_.push_back( *iter ) ;
                }
        }

        constexpr generic_image ( pixel_storage_type const & _pixels_, shape_type _shape_ ) UTI_NOEXCEPT_UNLESS_BADALLOC
                : shape_{ _shape_ }, pixels_( _pixels_ ) {}

        constexpr generic_image ( pixel_storage_type && _pixels_, shape_type _shape_ ) noexcept
                : shape_{ _shape_ }, pixels_( UTI_MOVE( _pixels_ ) ) {}

        constexpr generic_image             ( generic_image const &  _other_ ) UTI_NOEXCEPT_UNLESS_BADALLOC : shape_ { _other_.shape_ }, pixels_ (           _other_.pixels_   ) {                                      }
        constexpr generic_image             ( generic_image       && _other_ )     noexcept                 : shape_ { _other_.shape_ }, pixels_ ( UTI_MOVE( _other_.pixels_ ) ) { _other_.shape_ = {} ;                }
        constexpr generic_image & operator= ( generic_image const &  _other_ ) UTI_NOEXCEPT_UNLESS_BADALLOC { shape_ = _other_.shape_  ; pixels_ =           _other_.pixels_   ;                       ; return *this ; }
        constexpr generic_image & operator= ( generic_image       && _other_ )     noexcept                 { shape_ = _other_.shape_  ; pixels_ = UTI_MOVE( _other_.pixels_ ) ;   _other_.shape_ = {} ; return *this ; }

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

        template< meta::interpolator< pixel_type > Interpolator >
        UTI_NODISCARD constexpr generic_image scale_up ( size_type _scale_ ) const UTI_NOEXCEPT_UNLESS_BADALLOC
        {
                size_type new_width  {  width() * _scale_ } ;
                size_type new_height { height() * _scale_ } ;

                shape_type new_shape {} ;
                new_shape.end_point().x() =  new_width ;
                new_shape.end_point().y() = new_height ;

                generic_image scaled( new_shape ) ;

                for( size_type i = 0; i < new_height; ++i )
                {
                        for( size_type j = 0; j < new_width; ++j )
                        {
                                pixel_type top_left  = at( { j / _scale_    , i / _scale_     } ) ;
                                pixel_type top_right = at( { j / _scale_ + 1, i / _scale_     } ) ;
                                pixel_type bot_left  = at( { j / _scale_    , i / _scale_ + 1 } ) ;
                                pixel_type bot_right = at( { j / _scale_ + 1, i / _scale_ + 1 } ) ;

                                double dx = static_cast< double >( j ) / static_cast< double >( _scale_ ) -
                                            static_cast<  i64_t >( j ) / static_cast<  i64_t >( _scale_ ) ;

                                double dy = static_cast< double >( i ) / static_cast< double >( _scale_ ) -
                                            static_cast<  i64_t >( i ) / static_cast<  i64_t >( _scale_ ) ;

                                pixel_type top { Interpolator::template operator()< pixel_type >( top_left, top_right, 1.0, dx ) } ;
                                pixel_type bot { Interpolator::template operator()< pixel_type >( bot_left, bot_right, 1.0, dx ) } ;

                                pixel_type pix { Interpolator::template operator()< pixel_type >( top, bot, 1.0, dy ) } ;

                                scaled.at( { j, i } ) = pix ;
                        }
                }
                return scaled ;
        }

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
        shape_type          shape_ {} ;
        pixel_storage_type pixels_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType > using image_2d  = generic_image< PixelType,  point_2d > ;
template< meta::pixel_like PixelType > using image_2df = generic_image< PixelType, fpoint_2d > ;
template< meta::pixel_like PixelType > using image_3d  = generic_image< PixelType,  point_3d > ;
template< meta::pixel_like PixelType > using image_3df = generic_image< PixelType, fpoint_3d > ;

template< meta::pixel_like PixelType > using image = image_2d< PixelType > ;

using mono_image = image< mono_u8_pixel > ;
using  rgb_image = image<  rgb_u8_pixel > ;
using rgba_image = image< rgba_u8_pixel > ;
using bgra_image = image< bgra_u8_pixel > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
