//
//
//      haze
//      scene/layer.hxx
//

#pragma once

#include <uti/core/container/variant_vector.hxx>

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/rectangle.hxx>
#include <haze/geometry/line.hxx>
#include <haze/geometry/triangle.hxx>

#include <haze/object/meta.hxx>
#include <haze/object/pixel.hxx>
#include <haze/object/shape.hxx>
#include <haze/object/image.hxx>

#include <haze/draw/meta.hxx>
#include <haze/draw/blender.hxx>
#include <haze/draw/interpolator.hxx>
#include <haze/draw/fill.hxx>

#include <haze/scene/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType, meta::shape_object... ExtraShapes >
class fast_layer
{
        struct object_data ;
public:
        using point_type = PointType ;
        using pixel_type = PixelType ;
        using image_type = generic_image< pixel_type, point_type > ;

        using pixel_value_type = typename pixel_type::value_type ;

        using      line_type = generic_line     < point_type > ;
        using rectangle_type = generic_rectangle< point_type > ;
        using  triangle_type = generic_triangle < point_type > ;

        using filled_rectangle_type = filled_shape< pixel_type, rectangle_type > ;
        using filled_triangle_type  = filled_shape< pixel_type,  triangle_type > ;

        using object_storage = uti::variant_vector< ::haze::memory_resource, filled_rectangle_type, filled_triangle_type, ExtraShapes... > ;

        using shape_type = rectangle_type ;

        using blender_type = haze::alpha_blender ;

        static constexpr auto blend_fn = blender_type::template operator()< pixel_type > ;

        constexpr fast_layer (                                     ) noexcept = default ;
        constexpr fast_layer ( shape_type _rect_                   ) noexcept : shape_{ _rect_ }, alpha_{       1.0 } {}
        constexpr fast_layer ( shape_type _rect_, double _opacity_ ) noexcept : shape_{ _rect_ }, alpha_{ _opacity_ } {}

        constexpr double       & opacity ()       noexcept { return alpha_ ; }
        constexpr double const & opacity () const noexcept { return alpha_ ; }

        template< meta::shape_object ShapeObject >
        constexpr void add_object ( ShapeObject const & _object_ ) noexcept
        {
                using known_objects = typename object_storage::value_types ;

                if constexpr( uti::meta::list::contains_type_v< ShapeObject, known_objects > )
                {
                        objects_.push_back( _object_ ) ;
                }
                else
                {
                        objects_.push_back( _make_object_data( _object_ ) ) ;
                }
        }

        UTI_NODISCARD constexpr image_type render () const noexcept
        {
                image_type image( pixel_type{}, shape_ ) ;

                objects_.for_each(
                        [ & ]( auto const & obj )
                {
                        typename image_type::image_view_type view ;

                        if constexpr( uti::meta::same_as< uti::remove_cvref_t< decltype( obj ) >, object_data > )
                        {
                                view = image.subview( obj.bounding_box() ) ;
                        }
                        else
                        {
                                view = image.subview( obj.shape().bounding_box() ) ;
                        }
                        for( auto iter = view.row_begin(); iter < view.row_end(); ++iter )
                        {
                                pixel_type at { obj.at( iter.point() ) } ;
                        
                                double alpha { static_cast< double >( at[ pixel_type::ALPHA ] ) } ;
                                alpha /= 255.0 ;
                                alpha *= alpha_ ;
                                at[ pixel_type::ALPHA ] = alpha * 255.0 ;
                        
                                *iter = blend_fn( *iter, at ) ;
                        }
                } ) ;
                return image ;
        }

        UTI_NODISCARD constexpr pixel_type at ( point_type _point_ ) const noexcept
        {
                pixel_type pixel {} ;

                objects_.for_each(
                        [ & ]( auto const & obj )
                        {
                                if( obj.contains( _point_ ) )
                                {
                                        pixel = blend_fn( pixel, obj.at( _point_ - obj.shape().bounding_box().begin_point() ) ) ;
                                }
                        }
                ) ;
                return pixel ;
        }

        template< typename Self, typename Visitor, typename... Args >
        constexpr void for_each ( this Self && self, Visitor&& _visitor_, Args&&... _args_ ) noexcept
        {
                for( ssize_t i = 0; i < UTI_FWD( self ).objects_.size(); ++i )
                {
                        UTI_FWD( self ).objects_.visit( i, UTI_FWD( _visitor_ ), UTI_FWD( _args_ )... ) ;
                }
        }

        template< meta::shape_object ShapeObject > UTI_NODISCARD constexpr ShapeObject       & get_object ( ssize_t _idx_ )       noexcept { return objects_.template get< ShapeObject >( _idx_ ) ; }
        template< meta::shape_object ShapeObject > UTI_NODISCARD constexpr ShapeObject const & get_object ( ssize_t _idx_ ) const noexcept { return objects_.template get< ShapeObject >( _idx_ ) ; }
private:
        struct object_data
        {
                void const * obj_ ;
                bool           ( * contains_    )( void const *, point_type ) ;
                pixel_type     ( *       at_    )( void const *, point_type ) ;
                rectangle_type ( *bounding_box_ )( void const *             ) ;
                point_type     ( *box_begin_    )( void const *             ) ;

                constexpr bool               contains ( point_type _p_ ) const noexcept { return     contains_( obj_, _p_ ) ; }
                constexpr pixel_type               at ( point_type _p_ ) const noexcept { return           at_( obj_, _p_ ) ; }
                constexpr rectangle_type bounding_box (                ) const noexcept { return bounding_box_( obj_      ) ; }
                constexpr point_type        box_begin (                ) const noexcept { return    box_begin_( obj_      ) ; }
        } ;

        shape_type       shape_ ;
        double           alpha_ ;
        object_storage objects_ ;

        template< meta::shape_object ShapeObject >
        constexpr object_data _make_object_data ( ShapeObject const & _object_ ) const noexcept
        {
                return object_data{
                        .obj_          = &_object_                                                                                                          ,
                        .contains_     = []( void const * obj, point_type p ){ return static_cast< ShapeObject const * >( obj )->shape().contains( p )  ; } ,
                        .at_           = []( void const * obj, point_type p ){ return static_cast< ShapeObject const * >( obj )->at( p )                ; } ,
                        .bounding_box_ = []( void const * obj ){ return static_cast< ShapeObject const * >( obj )->shape().bounding_box()               ; } ,
                        .box_begin_    = []( void const * obj ){ return static_cast< ShapeObject const * >( obj )->shape().bounding_box().begin_point() ; }
                } ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType >
class layer
{
public:
        using point_type =                            PointType    ;
        using pixel_type =                PixelType                ;
        using image_type = generic_image< pixel_type, point_type > ;

        using pixel_value_type = typename pixel_type::value_type ;

        using rectangle_type = generic_rectangle< point_type > ;
        using     shape_type =         rectangle_type          ;

        using blender_type = haze::alpha_blender ;

        static constexpr auto blend_fn = blender_type::template operator()< pixel_type > ;

        constexpr layer (                                     ) noexcept = default ;
        constexpr layer ( shape_type _rect_                   ) noexcept : shape_{ _rect_ }, alpha_{       1.0 }, objects_() {}
        constexpr layer ( shape_type _rect_, double _opacity_ ) noexcept : shape_{ _rect_ }, alpha_{ _opacity_ }, objects_() {}

        template< meta::shape_object ShapeObject >
        constexpr void add_object ( ShapeObject const & _object_ ) noexcept
        {
                objects_.emplace_back( _make_object_data( _object_ ) ) ;
        }

        constexpr void set_opacity ( double _opacity_ ) noexcept { alpha_ = _opacity_ ; }

        constexpr double opacity () const noexcept { return alpha_ ; }

        constexpr pixel_type at ( point_type _point_ ) const noexcept
        {
                pixel_type pixel {} ;

                for( auto const & obj : objects_ )
                {
                        if( obj.contains( _point_ ) )
                        {
                                pixel = blend_fn( pixel, obj.at( _point_ - obj.box_begin() ) ) ;

                                double alpha { static_cast< double >( pixel[ pixel_type::ALPHA ] ) } ;
                                alpha /= 255.0 ;
                                alpha *= alpha_ ;
                                pixel[ pixel_type::ALPHA ] = alpha * 255.0 ;
                        }
                }
                return pixel ;
        }

        constexpr image_type render () const noexcept
        {
                image_type image( pixel_type{}, shape_ ) ;

                for( auto const & obj : objects_ )
                {
                        auto view = image.subview( obj.box() ) ;

                        for( auto iter = view.row_begin(); iter < view.row_end(); ++iter )
                        {
                                pixel_type at { obj.at( iter.point() ) } ;

                                double alpha { static_cast< double >( at[ pixel_type::ALPHA ] ) } ;
                                alpha /= 255.0 ;
                                alpha *= alpha_ ;
                                at[ pixel_type::ALPHA ] = alpha * 255.0 ;

                                *iter = blend_fn( *iter, obj.at( iter.point() ) ) ;
                        }
                }
                return image ;
        }
private:
        struct object_data
        {
                void const * obj_ ;
                bool           ( * contains_ )( void const *, point_type ) ;
                pixel_type     ( *       at_ )( void const *, point_type ) ;
                rectangle_type ( *      box_ )( void const *             ) ;
                point_type     ( *box_begin_ )( void const *             ) ;

                constexpr bool        contains ( point_type _p_ ) const noexcept { return  contains_( obj_, _p_ ) ; }
                constexpr pixel_type        at ( point_type _p_ ) const noexcept { return        at_( obj_, _p_ ) ; }
                constexpr rectangle_type   box (                ) const noexcept { return       box_( obj_      ) ; }
                constexpr point_type box_begin (                ) const noexcept { return box_begin_( obj_      ) ; }
        } ;

        shape_type              shape_ {     } ;
        double                  alpha_ { 1.0 } ;
        vector< object_data > objects_ {     } ;

        template< meta::shape_object ShapeObject >
        constexpr object_data _make_object_data ( ShapeObject const & _object_ ) const noexcept
        {
                return object_data{
                        .obj_       = &_object_                                                                                                          ,
                        .contains_  = []( void const * obj, point_type p ){ return static_cast< ShapeObject const * >( obj )->shape().contains( p )  ; } ,
                        .at_        = []( void const * obj, point_type p ){ return static_cast< ShapeObject const * >( obj )->at( p )                ; } ,
                        .box_       = []( void const * obj ){ return static_cast< ShapeObject const * >( obj )->shape().bounding_box()               ; } ,
                        .box_begin_ = []( void const * obj ){ return static_cast< ShapeObject const * >( obj )->shape().bounding_box().begin_point() ; }
                } ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
