//
//
//      haze
//      image/meta.hxx
//

#pragma once

#include <uti/core/type/traits.hxx>
#include <uti/core/meta/concepts.hxx>
#include <uti/core/container/meta.hxx>


namespace haze::meta
{


template< typename Image >
concept image_like = requires( Image img )
{
        uti::meta::simple_container< Image > ;
        typename Image::pixel_type ;
        { img. width() } -> uti::meta::convertible_to< uti::ssize_t > ;
        { img.height() } -> uti::meta::convertible_to< uti::ssize_t > ;
} ;


} // namespace haze::meta
