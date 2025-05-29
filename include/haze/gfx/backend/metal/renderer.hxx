//
//
//      haze
//      gfx/backend/metal/renderer.hxx
//

#pragma once

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/gfx/context.hxx>
#include <haze/gfx/buffer.hxx>
#include <haze/gfx/texture.hxx>
#include <haze/gfx/renderer_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class renderer : public renderer_base< renderer >
{
        using _self = renderer ;
        using _base = renderer_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  renderer ( context & _ctx_ ) noexcept : ctx_{ _ctx_ } { HAZE_CORE_DBG( "renderer : created" ) ; }
        constexpr ~renderer (                 ) noexcept { release() ; }

        constexpr void draw ( layer & _layer_, std::function< void( layer &, MTL::RenderCommandEncoder * ) > const & _update_, MTK::View * _view_ ) ;

        constexpr bool initialized () const noexcept { return cmd_q_ != nullptr ; }
private:
        context                  &     ctx_    ;
        MTL::CommandQueue        *   cmd_q_ {} ;
//      MTL::RenderPipelineState * rpstate_ {} ;

        constexpr void    _init ()          ;
        constexpr void _release () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::draw ( layer & _layer_, std::function< void( layer &, MTL::RenderCommandEncoder * ) > const & _update_, MTK::View * _view_ )
{
        HAZE_CORE_YAP_S( "renderer::draw : starting draw call..." ) ;

        NS::AutoreleasePool * pool = NS::AutoreleasePool::alloc()->init() ;

        HAZE_CORE_YAP( "renderer::draw : initializing command buffer..." ) ;
        MTL::CommandBuffer * cmd = cmd_q_->commandBuffer() ;
        if( !cmd ) { HAZE_CORE_FATAL( "renderer::draw : failed initializing command buffer!" ) ; return ; }
        HAZE_CORE_YAP( "renderer::draw : command buffer initialized" ) ;

        HAZE_CORE_YAP( "renderer::draw : getting render pass descriptor..." ) ;
        MTL::RenderPassDescriptor * rpd = _view_->currentRenderPassDescriptor() ;
        if( !rpd ) { HAZE_CORE_FATAL( "renderer::draw : failed getting render pass descriptor!" ) ; return ; }
        HAZE_CORE_YAP( "renderer::draw : render pass descriptor initialized" ) ;

        HAZE_CORE_YAP( "renderer::draw : initializing render command encoder..." ) ;
        MTL::RenderCommandEncoder * enc = cmd->renderCommandEncoder( rpd ) ;
        if( !enc ) { HAZE_CORE_FATAL( "renderer::draw : failed initializing render command encoder!" ) ; return ; }
        HAZE_CORE_YAP( "renderer::draw : render command encoder intialized" ) ;

//      enc->setRenderPipelineState( rpstate_ ) ;
//      HAZE_CORE_YAP( "renderer::draw : render pipeline state set" ) ;

        HAZE_CORE_YAP( "renderer::draw : dispatching to on_update handler..." ) ;
        _update_( _layer_, enc ) ;
        HAZE_CORE_YAP( "renderer::draw : on_update finished" ) ;

//      _layer_.for_each(
//              []( auto const & object )
//              {

//              }
//      ) ;

        HAZE_CORE_YAP( "renderer::draw : ending encoding..." ) ;
        enc->endEncoding() ;

        HAZE_CORE_YAP( "renderer::draw : asking system to present drawable..." ) ;
        cmd->presentDrawable( _view_->currentDrawable() ) ;

        HAZE_CORE_YAP( "renderer::draw : committing work to GPU..." ) ;
        cmd->commit() ;

        HAZE_CORE_YAP( "renderer::draw : finished" ) ;
        pool->release() ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_release () noexcept
{

}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_init ()
{
        HAZE_CORE_INFO( "renderer::init : creating command queue..." ) ;

        cmd_q_ = ctx_.device()->newCommandQueue() ;

        if( !cmd_q_ ) HAZE_CORE_FATAL( "renderer::init : failed creating command queue!" ) ;
        else          HAZE_CORE_INFO ( "renderer::init : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

static constexpr ssize_t max_quad_count   {             10'000 } ;
static constexpr ssize_t max_vertex_count { max_quad_count * 4 } ;
static constexpr ssize_t max_index_count  { max_quad_count * 6 } ;
static constexpr ssize_t max_textures     {                 32 } ; // ask driver for exact number of slots

////////////////////////////////////////////////////////////////////////////////

class batch_renderer
{
        struct stats
        {
                i32_t draw_count {} ;
                i32_t quad_count {} ;
        } ;
public:
        constexpr  batch_renderer ( context & _ctx_ ) noexcept : ctx_{ _ctx_ }, quad_buffer_( max_quad_count ), quad_buffer_iter_( quad_buffer_.begin() ) {}
        constexpr ~batch_renderer (                 ) noexcept { release() ; }

        constexpr void    init ()           ;
        constexpr void release () noexcept {}

        constexpr void begin_batch () ;
        constexpr void   end_batch () ;
        constexpr void       flush () ;

        constexpr void draw_quad ( simd::float2 const & _position_, simd::float2 const & _size_, simd::float4 const & _color_ ) ;
        constexpr void draw_quad ( simd::float2 const & _position_, simd::float2 const & _size_, i32_t           _texture_id_ ) ;

        constexpr stats const & get_stats () const noexcept { return stats_ ; }
        constexpr void        reset_stats ()       noexcept { stats_ = {}   ; }
private:
        context & ctx_ ;

        stats stats_ {} ;

        u32_t quad_vertex_array_  {} ;
        u32_t quad_vertex_buffer_ {} ;
        u32_t quad_index_buffer_  {} ; // 16:20

        texture white_texture_      {} ;
        u32_t   white_texture_slot_ {} ;

        u32_t index_count_ {} ;

        vector< vertex >           quad_buffer_      {} ;
        vector< vertex >::iterator quad_buffer_iter_ {} ;

        uti::array< texture, max_textures > texture_slots_ {} ;
        u32_t texture_slot_index_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
