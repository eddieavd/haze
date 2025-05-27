//
//
//      haze
//      gfx/backend/metal/compat.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/gfx/backend/metal/common.hxx>


namespace haze::mtl::compat
{


////////////////////////////////////////////////////////////////////////////////

constexpr MTL::PixelFormat metal_pixel_format ( pixel_format _format_ ) noexcept
{
        if     ( _format_ == pixel_format::   Ru8 ) { return MTL::PixelFormatR8Uint ; }
        else if( _format_ == pixel_format:: RGBu8 ) {}
        else if( _format_ == pixel_format::RGBAu8 ) { return MTL::PixelFormatRGBA8Uint ; }
        else if( _format_ == pixel_format::BGRAu8 ) {}
        else if( _format_ == pixel_format::   Ri8 ) { return MTL::PixelFormatR8Sint ; }
        else if( _format_ == pixel_format:: RGBi8 ) {}
        else if( _format_ == pixel_format::RGBAi8 ) { return MTL::PixelFormatRGBA8Sint ; }
        else if( _format_ == pixel_format::BGRAi8 ) {}

        else if( _format_ == pixel_format::   Ru16 ) { return MTL::PixelFormatR16Uint ; }
        else if( _format_ == pixel_format:: RGBu16 ) {}
        else if( _format_ == pixel_format::RGBAu16 ) { return MTL::PixelFormatRGBA16Uint ; }
        else if( _format_ == pixel_format::BGRAu16 ) {}
        else if( _format_ == pixel_format::   Ri16 ) { return MTL::PixelFormatR16Sint ; }
        else if( _format_ == pixel_format:: RGBi16 ) {}
        else if( _format_ == pixel_format::RGBAi16 ) { return MTL::PixelFormatRGBA16Sint ; }
        else if( _format_ == pixel_format::BGRAi16 ) {}

        else if( _format_ == pixel_format::   Ru32 ) { return MTL::PixelFormatR32Uint ; }
        else if( _format_ == pixel_format:: RGBu32 ) {}
        else if( _format_ == pixel_format::RGBAu32 ) { return MTL::PixelFormatRGBA32Uint ; }
        else if( _format_ == pixel_format::BGRAu32 ) {}
        else if( _format_ == pixel_format::   Ri32 ) { return MTL::PixelFormatR32Sint ; }
        else if( _format_ == pixel_format:: RGBi32 ) {}
        else if( _format_ == pixel_format::RGBAi32 ) { return MTL::PixelFormatRGBA32Sint ; }
        else if( _format_ == pixel_format::BGRAi32 ) {}

        else if( _format_ == pixel_format::   Ru64 ) {}
        else if( _format_ == pixel_format:: RGBu64 ) {}
        else if( _format_ == pixel_format::RGBAu64 ) {}
        else if( _format_ == pixel_format::BGRAu64 ) {}
        else if( _format_ == pixel_format::   Ri64 ) {}
        else if( _format_ == pixel_format:: RGBi64 ) {}
        else if( _format_ == pixel_format::RGBAi64 ) {}
        else if( _format_ == pixel_format::BGRAi64 ) {}

        else if( _format_ == pixel_format::   R32F ) { return MTL::PixelFormatR32Float ; }
        else if( _format_ == pixel_format:: RGB32F ) {}
        else if( _format_ == pixel_format::RGBA32F ) { return MTL::PixelFormatRGBA32Float ; }
        else if( _format_ == pixel_format::BGRA32F ) {}

        return MTL::PixelFormatInvalid ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl::compat
