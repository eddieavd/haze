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

//      using        iterator = ::uti::remove_cvref_t< decltype( uti::declval< image_reference >().    begin() ) > ;
        using    row_iterator =    row_iterator< pixel_type, _self > ;
        using column_iterator = column_iterator< pixel_type, _self > ;

//      using        reverse_iterator = ::uti::reverse_iterator<        iterator > ;
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

/*
template< meta::image_like Image >
class image_view
{
public:
        using image_type = Image ;
        using pixel_type = typename image_type::pixel_type ;
        using ssize_type = typename image_type::ssize_type ;
        using value_type = pixel_type ;

        using         pointer = value_type       * ;
        using   const_pointer = value_type const * ;
        using       reference = value_type       & ;
        using const_reference = value_type const & ;

        using               iterator = typename image_type::          iterator   ;
        using         const_iterator = typename image_type::    const_iterator   ;
        using       reverse_iterator = ::uti::reverse_iterator<       iterator > ;
        using const_reverse_iterator = ::uti::reverse_iterator< const_iterator > ;

        using               row_iterator = typename image_type::          row_iterator   ;
        using         const_row_iterator = typename image_type::    const_row_iterator   ;
        using       reverse_row_iterator = ::uti::reverse_iterator<       row_iterator > ;
        using const_reverse_row_iterator = ::uti::reverse_iterator< const_row_iterator > ;

        using               column_iterator = typename image_type::          column_iterator   ;
        using         const_column_iterator = typename image_type::    const_column_iterator   ;
        using       reverse_column_iterator = ::uti::reverse_iterator<       column_iterator > ;
        using const_reverse_column_iterator = ::uti::reverse_iterator< const_column_iterator > ;

        constexpr image_view ( image_type const & _image_ ) noexcept
                : begin_ ( _image_. begin() )
                ,   end_ ( _image_.   end() )
                , width_ ( _image_. width() )
                , height_( _image_.height() )
        {}

        constexpr image_view             ( image_view const &  ) noexcept = default ;
        constexpr image_view             ( image_view       && ) noexcept = default ;
        constexpr image_view & operator= ( image_view const &  ) noexcept = default ;
        constexpr image_view & operator= ( image_view       && ) noexcept = default ;

        constexpr ~image_view () noexcept = default ;

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) at ( this Self && self, ssize_type _row_, ssize_type _col_ ) noexcept
        { return UTI_FWD( self ).begin_[ _row_ * UTI_FWD( self ).width_ + _col_ ] ; }

        UTI_NODISCARD constexpr uti::view< value_type > row ( ssize_type _row_ ) noexcept
        { return uti::view< pixel_type >( begin_ + _row_ * width_, width_ ) ; }

        UTI_NODISCARD constexpr uti::view< value_type const > row ( ssize_type _row_ ) const noexcept
        { return uti::view< pixel_type const >( begin_ + _row_ * width_, width_ ) ; }

        UTI_NODISCARD constexpr ssize_type  width () const noexcept { return  width_ ; }
        UTI_NODISCARD constexpr ssize_type height () const noexcept { return height_ ; }

        UTI_NODISCARD constexpr       iterator  begin ()       noexcept { return begin_  ; }
        UTI_NODISCARD constexpr const_iterator  begin () const noexcept { return begin_  ; }
        UTI_NODISCARD constexpr const_iterator cbegin () const noexcept { return begin() ; }

        UTI_NODISCARD constexpr       iterator  end ()       noexcept { return end_  ; }
        UTI_NODISCARD constexpr const_iterator  end () const noexcept { return end_  ; }
        UTI_NODISCARD constexpr const_iterator cend () const noexcept { return end() ; }

        UTI_NODISCARD constexpr       reverse_iterator  rbegin ()       noexcept { return       reverse_iterator( --end() ) ; }
        UTI_NODISCARD constexpr const_reverse_iterator  rbegin () const noexcept { return const_reverse_iterator( --end() ) ; }
        UTI_NODISCARD constexpr const_reverse_iterator crbegin () const noexcept { return rbegin() ; }

        UTI_NODISCARD constexpr       reverse_iterator  rend ()       noexcept { return       reverse_iterator( --begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_iterator  rend () const noexcept { return const_reverse_iterator( --begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_iterator crend () const noexcept { return end() ; }

        UTI_NODISCARD constexpr       row_iterator row_begin  ()       noexcept { return       row_iterator( this, typename       row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_begin  () const noexcept { return const_row_iterator( this, typename const_row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cbegin () const noexcept { return row_begin() ; }

        UTI_NODISCARD constexpr       row_iterator row_end  ()       noexcept { return       row_iterator( this, typename       row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_end  () const noexcept { return const_row_iterator( this, typename const_row_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_cend () const noexcept { return row_end() ; }

        UTI_NODISCARD constexpr       reverse_row_iterator row_rbegin  ()       noexcept { return       reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_rbegin  () const noexcept { return const_reverse_row_iterator( --row_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crbegin () const noexcept { return row_rbegin() ; }

        UTI_NODISCARD constexpr       reverse_row_iterator row_rend  ()       noexcept { return       reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_rend  () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crend () const noexcept { return row_rend() ; }

        UTI_NODISCARD constexpr       column_iterator col_begin  ()       noexcept { return       column_iterator( this, typename       column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_begin  () const noexcept { return const_column_iterator( this, typename const_column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cbegin () const noexcept { return col_begin() ; }

        UTI_NODISCARD constexpr       column_iterator col_end  ()       noexcept { return       column_iterator( this, typename       column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_end  () const noexcept { return const_column_iterator( this, typename const_column_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_cend () const noexcept { return col_end() ; }

        UTI_NODISCARD constexpr       reverse_column_iterator col_rbegin  ()       noexcept { return       reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_rbegin  () const noexcept { return const_reverse_column_iterator( --col_end() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crbegin () const noexcept { return col_rbegin() ; }

        UTI_NODISCARD constexpr       reverse_column_iterator col_rend  ()       noexcept { return       reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_rend  () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crend () const noexcept { return col_rend() ; }
private:
        iterator    begin_ ;
        iterator      end_ ;
        ssize_type  width_ ;
        ssize_type height_ ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image >
class const_image_view
{
public:
        using image_type = Image ;
        using pixel_type = uti::remove_cv_t< typename image_type::pixel_type > ;
        using ssize_type =                   typename image_type::ssize_type   ;

        using         pointer = pixel_type       * ;
        using   const_pointer = pixel_type const * ;
        using       reference = pixel_type       & ;
        using const_reference = pixel_type const & ;

        using const_iterator = typename image_type::const_iterator ;

        constexpr const_image_view () noexcept = default ;

        constexpr const_image_view ( image_type const & _image_ ) noexcept
                : begin_ ( _image_.cbegin() )
                , end_   ( _image_.  cend() )
                , width_ ( _image_. width() )
                , height_( _image_.height() )
        {}

        constexpr const_image_view             ( const_image_view const &  ) noexcept = default ;
        constexpr const_image_view             ( const_image_view       && ) noexcept = default ;
        constexpr const_image_view & operator= ( const_image_view const &  ) noexcept = default ;
        constexpr const_image_view & operator= ( const_image_view       && ) noexcept = default ;

        constexpr ~const_image_view () noexcept = default ;

        UTI_NODISCARD constexpr const_reference at ( ssize_type _row_, ssize_type _col_ ) const noexcept
        { return begin_[ _row_ * width_ + _col_ ] ; }

        UTI_NODISCARD constexpr uti::view< pixel_type const > row ( ssize_type const _row_ ) const noexcept
        { return uti::view< pixel_type const >( begin_ + _row_ * width_, width_ ) ; }

        UTI_NODISCARD constexpr ssize_type  width () const noexcept { return  width_ ; }
        UTI_NODISCARD constexpr ssize_type height () const noexcept { return height_ ; }

        UTI_NODISCARD constexpr const_iterator  begin () const noexcept { return begin_  ; }
        UTI_NODISCARD constexpr const_iterator cbegin () const noexcept { return begin() ; }

        UTI_NODISCARD constexpr const_iterator  end () const noexcept { return end_  ; }
        UTI_NODISCARD constexpr const_iterator cend () const noexcept { return end() ; }
private:
        const_iterator begin_ ;
        const_iterator   end_ ;
        ssize_type     width_ ;
        ssize_type    height_ ;
} ;
*/


} // namespace haze
