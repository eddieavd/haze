//
//
//      haze
//      gfx/backend/metal/renderer_api.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/allocator.hxx>
#include <haze/gfx/vertex_array.hxx>
#include <haze/gfx/renderer_api_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class renderer_api : public renderer_api_base< renderer_api >
{
        using _self = renderer_api               ;
        using _base = renderer_api_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        renderer_api ( context & _ctx_, renderer_api_config const & _config_ ) ;
private:
        context & ctx_ ;
        vector< vertex_array > vertex_arrays_ ;
        gpu_memory_resource dynamic_resource_ ;

        MTL::RenderPipelineState *    rpstate_ ;
        MTL::Library             * shader_lib_ ;

        void _compile_shaders () ;

        void _clear () ;

        void   _load_vertices ( layer const & _layer_                ) ;
        void _update_vertices ( layer const & _layer_, ssize_t _idx_ ) ;

        void _draw ( window & _window_ ) ;

        void     _init ()          ;
        void _shutdown () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
