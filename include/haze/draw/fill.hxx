//
//
//      haze
//      draw/fill.hxx
//

#pragma once

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/line.hxx>

#include <haze/object/meta.hxx>
#include <haze/object/pixel.hxx>

#include <haze/draw/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename BlenderType, meta::image_like ImageType, meta::shape_object ShapeObject >
        requires( meta::color_blender< BlenderType, typename ImageType::pixel_type > )
constexpr void fill ( ImageType & _image_, ShapeObject const & _object_ ) noexcept ;

////////////////////////////////////////////////////////////////////////////////

template< typename BlenderType, meta::image_like ImageType, meta::shape_object ShapeObject >
        requires( meta::color_blender< BlenderType, typename ImageType::pixel_type > )
constexpr void fill ( ImageType & _image_, ShapeObject const & _object_ ) noexcept
{
        using blender_type =        BlenderType              ;
        using   image_type =          ImageType              ;
        using   pixel_type = typename image_type::pixel_type ;

        constexpr auto blend_fn = blender_type::template operator()< pixel_type > ;

        auto bounding_box = _object_.shape().bounding_box() ;

        auto view = _image_.subview( bounding_box ) ;

        for( auto iter = view.row_begin(); iter != view.row_end(); ++iter )
        {
                *iter = blend_fn( *iter, static_cast< pixel_type >( _object_.at( iter.point() ) ) ) ;
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
