//
//
//      haze
//      image/image_view.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/common/range.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image_iterator.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, typename ImageType >
class image_view ;

template< meta::pixel_like PixelType, typename ImageType >
        requires( uti::is_const_v< PixelType > )
using const_image_view = image_view< PixelType, ImageType > ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, typename ImageType >
class image_view
{
        using _self = image_view< PixelType, ImageType > ;
public:
        using     pixel_type = PixelType                       ;
        using     image_type = ImageType                       ;
        using raw_pixel_type = uti::remove_cv_t<  pixel_type > ;
        using     ssize_type = ssize_t    ;
        using     value_type = pixel_type ;

        using image_pointer   = uti::add_pointer_t         < uti::copy_cv_t< pixel_type, image_type > > ;
        using image_reference = uti::add_lvalue_reference_t< uti::copy_cv_t< pixel_type, image_type > > ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        using    row_iterator =    row_iterator< pixel_type, _self > ;
        using column_iterator = column_iterator< pixel_type, _self > ;

        using    reverse_row_iterator = ::uti::reverse_iterator<    row_iterator > ;
        using reverse_column_iterator = ::uti::reverse_iterator< column_iterator > ;

        using    image_row_iterator = typename image_type::   row_iterator ;
        using image_column_iterator = typename image_type::column_iterator ;

        using         row_range = uti::range<         row_iterator > ;
        using reverse_row_range = uti::range< reverse_row_iterator > ;

        using         column_range = uti::range<         column_iterator > ;
        using reverse_column_range = uti::range< reverse_column_iterator > ;

        constexpr image_view ( image_pointer _image_, ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) noexcept
                : image_( _image_ )
                , begin_( this, _row_           , _col_           )
                , end_  ( this, _row_ + _height_, _col_ + _width_ )
        {}

        constexpr image_view             ( image_view const &  ) noexcept = default ;
        constexpr image_view             ( image_view       && ) noexcept = default ;
        constexpr image_view & operator= ( image_view const &  ) noexcept = default ;
        constexpr image_view & operator= ( image_view       && ) noexcept = default ;

        constexpr ~image_view () noexcept = default ;

        UTI_NODISCARD constexpr reference at ( ssize_type _row_, ssize_type _col_ ) const noexcept
        {
                return image_->at( begin_.row() + _row_, begin_.col() + _col_ ) ;
        }

        UTI_NODISCARD constexpr row_range row ( ssize_type _row_ ) const noexcept
        {
                return row_range( row_iterator( this, begin_.row() + _row_    , begin_.col() ) ,
                                  row_iterator( this, begin_.row() + _row_ + 1, begin_.col() ) ) ;
        }

        UTI_NODISCARD constexpr column_range col ( ssize_type _col_ ) const noexcept
        {
                return column_range( column_iterator( this, begin_.row(), begin_.col() + _col_     ) ,
                                     column_iterator( this, begin_.row(), begin_.col() + _col_ + 1 ) ) ;
        }

        UTI_NODISCARD constexpr ssize_type  width () const noexcept { return end_.col() - begin_.col() ; }
        UTI_NODISCARD constexpr ssize_type height () const noexcept { return end_.row() - begin_.row() ; }

        UTI_NODISCARD constexpr row_iterator row_begin ()       noexcept { return row_iterator( this, typename row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr row_iterator row_begin () const noexcept { return row_iterator( this, typename row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr row_iterator row_end   ()       noexcept { return row_iterator( this, typename row_iterator::  end_tag{} ) ; }
        UTI_NODISCARD constexpr row_iterator row_end   () const noexcept { return row_iterator( this, typename row_iterator::  end_tag{} ) ; }

        UTI_NODISCARD constexpr column_iterator col_begin ()       noexcept { return column_iterator( this, typename column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr column_iterator col_begin () const noexcept { return column_iterator( this, typename column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr column_iterator col_end   ()       noexcept { return column_iterator( this, typename column_iterator::  end_tag{} ) ; }
        UTI_NODISCARD constexpr column_iterator col_end   () const noexcept { return column_iterator( this, typename column_iterator::  end_tag{} ) ; }

        UTI_NODISCARD constexpr row_iterator  begin ()       noexcept { return row_begin() ; }
        UTI_NODISCARD constexpr row_iterator  begin () const noexcept { return row_begin() ; }
        UTI_NODISCARD constexpr row_iterator cbegin () const noexcept { return    cbegin() ; }

        UTI_NODISCARD constexpr row_iterator  end ()       noexcept { return row_end() ; }
        UTI_NODISCARD constexpr row_iterator  end () const noexcept { return row_end() ; }
        UTI_NODISCARD constexpr row_iterator cend () const noexcept { return    cend() ; }
private:
        image_pointer image_ ;
        row_iterator  begin_ ;
        row_iterator    end_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
