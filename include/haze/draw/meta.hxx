//
//
//      haze
//      draw/meta.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/common/time.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>

#include <haze/object/meta.hxx>


namespace haze::meta
{


////////////////////////////////////////////////////////////////////////////////

template< typename Interpolator, typename PointType >
concept interpolator = requires( Interpolator interpolator )
{
        point_like< PointType > ;

        { Interpolator::template operator()< PointType >( PointType{}, PointType{}, duration{}, duration{} ) }
                -> uti::meta::convertible_to< PointType > ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename ColorBlender, typename PixelType >
concept color_blender = requires( ColorBlender blender )
{
        pixel_like< PixelType > ;

        { ColorBlender::template operator()< PixelType >( PixelType{}, PixelType{} ) }
                -> uti::meta::convertible_to< PixelType > ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename Object >
concept drawable = shape_object< Object > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::meta
