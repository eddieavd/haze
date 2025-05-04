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


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename Pixel, auto Compare > constexpr void sort_pixel_rows ( uti::view< Pixel > _pixels_, ssize_t _sortlen_                            ) noexcept ;
template< typename Pixel, auto Compare > constexpr void sort_pixel_rows ( uti::view< Pixel > _pixels_, ssize_t _sortlen_min_, ssize_t _sortlen_max_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< typename Pixel, auto Compare >
constexpr void sort_pixel_rows ( uti::view< Pixel > _pixels_, ssize_t _sortlen_ ) noexcept
{
        ( void )  _pixels_ ;
        ( void ) _sortlen_ ;
}

////////////////////////////////////////////////////////////////////////////////

template< typename Pixel, auto Compare >
constexpr void sort_pixel_rows ( uti::view< Pixel > _pixels_, ssize_t _sortlen_min_, ssize_t _sortlen_max_ ) noexcept
{
        ( void )      _pixels_ ;
        ( void ) _sortlen_min_ ;
        ( void ) _sortlen_max_ ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
