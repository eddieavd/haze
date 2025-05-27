//
//
//      haze
//      gfx/backend/metal/context.hxx
//

#pragma once

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/gfx/context_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class context : public context_base< context >
{
        using _self = context               ;
        using _base = context_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr context () noexcept : _base(), device_{ MTL::CreateSystemDefaultDevice() }
        {
                if( !device_ ) HAZE_CORE_FATAL( "failed to initialize gpu device" ) ;
                else           HAZE_CORE_INFO (          "initialized gpu device" ) ;
        }

        constexpr ~context () noexcept { device_->release() ; }
private:
        MTL::Device * device_ ;

        template< meta::image_like ImageType >
        constexpr generic_texture< typename ImageType::pixel_type, typename ImageType::point_type >
        _create_texture ( ImageType const & _image_ ) ;

        template< meta::pixel_like PixelType, meta::point_like PointType >
        constexpr generic_texture< PixelType, PointType >
        _create_texture ( texture_spec const &  _spec_ ) ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType >
constexpr generic_texture< typename ImageType::pixel_type, typename ImageType::point_type >
context::_create_texture ( ImageType const & _image_ )
{
        using texture_type = generic_texture< typename ImageType::pixel_type, typename ImageType::point_type > ;

        MTL::TextureDescriptor * desc = MTL::TextureDescriptor::texture2DDescriptor
                (
                        compat::metal_pixel_format( ImageType::pixel_type::format ),
                        _image_.width(), _image_.height(), false
                ) ;
        MTL::Texture * mtl_texture = device_->newTexture( desc ) ;

        desc->release() ;

        auto txt = texture_type( mtl_texture ) ;

        txt.upload( _image_ ) ;

        return txt ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, meta::point_like PointType >
constexpr generic_texture< PixelType, PointType >
context::_create_texture ( texture_spec const & _spec_ )
{
        using texture_type = generic_texture< PixelType, PointType > ;

        MTL::TextureDescriptor * desc = MTL::TextureDescriptor::texture2DDescriptor
                (
                        compat::metal_pixel_format( PixelType::format ),
                        _spec_.width, _spec_.height, _spec_.mipmapped
                ) ;
        MTL::Texture * mtl_texture = device_->newTexture( desc ) ;

        desc->release() ;

        return texture_type( mtl_texture ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
