//
//
//      haze
//      demo.cxx
//

#include <haze/core/common/types.hxx>
#include <haze/core/common/range.hxx>
#include <haze/core/common/time.hxx>

#include <haze/core/common/log.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/line.hxx>
#include <haze/core/geometry/rectangle.hxx>
#include <haze/core/geometry/triangle.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>
#include <haze/core/object/shape.hxx>
#include <haze/core/object/image.hxx>
#include <haze/core/object/image_io.hxx>

#include <haze/cpugfx/draw/interpolator.hxx>

#include <haze/gfx/context.hxx>
#include <haze/gfx/buffer.hxx>
#include <haze/gfx/texture.hxx>

#include <haze/app/window.hxx>
#include <haze/app/app.hxx>



using texture_type = haze::texture ;

using point_type = typename texture_type::point_type ;
using pixel_type = typename texture_type::pixel_type ;

using image_type = haze::generic_image< pixel_type, point_type > ;

constexpr MTL::RenderPipelineState * init_render_pipeline_state ( MTL::Device * _device_ ) noexcept ;

int main ( int argc, char ** argv )
{
        static constexpr size_t num_vertices = 3;
        
        static constexpr simd::float3 positions[ num_vertices ] =
        {
                { -0.8f,  0.8f, 0.0f },
                {  0.0f, -0.8f, 0.0f },
                { +0.8f,  0.8f, 0.0f }
        };
        static constexpr simd::float4 colors[ num_vertices ] =
        {
                { 1.0f, 0.0f, 0.0f, 0.1f },
                { 0.0f, 1.0f, 0.0f, 0.1f },
                { 0.0f, 0.0f, 1.0f, 0.1f }
        };
        static constexpr int positions_data_size = num_vertices * sizeof( simd::float3 ) ;
        static constexpr int    colors_data_size = num_vertices * sizeof( simd::float4 ) ;

        haze::log::init( argc, argv ) ;

        HAZE_INFO( "main : preparing for launch..." ) ;

        haze::context ctx ;
        ctx.init() ;

        HAZE_INFO( "main : initialized context" ) ;

        haze::buffer vertex_positions_buffer = ctx.create_buffer( positions_data_size, haze::storage_mode::managed ) ;
        haze::buffer vertex_colors_buffer    = ctx.create_buffer(    colors_data_size, haze::storage_mode::managed ) ;

        HAZE_INFO( "main : allocated buffers" ) ;

        memcpy( vertex_positions_buffer.data(), positions, positions_data_size ) ;
        memcpy( vertex_colors_buffer   .data(),    colors,    colors_data_size ) ;

        vertex_positions_buffer.signal_modified() ;
        vertex_colors_buffer   .signal_modified() ;

        HAZE_INFO( "main : initialized buffers" ) ;

        MTL::RenderPipelineState * rpstate = init_render_pipeline_state( ctx.device() ) ;

        HAZE_INFO( "main : compiled shaders and initialized render pipeline state" ) ;

        auto on_update = [ & ]( haze::layer &, MTL::RenderCommandEncoder * enc )
        {
                enc->setRenderPipelineState( rpstate ) ;

                enc->setVertexBuffer( vertex_positions_buffer, 0, 0 ) ;
                enc->setVertexBuffer( vertex_colors_buffer   , 0, 1 ) ;
                enc->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger( 0 ), NS::UInteger( 3 ) ) ;
        } ;
        HAZE_INFO( "main : creating app..." ) ;

        haze::app app ;

        HAZE_INFO( "main : setting window size and title..." ) ;

        app.get_window().set_size( 800, 800 ) ;
        app.get_window().set_title( "haze::app" ) ;

        HAZE_INFO( "main : setting on_update handler..." ) ;

        app.on_update( on_update ) ;

        HAZE_INFO( "main : launching app..." ) ;

        app.run() ;


        return 0 ;
}

constexpr MTL::RenderPipelineState * init_render_pipeline_state ( MTL::Device * _device_ ) noexcept
{
        using NS::StringEncoding::UTF8StringEncoding;

        const char* shaderSrc = R"(
                #include <metal_stdlib>
                using namespace metal;

                struct v2f
                {
                        float4 position [[position]];
                        half4 color;
                };

                v2f vertex vertexMain( uint vertexId [[vertex_id]],
                                       device const float3* positions [[buffer(0)]],
                                       device const float4* colors [[buffer(1)]] )
                {
                        v2f o;
                        o.position = float4( positions[ vertexId ], 1.0 );
                        o.color = half4 ( colors[ vertexId ] );
                        return o;
                }

                half4 fragment fragmentMain( v2f in [[stage_in]] )
                {
                        return in.color ;
                }
        )";

        NS::Error* pError = nullptr;
        MTL::Library* pLibrary = _device_->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError );
        if ( !pLibrary )
        {
                __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
                assert( false );
        }

        MTL::Function* pVertexFn = pLibrary->newFunction( NS::String::string("vertexMain", UTF8StringEncoding) );
        MTL::Function* pFragFn = pLibrary->newFunction( NS::String::string("fragmentMain", UTF8StringEncoding) );

        MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
        pDesc->setVertexFunction( pVertexFn );
        pDesc->setFragmentFunction( pFragFn );
        pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

        MTL::RenderPipelineState * rpstate = _device_->newRenderPipelineState( pDesc, &pError );
        if ( !rpstate )
        {
                __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
                assert( false );
        }

        pVertexFn->release();
        pFragFn->release();
        pDesc->release();
        pLibrary->release();

        return rpstate ;
}
