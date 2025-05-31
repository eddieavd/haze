//
//
//      haze
//      gfx/renderer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>

#include <haze/gfx/context.hxx>
#include <haze/gfx/buffer.hxx>
#include <haze/gfx/renderer_api.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

class renderer
{
public:
        using point_type =     fpoint_3d ;
        using pixel_type = rgba_u8_pixel ;

        constexpr  renderer () ;
        constexpr ~renderer () noexcept { release() ; }

        constexpr void           clear       (                    )                ;
        constexpr pixel_type     clear_color (                    ) const noexcept ;
        constexpr void       set_clear_color ( pixel_type _color_ )       noexcept ;

//      constexpr void draw_indexed ( vertex_array const & _vertex_array_, i64_t  _index_count_ ) ;
//      constexpr void draw_lines   ( vertex_array const & _vertex_array_, i64_t _vertex_count_ ) ;

        constexpr void set_line_thickness ( float _thicc_ ) noexcept ;

        constexpr void log_info () const noexcept ;

       constexpr void init ( ssize_t       _dynamic_alloc_init_    , ssize_t       _static_alloc_init_    ,
                             ssize_t _dynamic_write_alloc_init_ = 0, ssize_t _static_write_alloc_init_ = 0,
                             ssize_t      _sparse16_alloc_init_ = 0, ssize_t     _sparse64_alloc_init_ = 0, ssize_t _sparse256_alloc_init_ = 0 )
        {
                api_.init( _dynamic_alloc_init_, _static_alloc_init_, _dynamic_write_alloc_init_, _static_write_alloc_init_,
                           _sparse16_alloc_init_, _sparse64_alloc_init_, _sparse256_alloc_init_ ) ;
        }

        constexpr void release () noexcept ;
private:
        context      ctx_ ;
        renderer_api api_ ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr renderer::renderer ()
        : ctx_( context::create_initialized() )
        , api_( ctx_ )
{}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::release () noexcept
{
        HAZE_CORE_INFO_S( "renderer::release" ) ;
        api_.release() ;
        ctx_.release() ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
