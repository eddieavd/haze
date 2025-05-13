//
//
//      haze
//      image/meta.hxx
//

#pragma once

#include <uti/core/type/traits.hxx>
#include <uti/core/meta/concepts.hxx>
#include <uti/core/container/meta.hxx>

#include <haze/common/types.hxx>
#include <haze/geometry/meta.hxx>


namespace haze::meta
{


////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
concept pixel_like = requires( PixelType pixel )
{
        typename PixelType::value_type ;

        { PixelType::  dimensions } -> uti::meta::convertible_to< ssize_t > ;
        { PixelType::num_channels } -> uti::meta::convertible_to< ssize_t > ;

        { pixel.channels[ ssize_t{} ] } -> uti::meta::convertible_to< typename PixelType::value_type > ;
        { pixel         [ ssize_t{} ] } -> uti::meta::convertible_to< typename PixelType::value_type > ;

        uti::meta::arithmetic< uti::remove_reference_t< decltype( pixel.channels[ ssize_t{} ] ) > > ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename PixelType >
concept alpha_pixel = requires
{
        pixel_like< PixelType > ;
        PixelType::num_channels == 4 ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename ShapeObject >
concept shape_object = requires( ShapeObject shape_object )
{
        typename ShapeObject::point_type ;
        typename ShapeObject::shape_type ;
        typename ShapeObject::pixel_type ;

        point_like< typename ShapeObject::point_type > ;
        shape_like< typename ShapeObject::shape_type > ;
        pixel_like< typename ShapeObject::pixel_type > ;

        { shape_object.shape()                                  } -> uti::meta::convertible_to< typename ShapeObject::shape_type > ;
        { shape_object.at( typename ShapeObject::point_type{} ) } -> uti::meta::convertible_to< typename ShapeObject::pixel_type > ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename ImageType >
concept image_like = requires( ImageType & image, ImageType const & cimage )
{
        uti::meta::simple_container< ImageType > ;
                       shape_object< ImageType > ;

        typename ImageType::   row_iterator ;
        typename ImageType::column_iterator ;

        { image. width() } -> uti::meta::convertible_to< typename ImageType::point_type::coordinate_type > ;
        { image.height() } -> uti::meta::convertible_to< typename ImageType::point_type::coordinate_type > ;

        { image.at
        (
                typename ImageType::point_type{}
        )
        } -> uti::meta::same_as< typename ImageType::pixel_type & > ;

        { cimage.at
        (
                typename ImageType::point_type{}
        )
        } -> uti::meta::same_as< typename ImageType::pixel_type const & > ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::meta
