//
//
//      haze
//      gfx/backend/metal/texture.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/texture_base.hxx>

#include <haze/gfx/backend/metal/common.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType >
class generic_texture : public texture_base< generic_texture< PixelType, PointType >, PixelType, PointType >
{
        using _self = generic_texture ;
        using _base = texture_base< generic_texture, PixelType, PointType > ;
public:
        using pixel_type = typename _base::pixel_type ;
        using point_type = typename _base::point_type ;
        using  size_type = typename _base:: size_type ;
        using image_type = typename _base::image_type ;

        using  _base::_base ;
        friend _base        ;

        constexpr generic_texture ( MTL::Texture * _texture_ ) noexcept : texture_{ _texture_ } {}

        constexpr ~generic_texture () noexcept { _release() ; }
private:
        MTL::Texture * texture_ ;

        constexpr size_type  _width () const noexcept { return texture_ -> width() ; }
        constexpr size_type _height () const noexcept { return texture_ ->height() ; }
        constexpr bool   _is_loaded () const noexcept { return texture_ != nullptr ; }

        constexpr void         _upload ( image_type const & _image_ )       noexcept ;
        constexpr image_type _download (                            ) const noexcept ;

        constexpr void _release () noexcept { if( texture_ ) texture_->release() ; }
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType >
constexpr void
generic_texture< PixelType, PointType >::_upload ( image_type const & _image_ ) noexcept
{
        texture_->replaceRegion
        (
                MTL::Region
                {
                        0 ,
                        0 ,
                        static_cast< NS::UInteger >( _image_. width() ) ,
                        static_cast< NS::UInteger >( _image_.height() ) ,
                },
                0, 0,
                _image_.pixels().data() ,
                _image_.width() *                    pixel_type::num_channels * sizeof( typename pixel_type::value_type ) ,
                _image_.width() * _image_.height() * pixel_type::num_channels * sizeof( typename pixel_type::value_type )
        ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType >
constexpr
generic_texture< PixelType, PointType >::image_type
generic_texture< PixelType, PointType >::_download () const noexcept
{
        typename image_type::shape_type shape {} ;
        shape.end_point().x() =  _width() ;
        shape.end_point().y() = _height() ;

        image_type image( shape ) ;

        texture_->getBytes
        (
                image.pixels().data() ,
                image.width() * pixel_type::num_channels * sizeof( typename pixel_type::value_type ),
                MTL::Region
                {
                        0 ,
                        0 ,
                        static_cast< NS::UInteger >( image. width() ) ,
                        static_cast< NS::UInteger >( image.height() ) ,
                } ,
                0
        ) ;
        return image ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
