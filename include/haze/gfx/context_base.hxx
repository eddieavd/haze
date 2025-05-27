//
//
//      haze
//      gfx/context_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/texture.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename ContextImpl >
class context_base
{
        using _impl = ContextImpl ;
public:
        template< meta::image_like ImageType >
        UTI_NODISCARD constexpr generic_texture< typename ImageType::pixel_type, typename ImageType::point_type >
        create_texture ( ImageType    const & _image_ ) { return static_cast< _impl * >( this )->_create_texture( _image_ ) ; }

        template< meta::pixel_like PixelType, meta::point_like PointType >
        UTI_NODISCARD constexpr generic_texture< PixelType, PointType >
        create_texture ( texture_spec const &  _spec_ ) { return static_cast< _impl * >( this )->template _create_texture< PixelType, PointType >(  _spec_ ) ; }
protected:
        constexpr  context_base () noexcept = default ;
        constexpr ~context_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
