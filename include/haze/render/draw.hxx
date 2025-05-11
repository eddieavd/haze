//
//
//      haze
//      render/draw.hxx
//

#pragma once

#include <cstdio>
#include <haze/geo/meta.hxx>
#include <haze/geo/point.hxx>
#include <haze/geo/line.hxx>
#include <haze/geo/rectangle.hxx>
#include <haze/geo/circle.hxx>
#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>


namespace haze
{


template< meta::image_like ImageType, typename LineType >
constexpr void draw_line ( ImageType & _image_, LineType const & _line_, typename ImageType::pixel_type _stroke_ ) noexcept
{
        fpoint_2d normal_point = _line_.end_point() - _line_.begin_point() ;
        fpoint_2d step {} ;
        
        if( normal_point.x() == 0 )
        {
                step.x() = 0.0 ;
                step.y() = 1.0 ;
        }
        else if( normal_point.y() == 0 )
        {
                step.x() = 1.0 ;
                step.y() = 0.0 ;
        }
        else
        {
                double slope = normal_point.y() / normal_point.x() ;
                step.x() = 1.0 ;
                step.y() = slope ;
        }
        fpoint_2d draw( _line_.begin_point() ) ;
        
        while( draw.x() <= _line_.end_point().x() && draw.y() <= _line_.end_point().y() )
        {
                _image_.at( draw.y(), draw.x() ) = _stroke_ ;
                draw += step ;
        }
}

template< meta::image_like ImageType, meta::shape_like ShapeType >
constexpr void fill_shape ( ImageType & _image_, ShapeType const & _shape_, typename ImageType::pixel_type _fill_ ) noexcept
{
        auto box = _shape_.bounding_box() ;

        for( ssize_t i = box.begin_y(); i <= box.end_y(); ++i )
        {
                for( ssize_t j = box.begin_x(); j <= box.end_x(); ++j )
                {
                        if( _shape_.contains( { j, i } ) )
                        {
                                _image_.at( i, j ) = _fill_ ;
                        }
                }
        }
}


} // namespace haze
