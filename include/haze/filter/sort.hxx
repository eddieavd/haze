//
//
//      haze
//      filter/sort.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image_view.hxx>

#include <algorithm>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< auto Compare, meta::image_like Image > constexpr void sort_pixel_rows ( Image & _image_, ssize_t _sortlen_                            ) noexcept ;
template< auto Compare, meta::image_like Image > constexpr void sort_pixel_rows ( Image & _image_, ssize_t _sortlen_min_, ssize_t _sortlen_max_ ) noexcept ;

template< auto Compare, meta::image_like Image > constexpr void sort_pixel_cols ( Image & _image_, ssize_t _sortlen_                            ) noexcept ;
template< auto Compare, meta::image_like Image > constexpr void sort_pixel_cols ( Image & _image_, ssize_t _sortlen_min_, ssize_t _sortlen_max_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< auto Compare, meta::image_like Image >
constexpr void sort_pixel_rows ( Image & _image_, ssize_t _sortlen_ ) noexcept
{
        auto iter = _image_.row_begin() ;

        while( iter + _sortlen_ < _image_.row_end() )
        {
                std::sort( iter, iter + _sortlen_,
                        []( auto const & lhs, auto const & rhs )
                {
                        return Compare( lhs, rhs ) ;
                } ) ;
                iter += _sortlen_ ;
        }
}

////////////////////////////////////////////////////////////////////////////////

template< auto Compare, meta::image_like Image >
constexpr void sort_pixel_rows ( Image & _image_, ssize_t _sortlen_min_, ssize_t _sortlen_max_ ) noexcept
{
        auto iter = _image_.row_begin() ;

        ssize_t rmax = _sortlen_max_ - _sortlen_min_ ;

        ssize_t sortlen = rand() % rmax + _sortlen_min_ ;

        while( iter + sortlen < _image_.row_end() )
        {
                std::sort( iter, iter + sortlen,
                        []( auto const & lhs, auto const & rhs )
                {
                        return Compare( lhs, rhs ) ;
                } ) ;
                iter += sortlen ;
                sortlen = rand() % rmax + _sortlen_min_ ;
        }
}

////////////////////////////////////////////////////////////////////////////////

template< auto Compare, meta::image_like Image >
constexpr void sort_pixel_cols ( Image & _image_, ssize_t _sortlen_ ) noexcept
{
        auto iter = _image_.col_begin() ;

        while( iter + _sortlen_ < _image_.col_end() )
        {
                std::sort( iter, iter + _sortlen_,
                        []( auto const & lhs, auto const & rhs )
                {
                        return Compare( lhs, rhs ) ;
                } ) ;
                iter += _sortlen_ ;
        }
}

////////////////////////////////////////////////////////////////////////////////

template< auto Compare, meta::image_like Image >
constexpr void sort_pixel_cols ( Image & _image_, ssize_t _sortlen_min_, ssize_t _sortlen_max_ ) noexcept
{
        auto iter = _image_.col_begin() ;

        ssize_t rmax = _sortlen_max_ - _sortlen_min_ ;

        ssize_t sortlen = rand() % rmax + _sortlen_min_ ;

        while( iter + sortlen < _image_.col_end() )
        {
                std::sort( iter, iter + sortlen,
                        []( auto const & lhs, auto const & rhs )
                {
                        return Compare( lhs, rhs ) ;
                } ) ;
                iter += sortlen ;
                sortlen = rand() % rmax + _sortlen_min_ ;
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
