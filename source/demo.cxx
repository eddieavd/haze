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
#include <haze/filter/sort.hxx>

#include <cstdio>


template< typename View > constexpr void sort_pixels_red   ( View pixels ) noexcept ;
template< typename View > constexpr void sort_pixels_green ( View pixels ) noexcept ;
template< typename View > constexpr void sort_pixels_blue  ( View pixels ) noexcept ;


[[ maybe_unused ]] constexpr auto compare_red   = []( auto const & lhs, auto const & rhs ){ return lhs.channels[ lhs.  RED ] < rhs.channels[ rhs.  RED ] ; } ;
[[ maybe_unused ]] constexpr auto compare_green = []( auto const & lhs, auto const & rhs ){ return lhs.channels[ lhs.GREEN ] < rhs.channels[ rhs.GREEN ] ; } ;
[[ maybe_unused ]] constexpr auto compare_blue  = []( auto const & lhs, auto const & rhs ){ return lhs.channels[ lhs. BLUE ] < rhs.channels[ rhs. BLUE ] ; } ;


int main ( int argc, char ** argv )
{
        if( argc < 2 )
        {
                printf( "usage: %s <path_to_image>\n", argv[ 0 ] ) ;
                return 1 ;
        }
        haze::string_view filename( argv[ 1 ] ) ;

        auto image = haze::load_image< haze::rgb_pixel >( filename ) ;
        auto patch = haze::load_image< haze::rgb_pixel >( "sqr.png" ) ;

        printf( "image : %zdpx x %zdpx\n", image.width(), image.height() ) ;
        printf( "patch : %zdpx x %zdpx\n", patch.width(), patch.height() ) ;

        auto row = image.height() - patch.height() / 2 ;
        auto col = image. width() - patch. width() / 2 ;

        auto dest = image.subview( row, col, image.width() - col, image.height() - row ) ;

        printf( "view : %zdpx x %zdpx\n", dest.width(), dest.height() ) ;

        auto w = uti::min( dest. width(), patch. width() ) ;
        auto h = uti::min( dest.height(), patch.height() ) ;

        printf( "w : %ld\n", w ) ;
        printf( "h : %ld\n", h ) ;

        for( int i = 0; i < h; ++i )
        {
                for( int j = 0; j < w; ++j )
                {
                        dest.at( i, j ) = patch.at( i, j ) ;
                }
        }
        haze::store_image( "jank.png", image ) ;



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
