//
//
//      haze
//      demo.cxx
//

#include <algorithm>
#include <haze/image/image.hxx>
#include <haze/image/integral_image.hxx>
#include <haze/image/image_view.hxx>

#include <haze/image/io.hxx>

#include <haze/filter/mean_blur.hxx>
#include <haze/filter/transform.hxx>

#include <cstdio>


template< typename View > constexpr void sort_pixels_red   ( View pixels ) noexcept ;
template< typename View > constexpr void sort_pixels_green ( View pixels ) noexcept ;
template< typename View > constexpr void sort_pixels_blue  ( View pixels ) noexcept ;


int main ( int argc, char ** argv )
{
        if( argc < 2 )
        {
                printf( "usage: %s <path_to_image>\n", argv[ 0 ] ) ;
                return 1 ;
        }
        haze::string_view filename( argv[ 1 ] ) ;

        auto source = haze::load_image< haze::rgb_pixel >( filename ) ;

        for( int i = 0; i < source.height(); ++i )
        {
                auto  row = source. row( i ) ;
                auto rrow = source.rrow( i ) ;

                uti::range  left(  row.begin() + ( source.width() / 2 ), row.end() ) ;
                uti::range right( rrow.begin() + ( source.width() / 2 ), rrow.end() ) ;

                sort_pixels_green( left ) ;
                sort_pixels_green( right ) ;
        }
        haze::store_image( "twin.png", source ) ;


        return 0 ;
}

template< typename Range >
constexpr void sort_pixels_red ( Range pixels ) noexcept
{
        std::sort( pixels.begin(), pixels.end(),
                []( auto const & lhs, auto const & rhs )
        {
                return lhs.channels[ lhs.RED ] < rhs.channels[ rhs.RED ] ;
        } ) ;
}

template< typename Range >
constexpr void sort_pixels_green ( Range pixels ) noexcept
{
        std::sort( pixels.begin(), pixels.end(),
                []( auto const & lhs, auto const & rhs )
        {
                return lhs.channels[ lhs.GREEN ] < rhs.channels[ rhs.GREEN ] ;
        } ) ;
}

template< typename Range >
constexpr void sort_pixels_blue ( Range pixels ) noexcept
{
        std::sort( pixels.begin(), pixels.end(),
                []( auto const & lhs, auto const & rhs )
        {
                return lhs.channels[ lhs.BLUE ] < rhs.channels[ rhs.BLUE ] ;
        } ) ;
}
