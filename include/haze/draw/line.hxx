//
//
//      haze
//      draw/line.hxx
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

template< typename Blender, meta::image_like ImageType, meta::shape_like LineType >
        requires( uti::meta::instantiated_from< LineType, generic_line > &&
                  meta::color_blender< Blender, typename ImageType::pixel_type > )
constexpr void draw_line ( ImageType & _image_, LineType const & _line_, typename ImageType::pixel_type _stroke_ ) ;

template< meta::image_like ImageType, meta::shape_like LineType >
        requires( uti::meta::instantiated_from< LineType, generic_line > )
constexpr void draw_line ( ImageType & _image_, LineType const & _line_, typename ImageType::pixel_type _stroke_, typename LineType::point_type::coordinate_type _thickness_ ) ;


////////////////////////////////////////////////////////////////////////////////

template< typename BlenderType, meta::image_like ImageType, meta::shape_like LineType >
        requires( uti::meta::instantiated_from< LineType, generic_line > &&
                  meta::color_blender< BlenderType, typename ImageType::pixel_type > )
constexpr void draw_line ( ImageType & _image_, LineType const & _line_, typename ImageType::pixel_type _stroke_ )
{
        using       line_type =    LineType                          ;
        using      image_type =   ImageType                          ;
        using    blender_type = BlenderType                          ;
        using      point_type = typename  line_type::     point_type ;
        using      pixel_type = typename image_type::     pixel_type ;
        using coordinate_type = typename point_type::coordinate_type ;

        constexpr auto blend_fn = blender_type::template operator()< pixel_type > ;

        using fpoint_type = generic_point_nd< double, point_type::dimensions > ;

        fpoint_type normalized = _line_.normalized().end_point() ;
        fpoint_type step {} ;

        coordinate_type step_count {} ;

        if( normalized.x() == 0 )
        {
                step.x() = 0 ;
                step.y() = ( 1 * ( normalized.y() > 0 ) ) + ( -1 * ( normalized.y() < 0 ) ) ;

                step_count = normalized.y() * step.y() ;
        }
        else if( normalized.y() == 0 )
        {
                step.x() = ( 1 * ( normalized.x() > 0 ) ) + ( -1 * ( normalized.x() < 0 ) ) ;
                step.y() = 0 ;

                step_count = normalized.x() * step.x() ;
        }
        else
        {
                double slope = std::abs( static_cast< double >( normalized.y() ) / static_cast< double >( normalized.x() ) ) ;
                step.x() = ( 1 * ( normalized.x() > 0 ) ) + ( -1 * ( normalized.x() < 0 ) ) ;
                step.y() = ( 1 * ( normalized.y() > 0 ) ) + ( -1 * ( normalized.y() < 0 ) ) ;

                if( slope < 1.0 )
                {
                        step.y()  *= slope ;
                        step_count = normalized.x() * step.x() ;
                }
                else
                {
                        step.x()  *= 1.0 / slope ;
                        step_count = normalized.y() * step.y() ;
                }
        }
        fpoint_type draw_target( _line_.begin_point() ) ;

        for( int i = 0; i < step_count; ++i )
        {
                _image_.at( draw_target ) = blend_fn( _image_.at( draw_target ), _stroke_ ) ;
                draw_target += step ;
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
