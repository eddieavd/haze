//
//
//      haze
//      image/image_iterator.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>


namespace haze
{


template< meta::pixel_like PixelType > class image ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, template< meta::pixel_like > typename ImageType >
class row_iterator
{
        struct begin_tag {} ;
        struct   end_tag {} ;
public:
        using        pixel_type = PixelType  ;
        using        value_type = pixel_type ;
        using        ssize_type = ssize_t    ;
        using   difference_type = ssize_type ;
        using iterator_category = uti::random_access_iterator_tag ;

        using raw_pixel_type = uti::remove_cv_t< pixel_type >                            ;
        using     image_type = uti::copy_cv_t< pixel_type, ImageType< raw_pixel_type > > ;

        friend image_type ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        constexpr row_iterator () noexcept = default ;

        constexpr row_iterator ( image_type * _image_, ssize_type _row_, ssize_type _col_ ) noexcept
                : image_( _image_ ), row_( _row_ ), col_( _col_ ) {}

        constexpr row_iterator ( image_type * _image_, begin_tag ) noexcept : row_iterator( _image_,                 0, 0 ) {}
        constexpr row_iterator ( image_type * _image_,   end_tag ) noexcept : row_iterator( _image_, _image_->height(), 0 ) {}

        constexpr row_iterator             ( row_iterator const &  ) noexcept = default ;
        constexpr row_iterator             ( row_iterator       && ) noexcept = default ;
        constexpr row_iterator & operator= ( row_iterator const &  ) noexcept = default ;
        constexpr row_iterator & operator= ( row_iterator       && ) noexcept = default ;

        constexpr ~row_iterator () noexcept = default ;

        constexpr reference operator*  () const noexcept { return  image_->at( row_, col_ ) ; }
        constexpr pointer   operator-> () const noexcept { return &image_->at( row_, col_ ) ; }

        constexpr operator pointer () const noexcept { return &image_->at( row_, col_ ) ; }

        constexpr row_iterator & operator++ () noexcept
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
        constexpr row_iterator operator++ ( int ) noexcept
        {
                row_iterator prev = *this ;
                ++*this ;
                return prev ;
        }

        constexpr row_iterator & operator-- () noexcept
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
        constexpr row_iterator operator-- ( int ) noexcept
        {
                row_iterator prev = *this ;
                --*this ;
                return prev ;
        }

        constexpr row_iterator & operator+= ( difference_type _n_ ) noexcept
        {
                col_ += _n_ ;

                if( col_ >= image_->width() )
                {
                        row_ += col_ / image_->width() ;
                        col_ %= image_->width() ;
                }
                return *this ;
        }
        constexpr row_iterator operator+ ( difference_type _n_ ) const noexcept
        {
                row_iterator iter = *this ;
                iter += _n_ ;
                return iter ;
        }
        friend constexpr row_iterator operator+ ( difference_type _n_, row_iterator _iter_ ) noexcept
        { return _iter_ + _n_ ; }

        constexpr row_iterator & operator-= ( difference_type _n_ ) noexcept
        {
                col_ -= _n_ ;

                if( col_ < 0 )
                {
                        col_ = -col_ ;
                        row_ -= ( col_ / image_->width() ) + 1 ;
                        col_ = image_->width() - col_ ;
                }
                return *this ;
        }
        constexpr row_iterator operator- ( difference_type _n_ ) const noexcept
        {
                row_iterator iter = *this ;
                iter -= _n_ ;
                return iter ;
        }
        friend constexpr row_iterator operator- ( difference_type _n_, row_iterator _iter_ ) noexcept
        { return _iter_ - _n_ ; }

        constexpr row_iterator & operator+= ( row_iterator const & _other_ ) noexcept
        {
                row_ += _other_.row_ ;
                col_ += _other_.col_ ;

                if( col_ >= image_->width() )
                {
                        row_ += col_ / image_->width() ;
                        col_ %= image_->width() ;
                }
                return *this ;
        }
        constexpr row_iterator operator+ ( row_iterator const & _other_ ) const noexcept
        {
                row_iterator iter = *this ;
                iter += _other_ ;
                return iter ;
        }

        constexpr difference_type operator- ( row_iterator const & _other_ ) const noexcept
        {
                return ( row_ * image_->width() + col_ ) - ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ;
        }

        constexpr reference operator[] ( ssize_type _index_ ) const noexcept { return *( *this + _index_ ) ; }

        constexpr bool operator== ( row_iterator const & _other_ ) const noexcept
        { return image_ == _other_.image_ && row_ == _other_.row_ && col_ == _other_.col_ ; }

        constexpr bool operator!= ( row_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr bool operator< ( row_iterator const & _other_ ) const noexcept
        { return ( row_ * image_->width() + col_ ) < ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ; }

        constexpr bool operator<= ( row_iterator const & _other_ ) const noexcept
        { return ( row_ * image_->width() + col_ ) <= ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ; }

        constexpr bool operator> ( row_iterator const & _other_ ) const noexcept
        { return ( row_ * image_->width() + col_ ) > ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ; }

        constexpr bool operator>= ( row_iterator const & _other_ ) const noexcept
        { return ( row_ * image_->width() + col_ ) >= ( _other_.row_ * _other_.image_->width() + _other_.col_ ) ; }
private:
        image_type * image_ ;
        ssize_type     row_ ;
        ssize_type     col_ ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, template< meta::pixel_like > typename ImageType >
class column_iterator
{
        struct begin_tag {} ;
        struct   end_tag {} ;
public:
        using        pixel_type = PixelType  ;
        using        value_type = pixel_type ;
        using        ssize_type = ssize_t    ;
        using   difference_type = ssize_type ;
        using iterator_category = uti::random_access_iterator_tag ;

        using raw_pixel_type = uti::remove_cv_t< pixel_type >                            ;
        using     image_type = uti::copy_cv_t< pixel_type, ImageType< raw_pixel_type > > ;

        friend image_type ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        constexpr column_iterator () noexcept = default ;

        constexpr column_iterator ( image_type * _image_, ssize_type _row_, ssize_type _col_ ) noexcept
                : image_( _image_ ), row_( _row_ ), col_( _col_ ) {}

        constexpr column_iterator ( image_type * _image_, begin_tag ) noexcept : column_iterator( _image_, 0,                0 ) {}
        constexpr column_iterator ( image_type * _image_,   end_tag ) noexcept : column_iterator( _image_, 0, _image_->width() ) {}

        constexpr column_iterator             ( column_iterator const &  ) noexcept = default ;
        constexpr column_iterator             ( column_iterator       && ) noexcept = default ;
        constexpr column_iterator & operator= ( column_iterator const &  ) noexcept = default ;
        constexpr column_iterator & operator= ( column_iterator       && ) noexcept = default ;

        constexpr ~column_iterator () noexcept = default ;

        constexpr reference operator*  () const noexcept { return  image_->at( row_, col_ ) ; }
        constexpr pointer   operator-> () const noexcept { return &image_->at( row_, col_ ) ; }

        constexpr operator pointer () const noexcept { return &image_->at( row_, col_ ) ; }

        constexpr column_iterator & operator++ () noexcept
        {
                if( row_ >= image_->height() - 1 )
                {
                        row_ = 0 ;
                        ++col_ ;
                }
                else
                {
                        ++row_ ;
                }
                return *this ;
        }
        constexpr column_iterator operator++ ( int ) noexcept
        {
                column_iterator prev = *this ;
                ++*this ;
                return prev ;
        }

        constexpr column_iterator & operator-- () noexcept
        {
                if( row_ == 0 )
                {
                        row_ = image_->height() - 1 ;
                        --col_ ;
                }
                else
                {
                        --row_ ;
                }
                return *this ;
        }
        constexpr column_iterator operator-- ( int ) noexcept
        {
                column_iterator prev = *this ;
                --*this ;
                return prev ;
        }

        constexpr column_iterator & operator+= ( difference_type _n_ ) noexcept
        {
                row_ += _n_ ;

                if( row_ >= image_->height() )
                {
                        col_ += row_ / image_->height() ;
                        row_ %= image_->height() ;
                }
                return *this ;
        }
        constexpr column_iterator operator+ ( difference_type _n_ ) const noexcept
        {
                column_iterator iter = *this ;
                iter += _n_ ;
                return iter ;
        }
        friend constexpr column_iterator operator+ ( difference_type _n_, column_iterator _iter_ ) noexcept
        { return _iter_ + _n_ ; }

        constexpr column_iterator & operator-= ( difference_type _n_ ) noexcept
        {
                row_ -= _n_ ;

                if( row_ < 0 )
                {
                        row_ = -row_ ;
                        col_ -= ( row_ / image_->height() ) + 1 ;
                        row_ = image_->height() - row_ ;
                }
                return *this ;
        }
        constexpr column_iterator operator- ( difference_type _n_ ) const noexcept
        {
                column_iterator iter = *this ;
                iter -= _n_ ;
                return iter ;
        }
        friend constexpr column_iterator operator- ( difference_type _n_, column_iterator _iter_ ) noexcept
        { return _iter_ - _n_ ; }

        constexpr column_iterator & operator+= ( column_iterator const & _other_ ) noexcept
        {
                row_ += _other_.row_ ;
                col_ += _other_.col_ ;

                if( row_ > image_->height() )
                {
                        col_ += row_ / image_->height() ;
                        row_ %= image_->height() ;
                }
                return *this ;
        }
        constexpr column_iterator operator+ ( column_iterator const & _other_ ) const noexcept
        {
                column_iterator iter = *this ;
                iter += _other_ ;
                return iter ;
        }

        constexpr difference_type operator- ( column_iterator const & _other_ ) const noexcept
        {
                return ( col_ * image_->height() + row_ ) - ( _other_.col_ * _other_.image_->height() + _other_.row_ ) ;
        }

        constexpr reference operator[] ( ssize_type _index_ ) const noexcept { return *( *this + _index_ ) ; }

        constexpr bool operator== ( column_iterator const & _other_ ) const noexcept
        { return image_ == _other_.image_ && row_ == _other_.row_ && col_ == _other_.col_ ; }

        constexpr bool operator!= ( column_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr bool operator< ( column_iterator const & _other_ ) const noexcept
        { return ( col_ * image_->height() + row_ ) < ( _other_.col_ * _other_.image_->height() + _other_.row_ ) ; }

        constexpr bool operator<= ( column_iterator const & _other_ ) const noexcept
        { return ( col_ * image_->height() + row_ ) <= ( _other_.col_ * _other_.image_->height() + _other_.row_ ) ; }

        constexpr bool operator> ( column_iterator const & _other_ ) const noexcept
        { return ( col_ * image_->height() + row_ ) > ( _other_.col_ * _other_.image_->height() + _other_.row_ ) ; }

        constexpr bool operator>= ( column_iterator const & _other_ ) const noexcept
        { return ( col_ * image_->height() + row_ ) >= ( _other_.col_ * _other_.image_->height() + _other_.row_ ) ; }
private:
        image_type * image_ ;
        ssize_type     row_ ;
        ssize_type     col_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
