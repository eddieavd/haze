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

        constexpr  context () noexcept = default ;
        constexpr ~context () noexcept { if( device_ ) device_->release() ; }

        constexpr context             ( context const &  _other_ ) noexcept :              device_ ( _other_.device_ ) {                           ; device_->retain() ;                }
        constexpr context             ( context       && _other_ ) noexcept :              device_ ( _other_.device_ ) { _other_.device_ = nullptr ;                   ;                }
        constexpr context & operator= ( context const &  _other_ ) noexcept { _release() ; device_ = _other_.device_ ;                             ; device_->retain() ; return *this ; }
        constexpr context & operator= ( context       && _other_ ) noexcept { _release() ; device_ = _other_.device_ ;   _other_.device_ = nullptr ;                   ; return *this ; }

        constexpr MTL::Device       * device ()       noexcept { return device_ ; }
        constexpr MTL::Device const * device () const noexcept { return device_ ; }
private:
        MTL::Device * device_ ;

        constexpr void    _init ()          ;
        constexpr void _release () noexcept ;

        constexpr buffer _create_buffer ( ssize_t _bytes_, storage_mode _mode_ ) ;

        template< meta::image_like ImageType >
        constexpr generic_texture< typename ImageType::pixel_type, typename ImageType::point_type >
        _create_texture ( ImageType const & _image_, storage_mode _mode_ ) ;

        template< meta::pixel_like PixelType, meta::point_like PointType >
        constexpr generic_texture< PixelType, PointType >
        _create_texture ( texture_spec const & _spec_, storage_mode _mode_ ) ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr void context::_init ()
{
        device_ = MTL::CreateSystemDefaultDevice() ;

        if( !device_ ) HAZE_CORE_FATAL( "context::init : failed to initialize GPU device context!" ) ;
        else           HAZE_CORE_INFO ( "context::init : initialized default GPU device context"   ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void context::_release () noexcept
{
        if( device_ )
        {
                device_->release() ;
                device_ = nullptr ;
        }
}

////////////////////////////////////////////////////////////////////////////////

constexpr buffer context::_create_buffer ( ssize_t _bytes_, storage_mode _mode_ )
{
        MTL::Buffer * mtlbuff = device_->newBuffer( _bytes_, compat::storage_mode_to_mtl( _mode_ ) ) ;

        return buffer( mtlbuff ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType >
constexpr generic_texture< typename ImageType::pixel_type, typename ImageType::point_type >
context::_create_texture ( ImageType const & _image_, storage_mode _mode_ )
{
        HAZE_CORE_DBG( "generic_texture::_create_texture( image_type )" ) ;

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
context::_create_texture ( texture_spec const & _spec_, storage_mode _mode_ )
{
        HAZE_CORE_DBG( "generic_texture::_create_texture( texture_spec )" ) ;

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
