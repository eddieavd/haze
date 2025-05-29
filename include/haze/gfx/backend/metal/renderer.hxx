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

#include <haze/gfx/backend/metal/shaders/common.h>

#include <haze/gfx/backend/metal/shaders/basic_triangle_source.hxx>
#include <haze/gfx/backend/metal/shaders/rotating_triangle_source.hxx>
#include <haze/gfx/backend/metal/shaders/multi_triangle_source.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

static constexpr i32_t max_frames_in_flight { 4 } ;

////////////////////////////////////////////////////////////////////////////////

class renderer : public renderer_base< renderer >
{
        using _self = renderer ;
        using _base = renderer_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  renderer ( context & _ctx_ ) noexcept : ctx_{ _ctx_ } { HAZE_CORE_DBG( "renderer : created" ) ; }
        constexpr ~renderer (                 ) noexcept { _release() ; }

        constexpr bool initialized () const noexcept { return cmd_q_ != nullptr ; }
private:
        context                  &     ctx_    ;
        MTL::CommandQueue        *   cmd_q_ {} ;
        MTL::RenderPipelineState * rpstate_ {} ;

        MTL::Library * shader_lib_ {} ;

        buffer         arg_buffer_ {} ;
        buffer  vertex_pos_buffer_ {} ;
        buffer  vertex_col_buffer_ {} ;

        uti::array< buffer, max_frames_in_flight > frame_data_buffers_ {} ;

        float angle_ {} ;
        i32_t frame_ {} ;

        dispatch_semaphore_t semaphore_ ;

        constexpr void    _init ()          ;
        constexpr void _release () noexcept ;

        constexpr void    _compile_shaders   () ;
        constexpr void _initialize_buffers   () ;
        constexpr void _initialize_frame_data() ;

        constexpr void _draw ( layer const & _layer_, void * _view_ ) ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_draw ( layer const & _layer_, void * _view_ )
{
        MTK::View * view = static_cast< MTK::View * >( _view_ ) ;

        HAZE_CORE_YAP_S( "renderer::draw : starting draw call..." ) ;

        NS::AutoreleasePool * pool = NS::AutoreleasePool::alloc()->init() ;

        frame_ = ( frame_ + 1 ) % max_frames_in_flight ;
        MTL::Buffer * frame_data_buffer = frame_data_buffers_[ frame_ ] ;

        HAZE_CORE_YAP( "renderer::draw : initializing command buffer..." ) ;
        MTL::CommandBuffer * cmd = cmd_q_->commandBuffer() ;
        if( !cmd ) { HAZE_CORE_FATAL( "renderer::draw : failed initializing command buffer!" ) ; return ; }
        HAZE_CORE_YAP( "renderer::draw : command buffer initialized" ) ;

        HAZE_CORE_YAP( "renderer::draw : waiting for GPU to process in-flight frames..." ) ;
        dispatch_semaphore_wait( semaphore_, DISPATCH_TIME_FOREVER ) ;
        HAZE_CORE_YAP( "renderer::draw : wait over" ) ;

        renderer * r_ptr = this ;
        cmd->addCompletedHandler( ^void( MTL::CommandBuffer * _cmd_ ){ dispatch_semaphore_signal( r_ptr->semaphore_ ) ; } ) ;

        HAZE_CORE_YAP( "renderer::draw : updating per-frame data..." ) ;

        reinterpret_cast< frame_data * >( frame_data_buffer->contents() )->angle = ( angle_ += 0.01f ) ;
        frame_data_buffer->didModifyRange( NS::Range::Make( 0, sizeof( frame_data ) ) ) ;

        HAZE_CORE_YAP( "renderer::draw : getting render pass descriptor..." ) ;
        MTL::RenderPassDescriptor * rpd = view->currentRenderPassDescriptor() ;
        if( !rpd ) { HAZE_CORE_FATAL( "renderer::draw : failed getting render pass descriptor!" ) ; return ; }
        HAZE_CORE_YAP( "renderer::draw : render pass descriptor initialized" ) ;

        HAZE_CORE_YAP( "renderer::draw : initializing render command encoder..." ) ;
        MTL::RenderCommandEncoder * enc = cmd->renderCommandEncoder( rpd ) ;
        if( !enc ) { HAZE_CORE_FATAL( "renderer::draw : failed initializing render command encoder!" ) ; return ; }
        HAZE_CORE_YAP( "renderer::draw : render command encoder intialized" ) ;

        enc->setRenderPipelineState( rpstate_ ) ;
        HAZE_CORE_YAP( "renderer::draw : render pipeline state set" ) ;

        HAZE_CORE_YAP( "renderer::draw : filling vertex buffers..." ) ;

        _layer_.for_each(
        [ & ]( auto const & object )
        {
                using object_type = uti::remove_cvref_t< decltype( object ) > ;
        
                if constexpr( uti::meta::instantiated_from< object_type, filled_shape > )
                {
                        using pixel_type = typename object_type::pixel_type ;
                        using shape_type = typename object_type::shape_type ;
        
                        shape_type const & shape = object.shape() ;
        
                        if constexpr( uti::meta::instantiated_from< shape_type, generic_triangle > )
                        {
                                simd::float3 positions [ 3 ] =
                                {
                                        { shape.a_.x(), shape.a_.y(), 0.0 } ,
                                        { shape.b_.x(), shape.b_.y(), 0.0 } ,
                                        { shape.c_.x(), shape.c_.y(), 0.0 }
                                } ;
                                simd::float3 colors [ 3 ] =
                                {
                                        { 1.0, 0.0, 0.0 } ,
                                        { 0.0, 1.0, 0.0 } ,
                                        { 0.0, 0.0, 1.0 } ,
//                                      { object.fill()[ pixel_type:: RED ] / 255.0f, object.fill()[ pixel_type::GREEN ] / 255.0f , object.fill()[ pixel_type::BLUE ] / 255.0f } ,
//                                      { object.fill()[ pixel_type:: RED ] / 255.0f, object.fill()[ pixel_type::GREEN ] / 255.0f , object.fill()[ pixel_type::BLUE ] / 255.0f } ,
//                                      { object.fill()[ pixel_type:: RED ] / 255.0f, object.fill()[ pixel_type::GREEN ] / 255.0f , object.fill()[ pixel_type::BLUE ] / 255.0f }
                                } ;
                                memcpy( vertex_pos_buffer_.data(), positions, sizeof( positions ) ) ;
                                memcpy( vertex_col_buffer_.data(),    colors, sizeof(    colors ) ) ;
        
                                vertex_pos_buffer_.signal_modified() ;
                                vertex_col_buffer_.signal_modified() ;
                        }
                }
        } ) ;
        HAZE_CORE_YAP( "renderer::draw : setting render pass arguments..." ) ;

        enc->setVertexBuffer( arg_buffer_, 0, 0 ) ;
        enc->useResource( vertex_pos_buffer_, MTL::ResourceUsageRead ) ;
        enc->useResource( vertex_col_buffer_, MTL::ResourceUsageRead ) ;

        enc->setVertexBuffer( frame_data_buffer, 0, 1 ) ;

        HAZE_CORE_YAP( "renderer::draw : encoding draw call..." ) ;

        enc->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger( 0 ), NS::UInteger( 3 ) ) ;

        HAZE_CORE_YAP( "renderer::draw : ending encoding..." ) ;
        enc->endEncoding() ;

        HAZE_CORE_YAP( "renderer::draw : asking system to present drawable..." ) ;
        cmd->presentDrawable( view->currentDrawable() ) ;

        HAZE_CORE_YAP( "renderer::draw : committing work to GPU..." ) ;
        cmd->commit() ;

        HAZE_CORE_YAP( "renderer::draw : finished" ) ;
        pool->release() ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_release () noexcept
{
        if( shader_lib_ ) { shader_lib_      ->release() ; shader_lib_ = nullptr ; }
                            arg_buffer_       .release() ;
                            vertex_pos_buffer_.release() ;
                            vertex_col_buffer_.release() ;
        for( auto & buff : frame_data_buffers_ ) buff.release() ;
        if(    rpstate_ ) { rpstate_         ->release() ;    rpstate_ = nullptr ; }
        if(      cmd_q_ ) { cmd_q_           ->release() ;      cmd_q_ = nullptr ; }
}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_init ()
{
        HAZE_CORE_INFO( "renderer::init : creating command queue..." ) ;

        cmd_q_ = ctx_.device()->newCommandQueue() ;

        if( !cmd_q_ ) HAZE_CORE_FATAL( "renderer::init : failed creating command queue!" ) ;
        else          HAZE_CORE_INFO ( "renderer::init : command queue created" ) ;

        HAZE_CORE_INFO( "renderer::init : compiling shaders..." ) ;
        _compile_shaders() ;
        HAZE_CORE_INFO( "renderer::init : allocating buffers..." ) ;
        _initialize_buffers() ;
        _initialize_frame_data() ;

        semaphore_ = dispatch_semaphore_create( max_frames_in_flight ) ;
        HAZE_CORE_INFO( "renderer::init : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_compile_shaders ()
{
        HAZE_CORE_INFO_S( "renderer::compile_shaders" ) ;

        using NS::StringEncoding::UTF8StringEncoding ;

        string_view shader_source = ::haze::mtl::rotating_triangle_shader_source ;

        NS::Error * error = nullptr ;
        MTL::Library * library = ctx_.device()->newLibrary( NS::String::string( shader_source.data(), UTF8StringEncoding ), nullptr, &error ) ;
        if ( !library )
        {
                HAZE_CORE_FATAL( "renderer::_compile_shaders : %s", error->localizedDescription()->utf8String() ) ;
        }
        HAZE_CORE_INFO( "renderer::compile_shaders : created shader library from source" ) ;

        MTL::Function * vertex_fn = library->newFunction( NS::String::string(   "vertex_main", UTF8StringEncoding ) ) ;
        MTL::Function *   frag_fn = library->newFunction( NS::String::string( "fragment_main", UTF8StringEncoding ) ) ;

        HAZE_CORE_INFO( "renderer::compile_shaders : created vertex and fragment functions" ) ;

        MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init() ;
        desc->setVertexFunction( vertex_fn ) ;
        desc->setFragmentFunction( frag_fn ) ;
        desc->colorAttachments()->object( 0 )->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB ) ;

        HAZE_CORE_INFO( "renderer::compile_shaders : configured render pipeline descriptor" ) ;

        MTL::RenderPipelineState * rpstate = ctx_.device()->newRenderPipelineState( desc, &error ) ;
        if ( !rpstate )
        {
                HAZE_CORE_FATAL( "renderer::_compile_shaders : %s", error->localizedDescription()->utf8String() ) ;
        }
        HAZE_CORE_INFO( "renderer::compile_shaders : render pipeline state created" ) ;

        rpstate_    = rpstate ;
        shader_lib_ = library ;

        vertex_fn->release() ;
        frag_fn  ->release() ;
        desc     ->release() ;

        HAZE_CORE_INFO( "renderer::compile_shaders : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_initialize_buffers ()
{
        HAZE_CORE_INFO_S( "renderer::initialize_buffers" ) ;

        static constexpr ssize_t pos_data_size { 3 * sizeof( simd::float3 ) } ;
        static constexpr ssize_t col_data_size { 3 * sizeof( simd::float3 ) } ;

        vertex_pos_buffer_ = ctx_.create_buffer( pos_data_size, storage_mode::managed ) ;
        vertex_col_buffer_ = ctx_.create_buffer( col_data_size, storage_mode::managed ) ;

        if( !vertex_pos_buffer_.is_loaded() ) { HAZE_CORE_FATAL( "renderer::initialize_buffers : failed to allocate vertex position buffer!" ) ; return ; }
        if( !vertex_col_buffer_.is_loaded() ) { HAZE_CORE_FATAL( "renderer::initialize_buffers : failed to allocate vertex color buffer!"    ) ; return ; }

        HAZE_CORE_INFO( "renderer::intialize_buffers : allocated %ld bytes for vertex position buffer", pos_data_size ) ;
        HAZE_CORE_INFO( "renderer::intialize_buffers : allocated %ld bytes for vertex color buffer"   , col_data_size ) ;

        using NS::StringEncoding::UTF8StringEncoding ;

        if( !shader_lib_ ) { HAZE_CORE_FATAL( "renderer::initialize_buffers : shader library does not exist!" ) ; return ; }

        HAZE_CORE_INFO( "renderer::intialize_buffers : getting vertex function from shader library..." ) ;
        MTL::Function * vertex_fn = shader_lib_->newFunction( NS::String::string( "vertex_main", UTF8StringEncoding ) ) ;

        if( !vertex_fn ) { HAZE_CORE_FATAL( "renderer::initialize_buffers : failed getting vertex function from shader library!" ) ; return ; }

        HAZE_CORE_INFO( "renderer::intialize_buffers : creating argument encoder for vertex function..." ) ;
        MTL::ArgumentEncoder * arg_encoder = vertex_fn->newArgumentEncoder( 0 ) ;

        if( !arg_encoder ) { HAZE_CORE_FATAL( "renderer::initialize_buffers : failed creating argument encoder!" ) ; return ; }

        arg_buffer_ = ctx_.create_buffer( arg_encoder->encodedLength(), storage_mode::managed ) ;

        if( !arg_buffer_.is_loaded() ) { HAZE_CORE_FATAL( "renderer::initialize_buffers : failed to allocate argument buffer!" ) ; return ; }

        HAZE_CORE_INFO( "renderer::initialize_buffers : allocated %ld byte argument buffer", arg_buffer_.allocated_size() ) ;

        HAZE_CORE_INFO( "renderer::intialize_buffers : initializing argument buffer..." ) ;
        arg_encoder->setArgumentBuffer( arg_buffer_, 0 ) ;

        arg_encoder->setBuffer( vertex_pos_buffer_, 0, 0 ) ;
        arg_encoder->setBuffer( vertex_col_buffer_, 0, 1 ) ;

        arg_buffer_.signal_modified() ;

        vertex_fn  ->release() ;
        arg_encoder->release() ;

        HAZE_CORE_INFO( "renderer::intialize_buffers : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void renderer::_initialize_frame_data ()
{
        HAZE_CORE_INFO_S( "renderer::initialize_frame_data" ) ;

        for( i32_t i = 0; i < max_frames_in_flight; ++i )
        {
                frame_data_buffers_[ i ] = ctx_.create_buffer( sizeof( frame_data ), storage_mode::managed ) ;
        }
        HAZE_CORE_INFO  ( "renderer::initialize_frame_data : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
