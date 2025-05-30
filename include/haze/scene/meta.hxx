//
//
//      haze
//      scene/meta.hxx
//

#pragma once

#include <haze/common/types.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>

#include <haze/object/meta.hxx>
#include <haze/object/pixel.hxx>


namespace haze::meta
{


////////////////////////////////////////////////////////////////////////////////

template< typename LayerType >
concept layer_like = requires( LayerType layer )
{
                    typename LayerType::point_type   ;
                    typename LayerType::pixel_type   ;
                    typename LayerType::image_type   ;
        point_like< typename LayerType::point_type > ;
        pixel_like< typename LayerType::pixel_type > ;
        image_like< typename LayerType::image_type > ;

        { layer.render()                               } -> uti::meta::convertible_to< typename LayerType::image_type > ;
        { layer.at( typename LayerType::point_type{} ) } -> uti::meta::convertible_to< typename LayerType::pixel_type > ;

//      { layer.template add_object< shape_object >( shape_object const &  ) } ;
//      { layer.template add_object< shape_object >( shape_object       && ) } ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::meta
