//
//
//      haze
//      geometry/meta.hxx
//

#pragma once

#include <haze/common/types.hxx>


namespace haze::meta
{


////////////////////////////////////////////////////////////////////////////////

template< typename PointType >
concept point_like = requires( PointType point )
{
//      typename PointType::coordinate_type ;
        typename PointType::     value_type ;

        { PointType::dimensions } ;
//      { point.x() } ;
        { point[ ssize_t{} ] } ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename ShapeType >
concept shape_like = requires( ShapeType shape )
{
        typename ShapeType::point_type ;

        { ShapeType::      dimensions } ;
        { ShapeType::shape_dimensions } ;

        { shape.contains( typename ShapeType::point_type{} ) } -> uti::meta::convertible_to< bool > ;
        { shape.bounding_box() } ;
        { shape.  normalized() } -> uti::meta::convertible_to< ShapeType > ;
} ;


////////////////////////////////////////////////////////////////////////////////


} // namespace haze::meta
