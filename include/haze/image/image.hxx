//
//
//      haze
//      image/image.hxx
//

#pragma once

#include <haze/common/types.hxx>

#include <haze/geo/meta.hxx>
#include <haze/geo/point.hxx>
#include <haze/geo/rectangle.hxx>

#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image_iterator.hxx>
#include <haze/image/image_view.hxx>

#include <uti/core/container/vector.hxx>
//#include <uti/core/container/range.hxx>
#include <haze/common/range.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
class image
{
        using _self = image< PixelType > ;
public:
        using pixel_type    = PixelType            ;
        using value_type    = pixel_type           ;
        using pixel_storage = vector< pixel_type > ;

        using ssize_type = ssize_t ;

        using shape_type = generic_rectangle< ssize_type > ;
        using point_type = typename shape_type::point_type ;

        using       iterator = typename pixel_storage::      iterator ;
        using const_iterator = typename pixel_storage::const_iterator ;

        using               row_iterator = ::haze::   row_iterator< pixel_type      , _self > ;
        using         const_row_iterator = ::haze::   row_iterator< pixel_type const, _self > ;
        using       reverse_row_iterator = ::uti::reverse_iterator<            row_iterator > ;
        using const_reverse_row_iterator = ::uti::reverse_iterator<      const_row_iterator > ;

        using               column_iterator = ::haze::column_iterator< pixel_type      , _self > ;
        using         const_column_iterator = ::haze::column_iterator< pixel_type const, _self > ;
        using       reverse_column_iterator = ::uti::reverse_iterator<         column_iterator > ;
        using const_reverse_column_iterator = ::uti::reverse_iterator<   const_column_iterator > ;

        using               row_range = uti::range<               row_iterator > ;
        using         const_row_range = uti::range<         const_row_iterator > ;
        using       reverse_row_range = uti::range<       reverse_row_iterator > ;
        using const_reverse_row_range = uti::range< const_reverse_row_iterator > ;

        using               column_range = uti::range<               column_iterator > ;
        using         const_column_range = uti::range<         const_column_iterator > ;
        using       reverse_column_range = uti::range<       reverse_column_iterator > ;
        using const_reverse_column_range = uti::range< const_reverse_column_iterator > ;

        constexpr image () noexcept = default ;

        constexpr image (                    ssize_type _width_, ssize_type _height_ ) noexcept ;
        constexpr image ( pixel_type _fill_, ssize_type _width_, ssize_type _height_ ) noexcept ;

        constexpr image ( pixel_storage const & _pixels_, ssize_type _width_, ssize_type _height_ ) noexcept
                : width_( _width_ ), height_( _height_ ), pixels_( _pixels_ ) {}

        constexpr image ( pixel_storage && _pixels_, ssize_type _width_, ssize_type _height_ ) noexcept
                : width_( _width_ ), height_( _height_ ), pixels_( UTI_MOVE( _pixels_ ) ) {}

        template< meta::image_like ImageType >
        constexpr image ( ImageType const & _other_ ) noexcept
                : width_( _other_.width() ), height_( _other_.height() ), pixels_( _other_.row_begin(), _other_.row_end() ) {}

        constexpr image             ( image const &  _other_ ) noexcept : width_ ( _other_. width_ ), height_ ( _other_.height_ ), pixels_ (           _other_.pixels_   ) {                                                       }
        constexpr image             ( image       && _other_ ) noexcept : width_ ( _other_. width_ ), height_ ( _other_.height_ ), pixels_ ( UTI_MOVE( _other_.pixels_ ) ) { _other_.width_ = _other_.height_ = 0 ;                }
        constexpr image & operator= ( image const &  _other_ ) noexcept { width_ = _other_. width_ ;  height_ = _other_.height_ ;  pixels_ =           _other_.pixels_   ;                                          return *this ; }
        constexpr image & operator= ( image       && _other_ ) noexcept { width_ = _other_. width_ ;  height_ = _other_.height_ ;  pixels_ = UTI_MOVE( _other_.pixels_ ) ;   _other_.width_ = _other_.height_ = 0 ; return *this ; }

        constexpr ~image () noexcept = default ;

        template< meta::image_like ImageType >
        constexpr void place ( ImageType const & _other_, ssize_type _row_, ssize_type _col_ ) noexcept ;

        template< meta::image_like ImageType >
        constexpr void place_channel ( ImageType const & _other_, ssize_type _row_, ssize_type _col_, ssize_type _channel_ ) noexcept ;

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) operator[] ( this Self && self, ssize_type _x_ ) noexcept
        { return UTI_FWD( self ).pixels_[ _x_ ] ; }

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) at ( this Self && self, ssize_type _row_, ssize_type _col_ ) noexcept
        { return UTI_FWD( self ).pixels_.at( _row_ * UTI_FWD( self ).width_ + _col_ ) ; }

        UTI_NODISCARD constexpr image subimage ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) const noexcept ;

        UTI_NODISCARD constexpr image_view< pixel_type      , _self > subview ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ )       noexcept ;
        UTI_NODISCARD constexpr image_view< pixel_type const, _self > subview ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) const noexcept ;

        UTI_NODISCARD constexpr ssize_type  width () const noexcept { return  width_ ; }
        UTI_NODISCARD constexpr ssize_type height () const noexcept { return height_ ; }

        UTI_NODISCARD constexpr shape_type shape () const noexcept { return shape_type{ point_type{ 0, 0 }, point_type{ width(), height() } } ; }

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) pixels ( this Self && self ) noexcept
        { return UTI_FWD( self ).pixels_ ; }

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) begin ( this Self && self ) noexcept
        { return UTI_FWD( self ).pixels_.begin() ; }

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) end ( this Self && self ) noexcept
        { return UTI_FWD( self ).pixels_.end() ; }

        UTI_NODISCARD constexpr auto cbegin () const noexcept { return pixels_.cbegin() ; }
        UTI_NODISCARD constexpr auto cend   () const noexcept { return pixels_.cend  () ; }

        UTI_NODISCARD constexpr       row_iterator row_begin ()       noexcept { return       row_iterator( this, typename       row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_begin () const noexcept { return const_row_iterator( this, typename const_row_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr       row_iterator row_end   ()       noexcept { return       row_iterator( this, typename       row_iterator::  end_tag{} ) ; }
        UTI_NODISCARD constexpr const_row_iterator row_end   () const noexcept { return const_row_iterator( this, typename const_row_iterator::  end_tag{} ) ; }

        UTI_NODISCARD constexpr const_row_iterator row_cbegin () const noexcept { return row_begin() ; }
        UTI_NODISCARD constexpr const_row_iterator row_cend   () const noexcept { return row_end  () ; }

        UTI_NODISCARD constexpr       reverse_row_iterator row_rbegin ()       noexcept { return       reverse_row_iterator( --row_end  () ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_rbegin () const noexcept { return const_reverse_row_iterator( --row_end  () ) ; }
        UTI_NODISCARD constexpr       reverse_row_iterator row_rend   ()       noexcept { return       reverse_row_iterator( --row_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_rend   () const noexcept { return const_reverse_row_iterator( --row_begin() ) ; }

        UTI_NODISCARD constexpr const_reverse_row_iterator row_crbegin () const noexcept { return row_rbegin() ; }
        UTI_NODISCARD constexpr const_reverse_row_iterator row_crend   () const noexcept { return row_rend  () ; }

        UTI_NODISCARD constexpr       column_iterator col_begin ()       noexcept { return       column_iterator( this, typename       column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_begin () const noexcept { return const_column_iterator( this, typename const_column_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr       column_iterator col_end   ()       noexcept { return       column_iterator( this, typename       column_iterator::  end_tag{} ) ; }
        UTI_NODISCARD constexpr const_column_iterator col_end   () const noexcept { return const_column_iterator( this, typename const_column_iterator::  end_tag{} ) ; }

        UTI_NODISCARD constexpr const_column_iterator col_cbegin () const noexcept { return col_begin() ; }
        UTI_NODISCARD constexpr const_column_iterator col_cend   () const noexcept { return col_end  () ; }

        UTI_NODISCARD constexpr       reverse_column_iterator col_rbegin ()       noexcept { return       reverse_column_iterator( --col_end  () ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_rbegin () const noexcept { return const_reverse_column_iterator( --col_end  () ) ; }
        UTI_NODISCARD constexpr       reverse_column_iterator col_rend   ()       noexcept { return       reverse_column_iterator( --col_begin() ) ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_rend   () const noexcept { return const_reverse_column_iterator( --col_begin() ) ; }

        UTI_NODISCARD constexpr const_reverse_column_iterator col_crbegin () const noexcept { return col_rbegin() ; }
        UTI_NODISCARD constexpr const_reverse_column_iterator col_crend   () const noexcept { return col_rend  () ; }

        UTI_NODISCARD constexpr       row_range  row ( ssize_type _row_ )       noexcept { return       row_range( _row_iter( _row_, 0 ), _row_iter( _row_ + 1, 0 ) ) ; }
        UTI_NODISCARD constexpr const_row_range  row ( ssize_type _row_ ) const noexcept { return const_row_range( _row_iter( _row_, 0 ), _row_iter( _row_ + 1, 0 ) ) ; }
        UTI_NODISCARD constexpr const_row_range crow ( ssize_type _row_ ) const noexcept { return row( _row_ ) ; }

        UTI_NODISCARD constexpr       reverse_row_range  rrow ( ssize_type _row_ )       noexcept { return       reverse_row_range( --_row_iter( _row_ + 1, 0 ) , --_row_iter( _row_, 0 ) ) ; }
        UTI_NODISCARD constexpr const_reverse_row_range  rrow ( ssize_type _row_ ) const noexcept { return const_reverse_row_range( --_row_iter( _row_ + 1, 0 ) , --_row_iter( _row_, 0 ) ) ; }
        UTI_NODISCARD constexpr const_reverse_row_range crrow ( ssize_type _row_ ) const noexcept { return rrow( _row_ ) ; }

        UTI_NODISCARD constexpr       column_range  col ( ssize_type _column_ )       noexcept { return       column_range( _col_iter( 0, _column_ ), _col_iter( 0, _column_ + 1 ) ) ; }
        UTI_NODISCARD constexpr const_column_range  col ( ssize_type _column_ ) const noexcept { return const_column_range( _col_iter( 0, _column_ ), _col_iter( 0, _column_ + 1 ) ) ; }
        UTI_NODISCARD constexpr const_column_range ccol ( ssize_type _column_ ) const noexcept { return col( _column_ ) ; }

        UTI_NODISCARD constexpr       reverse_column_range  rcol ( ssize_type _column_ )       noexcept { return       reverse_column_range( --_col_iter( 0, _column_ + 1 ) , --_col_iter( 0, _column_ ) ) ; }
        UTI_NODISCARD constexpr const_reverse_column_range  rcol ( ssize_type _column_ ) const noexcept { return const_reverse_column_range( --_col_iter( 0, _column_ + 1 ) , --_col_iter( 0, _column_ ) ) ; }
        UTI_NODISCARD constexpr const_reverse_column_range crcol ( ssize_type _column_ ) const noexcept { return rcol( _column_ ) ; }
private:
        ssize_type     width_ ;
        ssize_type    height_ ;
        pixel_storage pixels_ ;

        constexpr       row_iterator _row_iter ( ssize_type _row_, ssize_type _col_ )       noexcept { return       row_iterator( this, _row_, _col_ ) ; }
        constexpr const_row_iterator _row_iter ( ssize_type _row_, ssize_type _col_ ) const noexcept { return const_row_iterator( this, _row_, _col_ ) ; }

        constexpr       column_iterator _col_iter ( ssize_type _row_, ssize_type _col_ )       noexcept { return       column_iterator( this, _row_, _col_ ) ; }
        constexpr const_column_iterator _col_iter ( ssize_type _row_, ssize_type _col_ ) const noexcept { return const_column_iterator( this, _row_, _col_ ) ; }
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr
image< PixelType >::image ( ssize_type _width_, ssize_type _height_ ) noexcept
        : width_ ( _width_                          )
        , height_(           _height_               )
        , pixels_( _width_ * _height_, pixel_type{} )
{}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
constexpr
image< PixelType >::image ( pixel_type _fill_, ssize_type _width_, ssize_type _height_ ) noexcept
        : width_ ( _width_                    )
        , height_(           _height_         )
        , pixels_( _width_ * _height_, _fill_ )
{}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
template< meta::image_like ImageType >
constexpr void
image< PixelType >::place ( ImageType const & _other_, ssize_type _row_, ssize_type _col_ ) noexcept
{
        auto dest = subview( _row_, _col_, width() - _col_, height() - _row_ ) ;

        auto w = uti::min( dest. width(), _other_. width() ) ;
        auto h = uti::min( dest.height(), _other_.height() ) ;

        for( ssize_type i = 0; i < h; ++i )
        {
                for( ssize_type j = 0; j < w; ++j )
                {
                        dest.at( i, j ) = _other_.at( i, j ) ;
                }
        }
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
template< meta::image_like ImageType >
constexpr void
image< PixelType >::place_channel ( ImageType const & _other_, ssize_type _row_, ssize_type _col_, ssize_type _channel_ ) noexcept
{
        auto dest = subview( _row_, _col_, width() - _col_, height() - _row_ ) ;

        auto w = uti::min( dest. width(), _other_. width() ) ;
        auto h = uti::min( dest.height(), _other_.height() ) ;

        for( ssize_type i = 0; i < h; ++i )
        {
                for( ssize_type j = 0; j < w; ++j )
                {
                        dest.at( i, j ).channels[ _channel_ ] = _other_.at( i, j ).channels[ _channel_ ] ;
                }
        }
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
UTI_NODISCARD constexpr
image< PixelType >
image< PixelType >::subimage ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) const noexcept
{
        UTI_CEXPR_ASSERT( 0 <= _row_ && _row_ + _height_ < height_ &&
                          0 <= _col_ && _col_ +  _width_ <  width_  ,
                          "haze::image::subimage: index out of bounds"
        ) ;
        image sub( _width_, _height_ ) ;

        for( ssize_type row = _row_; row < _row_ + _height_; ++row )
        {
                for( ssize_type col = _col_; col < _col_ + _width_; ++col )
                {
                        sub.at( row - _row_, col - _col_ ) = at( row, col ) ;
                }
        }
        return sub ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
UTI_NODISCARD constexpr
image_view< PixelType, image< PixelType > >
image< PixelType >::subview ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) noexcept
{
        UTI_CEXPR_ASSERT( 0 <= _row_ && _row_ + _height_ < height_ &&
                          0 <= _col_ && _col_ +  _width_ <  width_  ,
                          "haze::image::subview: index out of bounds"
        ) ;
        return image_view< PixelType, image >( this, _row_, _col_, _width_, _height_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType >
UTI_NODISCARD constexpr
image_view< PixelType const, image< PixelType > >
image< PixelType >::subview ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) const noexcept
{
        UTI_CEXPR_ASSERT( 0 <= _row_ && _row_ + _height_ < height_ &&
                          0 <= _col_ && _col_ +  _width_ <  width_  ,
                          "haze::image::subview: index out of bounds"
        ) ;
        return image_view< PixelType, image >( this, _row_, _col_, _width_, _height_ ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
