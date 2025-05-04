//
//
//      haze
//      filter/mean_blur.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>
#include <haze/image/integral_image.hxx>
#include <haze/image/image_view.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageView > constexpr image< typename ImageView::pixel_type > mean_blur_naive    ( ImageView const & _image_, ssize_t _radius_ ) noexcept ;
template< meta::image_like ImageView > constexpr image< typename ImageView::pixel_type > mean_blur_integral ( ImageView const & _image_, ssize_t _radius_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageView >
constexpr image< typename ImageView::pixel_type > mean_blur_naive ( ImageView const & _image_, ssize_t _radius_ ) noexcept
{
        using pixel_type = typename ImageView::pixel_type ;
        using image_type = image< pixel_type > ;

        ssize_t begin_row = _radius_ / 2 ;
        ssize_t begin_col = _radius_ / 2 ;
        ssize_t   end_row = _image_.height() - ( _radius_ / 2 ) ;
        ssize_t   end_col = _image_. width() - ( _radius_ / 2 ) ;

        image_type blurred( _image_.width() - _radius_, _image_.height() - _radius_ ) ;

        for( ssize_t i = begin_row; i < end_row; ++i )
        {
                for( ssize_t j = begin_col; j < end_col; ++j )
                {
                        pixel_type pix {} ;

                        for( ssize_t x = -( _radius_ / 2 ); x < _radius_ / 2; ++x )
                        {
                                for( ssize_t y = -( _radius_ / 2 ); y < _radius_ / 2; ++y )
                                {
                                        pix += _image_.at( i + x, j + y ) ;
                                }
                        }
                        pix /= _radius_ * _radius_ ;

                        blurred.at( i - ( _radius_ / 2 ), j - ( _radius_ / 2 ) ) = pix ;
                }
        }
        return blurred ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageView >
constexpr image< typename ImageView::pixel_type > mean_blur_integral ( ImageView const & _image_, ssize_t _radius_ ) noexcept
{
        using    pixel_type = typename ImageView::pixel_type ;
        using    image_type = image< pixel_type > ;
        using integral_type = integral_image< pixel_type > ;

        ssize_t begin_row = _radius_ / 2 ;
        ssize_t begin_col = _radius_ / 2 ;
        ssize_t   end_row = _image_.height() - ( _radius_ / 2 ) ;
        ssize_t   end_col = _image_. width() - ( _radius_ / 2 ) ;

        integral_type integral( _image_ ) ;

        image_type blurred( _image_.width() - _radius_, _image_.height() - _radius_ ) ;

        for( ssize_t i = begin_row; i < end_row; ++i )
        {
                for( ssize_t j = begin_col; j < end_col; ++j )
                {
                        pixel_type pix = integral.sum_rect( i - ( _radius_ / 2 ), j - ( _radius_ / 2 ) ,
                                                            i + ( _radius_ / 2 ), j + ( _radius_ / 2 ) ) / ( _radius_ * _radius_ ) ;
                        blurred.at( i - ( _radius_ / 2 ), j - ( _radius_ / 2 ) ) = pix ;
                }
        }
        return blurred ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
