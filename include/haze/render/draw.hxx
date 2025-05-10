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


template< meta::image_like ImageType, typename ShapeType >
constexpr void draw_shape ( ImageType & _image_, ShapeType const & _shape_, typename ImageType::pixel_type _fill_ ) noexcept
{
        if constexpr( uti::meta::instantiated_from< ShapeType, generic_line > )
        {
                fpoint_2d normal_point = _shape_.end_point() - _shape_.begin_point() ;
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
                fpoint_2d draw( _shape_.begin_point() ) ;

                while( draw.col() <= _shape_.end_point().col() && draw.row() <= _shape_.end_point().row() )
                {
                        _image_.at( draw.row(), draw.col() ) = _fill_ ;
                        draw += step ;
                }
        }
        else if constexpr( uti::meta::instantiated_from< ShapeType, generic_rectangle > )
        {
                for( ssize_t i = _shape_.begin_row(); i <= _shape_.end_row() && i < _image_.height(); ++i )
                {
                        for( ssize_t j = _shape_.begin_col(); j <= _shape_.end_col() && j < _image_.width(); ++j )
                        {
                                _image_.at( i, j ) = _fill_ ;
                        }
                }
        }
        else if constexpr( uti::meta::instantiated_from< ShapeType, generic_circle > )
        {
                using point_type = typename ShapeType::point_type ;
                using value_type = typename ShapeType::value_type ;
                using  rect_type = generic_rectangle< value_type > ;

                rect_type rect( { _shape_.center - point_type( _shape_.radius, _shape_.radius ) } ,
                                { _shape_.center + point_type( _shape_.radius, _shape_.radius ) } ) ;

                for( ssize_t i = rect.begin_row(); i <= rect.end_row() && i <= _image_.height(); ++i )
                {
                        for( ssize_t j = rect.begin_col(); j <= rect.end_col() && j <= _image_.width(); ++j )
                        {
                                if( _shape_.contains( point_type( j, i ) ) )
                                {
                                        _image_.at( i, j ) = _fill_ ;
                                }
                        }
                }
        }
}


} // namespace haze
