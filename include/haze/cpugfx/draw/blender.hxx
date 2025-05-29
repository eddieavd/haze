//
//
//      haze
//      cpugfx/draw/blender.hxx
//

#pragma once

#include <haze/core/common/types.hxx>
#include <haze/core/common/time.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/cpugfx/draw/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

class null_blender
{
public:
        template< meta::pixel_like PixelType >
        UTI_NODISCARD static constexpr PixelType operator() ( PixelType, PixelType _over_ ) noexcept { return _over_ ; }
} ;

////////////////////////////////////////////////////////////////////////////////

class alpha_blender
{
public:
        template< meta::alpha_pixel PixelType >
        UTI_NODISCARD static constexpr PixelType operator() ( PixelType _base_, PixelType _over_ ) noexcept
        {
                using       pixel_type =                       PixelType                  ;
                using thicc_pixel_type = generic_pixel< float, pixel_type::num_channels > ;

                thicc_pixel_type rhs( _base_ ) ;
                thicc_pixel_type lhs( _over_ ) ;

                lhs /= 255.0 ;
                rhs /= 255.0 ;

                auto lhs_alpha = lhs[ pixel_type::ALPHA ] ;
                auto rhs_alpha = rhs[ pixel_type::ALPHA ] ;

                thicc_pixel_type result { lhs } ;

                double alpha { lhs_alpha + ( rhs_alpha * ( 1.0 - lhs_alpha ) ) } ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                result.channels[ Idxs ] = (   lhs.channels[ Idxs ] * lhs_alpha
                                                          + ( rhs.channels[ Idxs ] * rhs_alpha * ( 1.0 - lhs_alpha ) ) )
                                                        / ( alpha ) ;
                        }() ) ;
                }( uti::make_index_sequence< pixel_type::num_channels - 1 >{} ) ;

                result[ pixel_type::ALPHA ] = alpha ;

                return pixel_type{ result * 255.0 } ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
