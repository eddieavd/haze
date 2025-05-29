//
//
//      haze
//      gfx/context_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/common.hxx>
#include <haze/gfx/buffer.hxx>
#include <haze/gfx/texture.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename ContextImpl >
class context_base
{
        using _impl = ContextImpl ;
public:
        constexpr void    init ()          { static_cast< _impl * >( this )->   _init() ; }
        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }

        UTI_NODISCARD constexpr buffer create_buffer ( ssize_t _bytes_, storage_mode _mode_ ) { return static_cast< _impl * >( this )->_create_buffer( _bytes_, _mode_ ) ; }

        template< meta::image_like ImageType >
        UTI_NODISCARD constexpr generic_texture< typename ImageType::pixel_type, typename ImageType::point_type >
        create_texture ( ImageType const & _image_, storage_mode _mode_ )
        { return static_cast< _impl * >( this )->_create_texture( _image_, _mode_ ) ; }

        template< meta::pixel_like PixelType, meta::point_like PointType >
        UTI_NODISCARD constexpr generic_texture< PixelType, PointType >
        create_texture ( texture_spec const & _spec_, storage_mode _mode_ )
        { return static_cast< _impl * >( this )->template _create_texture< PixelType, PointType >( _spec_, _mode_ ) ; }
protected:
        constexpr  context_base () noexcept = default ;
        constexpr ~context_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
