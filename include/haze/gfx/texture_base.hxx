//
//
//      haze
//      gfx/texture_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>
#include <haze/core/object/image.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

struct texture_spec
{
        ssize_t       width {                    1 } ;
        ssize_t      height {                    1 } ;
//      pixel_format format { pixel_format::RGBAu8 } ;
        bool      mipmapped {                false } ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename TextureImpl, meta::pixel_like PixelType, meta::point_like PointType >
class texture_base
{
        using _impl = TextureImpl ;
public:
        using pixel_type =          PixelType                      ;
        using point_type =          PointType                      ;
        using  size_type = typename point_type::coordinate_type    ;
        using image_type = generic_image< pixel_type, point_type > ;

        UTI_NODISCARD constexpr size_type  width () const noexcept { return static_cast< _impl const * >( this )->    _width() ; }
        UTI_NODISCARD constexpr size_type height () const noexcept { return static_cast< _impl const * >( this )->   _height() ; }
        UTI_NODISCARD constexpr bool   is_loaded () const noexcept { return static_cast< _impl const * >( this )->_is_loaded() ; }

                      constexpr void         upload ( image_type const & _image_ )       noexcept {        static_cast< _impl       * >( this )->  _upload( _image_ ) ; }
        UTI_NODISCARD constexpr image_type download (                            ) const noexcept { return static_cast< _impl const * >( this )->_download(         ) ; }

        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }
protected:
        constexpr  texture_base () noexcept = default ;
        constexpr ~texture_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
