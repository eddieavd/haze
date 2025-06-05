//
//
//      haze
//      gfx/backend/metal/renderer_api.cxx
//

#include <haze/gfx/backend/metal/renderer_api.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

renderer_api::renderer_api ( context & _ctx_, renderer_api_config const & _config_ )
        : ctx_( _ctx_ )
        , vertex_arrays_()
        , dynamic_resource_( _ctx_, _config_.dynamic_alloc_config )
{
        HAZE_CORE_INFO_S( "renderer_api::ctor" ) ;

        HAZE_CORE_INFO( "renderer_api : initialized metal renderer api" ) ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_init ()
{
        HAZE_CORE_INFO_S( "renderer_api::init" ) ;

        HAZE_CORE_INFO( "renderer_api::init : initializing memory resources..." ) ;
        dynamic_resource_.init( 64 * 1024 * 1024 ) ;
        HAZE_CORE_INFO( "renderer_api::init : compiling shaders..." ) ;
        _compile_shaders() ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_shutdown () noexcept
{
        HAZE_CORE_INFO_S( "renderer_api::shutdown" ) ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_clear ()
{
        HAZE_CORE_INFO_S( "renderer_api::clear" ) ;
        HAZE_CORE_ERROR ( "renderer_api::clear : unimplemented" ) ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_load_vertices ( layer const & _layer_ )
{
        HAZE_CORE_INFO_S( "renderer_api::load_vertices" ) ;
        HAZE_CORE_INFO  ( "renderer_api::load_vertices : currently supports triangles only" ) ;

        using            pixel_type = typename layer::           pixel_type ;
        using         triangle_type = typename layer::        triangle_type ;
        using  filled_triangle_type = typename layer:: filled_triangle_type ;
        using colored_triangle_type = typename layer::colored_triangle_type ;

        vector< colored_triangle_type > triangles ;

        HAZE_CORE_INFO( "renderer_api::load_vertices : filtering out triangles..." ) ;

        _layer_.for_each(
                [ & ]( auto const & obj )
                {
                        using object_type = uti::remove_cvref_t< decltype( obj ) > ;

                        constexpr bool  filled = uti::meta::instantiated_from< object_type,  filled_shape > ;
                        constexpr bool colored = uti::meta::instantiated_from< object_type, colored_shape > ;

                        if constexpr( filled || colored )
                        {
                                using shape_type = typename object_type::shape_type ;

                                if constexpr( uti::meta::instantiated_from< shape_type, generic_triangle > && filled )
                                {
                                        triangles.push_back( colored_triangle_type{ { obj.shape() }, { obj.fill(), obj.fill(), obj.fill() } } ) ;
                                }
                                if constexpr( uti::meta::instantiated_from< shape_type, generic_triangle > && colored )
                                {
                                        triangles.push_back( colored_triangle_type{ { obj.shape() }, { obj.fills_[ 0 ], obj.fills_[ 1 ], obj.fills_[ 2 ] } } ) ;
                                }
                        }
                }
        ) ;
        HAZE_CORE_INFO( "renderer_api::load_vertices : filtered out %ld triangles", triangles.size() ) ;

        if( triangles.size() == 0 )
        {
                HAZE_CORE_WARN( "renderer_api::load_vertices : scene empty" ) ;
                return ;
        }
        u32_t idxs[ 3 ] = { 0, 1, 2 } ;

        HAZE_CORE_INFO( "renderer_api::load_vertices : allocating buffers..." ) ;

        vertex_buffer positions = dynamic_resource_.alloc_vertex_buffer( triangles.size() * sizeof( simd::float3 ) * 3 ) ;
        vertex_buffer    colors = dynamic_resource_.alloc_vertex_buffer( triangles.size() * sizeof( simd::float4 ) * 3 ) ;
         index_buffer   indices = dynamic_resource_.alloc_index_buffer( idxs, 3 ) ;

        HAZE_CORE_INFO( "renderer_api::load_vertices : buffers allocated" ) ;

        HAZE_CORE_INFO( "renderer_api::load_vertices : configuring buffer layouts..." ) ;

        buffer_element position{ shader_data_type::FLOAT3, "triangle_vertex_position", false } ;
        buffer_element    color{ shader_data_type::FLOAT4, "triangle_vertex_color"   , false } ;

        buffer_layout pos_layout( position ) ;
        buffer_layout col_layout(    color ) ;

        positions.layout() = UTI_MOVE( pos_layout ) ;
        colors   .layout() = UTI_MOVE( col_layout ) ;

        HAZE_CORE_INFO( "renderer_api::load_vertices : initializing buffers..." ) ;

        for( ssize_t i = 0; i < triangles.size(); ++i )
        {
                simd::float3 pos_data[ 3 ] = {
                        { triangles[ i ].shape_.a_.x(), triangles[ i ].shape_.a_.y(), triangles[ i ].shape_.a_.z() } , // I_CAN_FIX_HER: use simd::float3 in haze::core::geometry
                        { triangles[ i ].shape_.b_.x(), triangles[ i ].shape_.b_.y(), triangles[ i ].shape_.b_.z() } ,
                        { triangles[ i ].shape_.c_.x(), triangles[ i ].shape_.c_.y(), triangles[ i ].shape_.c_.z() }
                } ;
                memcpy( ( unsigned char * ) positions.buffer().data() + positions.layout().stride() * i * 3,   pos_data            , positions.layout().stride() * 3 ) ;
                memcpy( ( unsigned char * )    colors.buffer().data() +    colors.layout().stride() * i * 3, &triangles[ i ].fills_,    colors.layout().stride() * 3 ) ;
        }
        vertex_array vert_array ;

        vert_array.push_vertex_buffer( UTI_MOVE( positions ) ) ;
        vert_array.push_vertex_buffer( UTI_MOVE(    colors ) ) ;
        vert_array.  set_index_buffer( UTI_MOVE(   indices ) ) ;

        vertex_arrays_.emplace_back( UTI_MOVE( vert_array ) ) ;
        HAZE_CORE_INFO( "renderer_api::load_vertices : finished" ) ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_update_vertices ( layer const & _layer_, ssize_t _idx_ )
{
        HAZE_CORE_INFO_S( "renderer_api::update_vertices" ) ;
        HAZE_CORE_ERROR ( "renderer_api::update_vertices : unimplemented" ) ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_draw ( window & _window_ )
{
        HAZE_CORE_INFO_S( "renderer_api::draw" ) ;

        CA::MetalDrawable * drawable = static_cast< CA::MetalLayer * >( _window_ )->nextDrawable() ;

        ssize_t idx {} ;

        for( auto const & vert_array : vertex_arrays_ )
        {
                HAZE_CORE_INFO_S( "renderer_api::draw : rendering vertex array %ld", idx ) ;

                NS::AutoreleasePool * pool = NS::AutoreleasePool::alloc()->init() ;

                MTL::CommandBuffer * cmd = static_cast< MTL::CommandQueue * >( ctx_ )->commandBuffer() ;

                MTL::RenderPassDescriptor                *      render_pass_desc = MTL::RenderPassDescriptor::alloc()->init() ;
                MTL::RenderPassColorAttachmentDescriptor * color_attachment_desc = render_pass_desc->colorAttachments()->object( 0 ) ;

                color_attachment_desc->setTexture( drawable->texture() ) ;
                color_attachment_desc->setLoadAction( MTL::LoadActionClear ) ;
                color_attachment_desc->setClearColor( MTL::ClearColor( 0, 0, 0, 1 ) ) ;
                color_attachment_desc->setStoreAction( MTL::StoreActionStore ) ;

                render_pass_desc->setRenderTargetWidth ( drawable->texture()-> width() ) ;
                render_pass_desc->setRenderTargetHeight( drawable->texture()->height() ) ;

                MTL::RenderCommandEncoder * encoder = cmd->renderCommandEncoder( render_pass_desc ) ;

                encoder->setRenderPipelineState( rpstate_ ) ;
                encoder->setVertexBuffer( vert_array.vertex_buffers().at( 0 ).buffer(), 0, 0 ) ;
                encoder->setVertexBuffer( vert_array.vertex_buffers().at( 1 ).buffer(), 0, 1 ) ;

//              {
//              HAZE_CORE_INFO_S( "renderer_api::draw : debug buffer contents :" ) ;
//              HAZE_CORE_INFO  ( "renderer_api::draw : position buffer :" ) ;
//              vert_array.vertex_buffers().at( 0 ).buffer().dump() ;
//              HAZE_CORE_INFO  ( "renderer_api::draw : color buffer :" ) ;
//              vert_array.vertex_buffers().at( 1 ).buffer().dump() ;
//              HAZE_CORE_INFO  ( "renderer_api::draw : index buffer :" ) ;
//              vert_array.index_buffer().buffer().dump() ;
//              }

                auto vertex_count = vert_array.vertex_buffers().at( 0 ).buffer().capacity() / vert_array.vertex_buffers().at( 0 ).layout().elements().at( 0 ).size ;

                HAZE_CORE_INFO( "renderer_api::draw : calling drawIndexedPrimitives with args :" ) ;
                HAZE_CORE_INFO( "renderer_api::draw :     index count { %u }", vert_array.index_buffer().count() ) ;
                HAZE_CORE_INFO( "renderer_api::draw :    vertex count { %zd }", vertex_count ) ;
                HAZE_CORE_INFO( "renderer_api::draw :     offset      { %u }", 0 ) ;

//              encoder->drawIndexedPrimitives( MTL::PrimitiveTypeTriangle, vert_array.index_buffer().count(), MTL::IndexTypeUInt32, vert_array.index_buffer().buffer(), 0 ) ;

                encoder->drawPrimitives( MTL::PrimitiveTypeTriangle, NS::UInteger( 0 ), vertex_count ) ;

                encoder->endEncoding() ;
                cmd->presentDrawable( drawable ) ; // I_CAN_FIX_HER: have the caller call _drawable_.present(), we might be rendering into a random texture
                cmd->commit() ;

                pool->release() ;

                ++idx ;
                break ;
        }
        HAZE_CORE_INFO( "renderer_api::draw : clearing vertex arrays..." ) ;
        vertex_arrays_.clear() ;
}

////////////////////////////////////////////////////////////////////////////////

void renderer_api::_compile_shaders ()
{
        HAZE_CORE_INFO_S( "renderer_api::compile_shaders" ) ;

        using NS::StringEncoding::UTF8StringEncoding ;

        string_view shader_source = R"(
                #include <metal_stdlib>

                using namespace metal ;

                struct v2f
                {
                        float4 position [[ position ]] ;
                        half4     color                ;
                } ;

                v2f vertex vertex_main (                uint   _vertex_id_ [[ vertex_id   ]] ,
                                         device const float3 * _positions_ [[ buffer( 0 ) ]] ,
                                         device const float4 *    _colors_ [[ buffer( 1 ) ]] )
                {
                        v2f out ;

                        out.position = float4( _positions_[ _vertex_id_ ], 1.0 ) ;
                        out.   color =  half4(    _colors_[ _vertex_id_ ]      ) ;

                        return out ;
                }
                half4 fragment fragment_main ( v2f _in_ [[ stage_in ]] )
                {
                        return half4( _in_.color ) ;
                }
        )" ;

        NS ::Error   * error {} ;
        MTL::Library * library = static_cast< MTL::Device * >( ctx_ )->newLibrary( NS::String::string( shader_source.data(), UTF8StringEncoding ), nullptr, &error ) ;

        HAZE_ASSERT( library != nullptr, "renderer_api::compile_shaders : %s", error->localizedDescription()->utf8String() ) ;

        HAZE_CORE_INFO( "renderer_api::compile shaders : created library" ) ;

        MTL::Function * vertex_fn = library->newFunction( NS::String::string(   "vertex_main", UTF8StringEncoding ) ) ;
        MTL::Function *   frag_fn = library->newFunction( NS::String::string( "fragment_main", UTF8StringEncoding ) ) ;

        MTL::RenderPipelineDescriptor * rp_desc = MTL::RenderPipelineDescriptor::alloc()->init() ;
        rp_desc->setVertexFunction( vertex_fn ) ;
        rp_desc->setFragmentFunction( frag_fn ) ;
        rp_desc->colorAttachments()->object( 0 )->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm ) ;

        HAZE_CORE_INFO( "renderer_api::compile_shaders : configured render pipeline descriptor" ) ;

        MTL::RenderPipelineState * rp_state = static_cast< MTL::Device * >( ctx_ )->newRenderPipelineState( rp_desc, &error ) ;

        HAZE_ASSERT( rp_state != nullptr, "renderer_api::compile_shaders : %s", error->localizedDescription()->utf8String() ) ;

        HAZE_CORE_INFO( "renderer_api::compile_shaders : created render pipeline" ) ;

        rpstate_    = rp_state ;
        shader_lib_ =  library ;

        vertex_fn->release() ;
        frag_fn  ->release() ;
        rp_desc  ->release() ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl

























