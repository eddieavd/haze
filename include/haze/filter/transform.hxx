//
//
//      haze
//      filter/transform.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image > constexpr Image rotate_90  ( Image const & _source_ ) noexcept ;
template< meta::image_like Image > constexpr Image rotate_180 ( Image const & _source_ ) noexcept ;
template< meta::image_like Image > constexpr Image rotate_270 ( Image const & _source_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image > constexpr Image flip_vertical   ( Image const & _source_ ) noexcept ;
template< meta::image_like Image > constexpr Image flip_horizontal ( Image const & _source_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image >
constexpr Image rotate_90 ( Image const & _source_ ) noexcept
{
        Image rot( _source_.height(), _source_.width() ) ;

        for( ssize_t i = 0; i < _source_.height(); ++i )
        {
                auto src_row = _source_.row( i ) ;
                auto rot_col =  rot.rcol( i ) ;

                auto src_iter = src_row.begin() ;
                auto rot_iter = rot_col.begin() ;

                while( src_iter != src_row.end() &&
                       rot_iter != rot_col.end()  )
                {
                        * rot_iter = *src_iter ;
                        ++rot_iter ;
                        ++src_iter ;
                }
        }
        return rot ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image >
constexpr Image rotate_180 ( Image const & _source_ ) noexcept
{
        Image rot( _source_.width(), _source_.height() ) ;

        for( ssize_t i = 0; i < _source_.height(); ++i )
        {
                auto src_row = _source_.row( i ) ;
                auto rot_row =  rot.rrow( rot.height() - i - 1 ) ;

                auto src_iter = src_row.begin() ;
                auto rot_iter = rot_row.begin() ;

                while( src_iter != src_row.end() &&
                       rot_iter != rot_row.end()  )
                {
                        * rot_iter = *src_iter ;
                        ++rot_iter ;
                        ++src_iter ;
                }
        }
        return rot ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image >
constexpr Image rotate_270 ( Image const & _source_ ) noexcept
{
        Image rot( _source_.height(), _source_.width() ) ;

        for( ssize_t i = 0; i < _source_.height(); ++i )
        {
                auto src_row = _source_.row( i ) ;
                auto rot_col =  rot.col( rot.width() - i - 1 ) ;

                auto src_iter = src_row.begin() ;
                auto rot_iter = rot_col.begin() ;

                while( src_iter != src_row.end() &&
                       rot_iter != rot_col.end()  )
                {
                        * rot_iter = *src_iter ;
                        ++rot_iter ;
                        ++src_iter ;
                }
        }
        return rot ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
