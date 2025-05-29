//
//
//      haze
//      gfx/backend/metal/basic_renderer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/gfx/context.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class basic_renderer
{
public:
        constexpr  basic_renderer ( context & _ctx_ ) noexcept : ctx_( _ctx_ ), cmd_q_{ ctx_.device()->newCommandQueue() } {}
        constexpr ~basic_renderer (                 ) noexcept { cmd_q_->release() ; }

        constexpr void draw ( MTK::View * _view_ ) ;
private:
        context           & ctx_      ;
        MTL::CommandQueue * cmd_q_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr void basic_renderer::draw ( MTK::View * _view_ )
{
        NS::AutoreleasePool * pool = NS::AutoreleasePool::alloc()->init() ;

        MTL::CommandBuffer        * cmd = cmd_q_->commandBuffer() ;
        MTL::RenderPassDescriptor * rpd = _view_->currentRenderPassDescriptor() ;
        MTL::RenderCommandEncoder * enc = cmd->renderCommandEncoder( rpd ) ;

        enc->endEncoding() ;
        cmd->presentDrawable( _view_->currentDrawable() ) ;
        cmd->commit() ;

        pool->release() ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
