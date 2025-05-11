//
//
//      haze
//      image/integral_image.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>

#include <uti/core/container/prefix_array.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename PixelType > class integral_image ;

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
class integral_image_iterator
{
        struct begin_tag {} ;
        struct   end_tag {} ;
public:
        using     pixel_type =                   PixelType   ;
        using raw_pixel_type = uti::remove_cv_t< PixelType > ;

        using      image_type = integral_image< raw_pixel_type > ;
        using      value_type = pixel_type ;
        using      ssize_type = ssize_t    ;
        using difference_type = ssize_type ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        using iterator_category = uti::random_access_iterator_tag ;

        friend image_type ;

        constexpr integral_image_iterator () noexcept = default ;

        constexpr integral_image_iterator ( image_type const & _image_, begin_tag ) noexcept : image_( &_image_ ), row_( 0 ), col_(                0 ) {}
        constexpr integral_image_iterator ( image_type const & _image_,   end_tag ) noexcept : image_( &_image_ ), row_( 0 ), col_( _image_.height() ) {}

        constexpr integral_image_iterator ( image_type const & _image_, ssize_type _row_, ssize_type _col_ ) noexcept
                : image_( &_image_ )
                , row_( _row_ )
                , col_( _col_ )
        {}

        constexpr integral_image_iterator             ( integral_image_iterator const &  _other_ ) noexcept = default ;
        constexpr integral_image_iterator             ( integral_image_iterator       && _other_ ) noexcept = default ;
        constexpr integral_image_iterator & operator= ( integral_image_iterator const &  _other_ ) noexcept = default ;
        constexpr integral_image_iterator & operator= ( integral_image_iterator       && _other_ ) noexcept = default ;

        constexpr ~integral_image_iterator () noexcept = default ;

        constexpr reference operator* () const noexcept
        { return image_->pixels().at( row_, col_ ) ; }

        constexpr pointer operator-> () const noexcept
        { return &image_->pixels().at( row_, col_ ) ; }

        constexpr integral_image_iterator & operator++ () noexcept
        {
                if( col_ >= image_->width() - 1 )
                {
                        col_ = 0 ;
                        ++row_ ;
                }
                else
                {
                        ++col_ ;
                }
                return *this ;
        }
        constexpr integral_image_iterator operator++ ( int ) noexcept
        {
                auto prev = *this ;
                ++*this ;
                return prev ;
        }

        constexpr integral_image_iterator & operator-- () noexcept
        {
                if( col_ == 0 )
                {
                        col_ = image_->width() - 1 ;
                        --row_ ;
                }
                else
                {
                        --col_ ;
                }
                return *this ;
        }
        constexpr integral_image_iterator operator-- ( int ) noexcept
        {
                auto prev = *this ;
                --*this ;
                return prev ;
        }

        constexpr integral_image_iterator & operator+= ( difference_type _n_ ) noexcept
        {
                if( col_ + _n_ >= image_->width() )
                {
                        _n_ -= ( image_->width() - col_ ) ;
                        col_ = _n_ ;
                        ++row_ ;
                }
                else
                {
                        col_ += _n_ ;
                }
                return *this ;
        }
        constexpr integral_image_iterator & operator-= ( difference_type _n_ ) noexcept
        {
                if( _n_ > col_ )
                {
                        _n_ -= col_ ;
                        col_ = image_->width() - _n_ ;
                        --row_ ;
                }
                else
                {
                        col_ -= _n_ ;
                }
                return *this ;
        }

        constexpr integral_image_iterator operator+ ( difference_type _n_ ) const noexcept
        {
                auto iter = *this ;
                iter += _n_ ;
                return iter ;
        }
        constexpr integral_image_iterator operator- ( difference_type _n_ ) const noexcept
        {
                auto iter = *this ;
                iter -= _n_ ;
                return iter ;
        }

        friend constexpr integral_image_iterator operator+ ( difference_type _n_, integral_image_iterator _iter_ ) noexcept
        { return _iter_ + _n_ ; }

        friend constexpr integral_image_iterator operator- ( difference_type _n_, integral_image_iterator _iter_ ) noexcept
        { return _iter_ - _n_ ; }

        constexpr integral_image_iterator & operator+= ( integral_image_iterator const & _other_ ) noexcept
        {
                row_ += _other_.row_ ;
                col_ += _other_.col_ ;

                if( col_ >= image_->width() )
                {
                        col_ -= image_->width() ;
                        ++row_ ;
                }
                return *this ;
        }

        constexpr integral_image_iterator & operator-= ( integral_image_iterator const & _other_ ) noexcept
        {
                row_ -= _other_.row_ ;
                col_ -= _other_.col_ ;

                if( col_ < 0 )
                {
                        col_ = image_->width() + col_ ;
                        --row_ ;
                }
                return *this ;
        }

        constexpr integral_image_iterator operator+ ( integral_image_iterator const & _other_ ) const noexcept
        {
                integral_image_iterator iter = *this ;
                iter += _other_ ;
                return iter ;
        }
        constexpr difference_type operator- ( integral_image_iterator const & _other_ ) const noexcept
        {
                integral_image_iterator iter = *this ;
                iter -= _other_ ;
                return iter ;
        }

        constexpr reference operator[] ( ssize_type _index_ ) const noexcept { return *( *this + _index_ ) ; }

        constexpr bool operator== ( integral_image_iterator const & _other_ ) const noexcept
        { return image_ == _other_.image_ && row_ == _other_.row_ && col_ == _other_.col_ ; }

        constexpr bool operator!= ( integral_image_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr bool operator< ( integral_image_iterator const & _other_ ) const noexcept
        { return ( row_ * image_->width() + col_ ) < ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ; }

        constexpr bool operator<= ( integral_image_iterator const & _other_ ) const noexcept
        { return ( row_ * image_->width() + col_ ) <= ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ; }

        constexpr bool operator> ( integral_image_iterator const & _other_ ) const noexcept
        { return _other_ < *this ; }

        constexpr bool operator >= ( integral_image_iterator const & _other_ ) const noexcept
        { return _other_ <= *this ; }
private:
        image_type const * image_ ;
        ssize_type           row_ ;
        ssize_type           col_ ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
class integral_image
{
public:
        using pixel_type    = PixelType                                  ;
        using value_type    = pixel_type                                 ;
        using pixel_storage = prefix_array< prefix_array< pixel_type > > ;

        using ssize_type = ssize_t ;

        using               iterator = integral_image_iterator< pixel_type       > ;
        using         const_iterator = integral_image_iterator< pixel_type const > ;
        using       reverse_iterator = ::uti::reverse_iterator<         iterator > ;
        using const_reverse_iterator = ::uti::reverse_iterator<   const_iterator > ;

        template< meta::image_like Image >
                requires( uti::meta::same_as< pixel_type, typename Image::pixel_type > )
        constexpr integral_image ( Image const & _image_ ) noexcept ;

        constexpr integral_image             ( integral_image const &  _other_ ) noexcept ;
        constexpr integral_image             ( integral_image       && _other_ ) noexcept ;
        constexpr integral_image & operator= ( integral_image const &  _other_ ) noexcept ;
        constexpr integral_image & operator= ( integral_image       && _other_ ) noexcept ;

        constexpr ~integral_image () noexcept = default ;

        UTI_NODISCARD constexpr operator image< PixelType > () const noexcept ;

        UTI_NODISCARD constexpr pixel_type at ( ssize_type _row_, ssize_type _col_ ) const noexcept
        { return pixels_.element_at( _row_, _col_ ) ; }

        UTI_NODISCARD constexpr integral_image subimage ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) const noexcept ;

        UTI_NODISCARD constexpr pixel_type sum_rect ( ssize_type _x1_, ssize_type _y1_, ssize_type _x2_, ssize_type _y2_ ) const noexcept
        { return pixels_.range( _x1_, _y1_, _x2_, _y2_ ) ; }

        UTI_NODISCARD constexpr ssize_type             width () const noexcept { return  width_ ; }
        UTI_NODISCARD constexpr ssize_type            height () const noexcept { return height_ ; }
        UTI_NODISCARD constexpr pixel_storage const & pixels () const noexcept { return pixels_ ; }

        UTI_NODISCARD constexpr const_iterator  begin () const noexcept { return const_iterator( *this, typename const_iterator::begin_tag{} ) ; }
        UTI_NODISCARD constexpr const_iterator cbegin () const noexcept { return begin() ; }

        UTI_NODISCARD constexpr const_iterator  end () const noexcept { return const_iterator( *this, typename const_iterator::end_tag{} ) ; }
        UTI_NODISCARD constexpr const_iterator cend () const noexcept { return end() ; }

        UTI_NODISCARD constexpr const_reverse_iterator  rbegin () const noexcept { return const_reverse_iterator{ --end() } ; }
        UTI_NODISCARD constexpr const_reverse_iterator crbegin () const noexcept { return const_reverse_iterator{ --end() } ; }

        UTI_NODISCARD constexpr const_reverse_iterator  rend () const noexcept { return const_reverse_iterator{ --begin() } ; }
        UTI_NODISCARD constexpr const_reverse_iterator crend () const noexcept { return const_reverse_iterator{ --begin() } ; }
private:
        ssize_type     width_ ;
        ssize_type    height_ ;
        pixel_storage pixels_ ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
template< meta::image_like Image >
        requires( uti::meta::same_as< PixelType, typename Image::pixel_type > )
constexpr
integral_image< PixelType >::integral_image ( Image const & _image_ ) noexcept
        : width_ ( _image_. width() )
        , height_( _image_.height() )
        , pixels_( _image_.height() )
{
        auto iter = _image_.begin() ;

        for( ssize_type i = 0; i < _image_.height(); ++i )
        {
                pixels_.emplace_back( iter, iter + _image_.width() ) ;
                iter += _image_.width() ;
        }
}

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
constexpr
integral_image< PixelType >::integral_image ( integral_image const & _other_ ) noexcept
        : width_ ( _other_. width() )
        , height_( _other_.height() )
        , pixels_( _other_.pixels() )
{}

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
constexpr
integral_image< PixelType >::integral_image ( integral_image && _other_ ) noexcept
        : width_ (           _other_. width()  )
        , height_(           _other_.height()  )
        , pixels_( UTI_MOVE( _other_.pixels_ ) )
{
        _other_.width_ = _other_.height_ = 0 ;
}

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
constexpr
integral_image< PixelType > &
integral_image< PixelType >::operator= ( integral_image const & _other_ ) noexcept
{
        width_  = _other_. width_ ;
        height_ = _other_.height_ ;
        pixels_ = _other_.pixels_ ;

        return *this ;
}

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
constexpr
integral_image< PixelType > &
integral_image< PixelType >::operator= ( integral_image && _other_ ) noexcept
{
        width_  =           _other_. width_   ;
        height_ =           _other_.height_   ;
        pixels_ = UTI_MOVE( _other_.pixels_ ) ;

        _other_.width_ = _other_.height_ = 0 ;

        return *this ;
}

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
constexpr
integral_image< PixelType >::operator image< PixelType > () const noexcept
{
        image< PixelType > img( width_, height_ ) ;

        for( ssize_type i = 0; i < height_; ++i )
        {
                for( ssize_type j = 0; j < width_; ++j )
                {
                        img.at( i, j ) = pixels_.element_at( i, j ) ;
                }
        }
        return img ;
}

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
constexpr
integral_image< PixelType >
integral_image< PixelType >::subimage ( ssize_type const _row_, ssize_type const _col_, ssize_type const _width_, ssize_type const _height_ ) const noexcept
{
        image< PixelType > img{ *this } ;

        return integral_image( img.subimage( _row_, _col_, _width_, _height_ ) ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
