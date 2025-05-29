//
//
//      haze
//      gfx/backend/metal/shaders/multi_triangle_source.hxx
//

#pragma once

#include <haze/core/common/types.hxx>


namespace haze::mtl
{

////////////////////////////////////////////////////////////////////////////////

static constexpr string_view multi_triangle_shader_source = R"(

#include <metal_stdlib>

using namespace metal ;

struct v2f
{
        float4 position [[ position ]] ;
         half3    color                ;
} ;

struct vertex_data
{
        device float3 * positions [[ id( 0 ) ]] ;
        device float3 *    colors [[ id( 1 ) ]] ;
} ;

// struct index_data
// {
//         float3 instance_offset ;
//         float4 instance_color  ;
// } ;
// 
// v2f vertex vertex_main ( device const   vertex_data *   _vertex_data_ [[ buffer( 0 ) ]] ,
//                          device const instance_data * _instance_data_ [[ buffer( 1 ) ]] ,
//                          uint vertex_id [[ vertex_id ]], uint instance_id [[ instance_id ]] )
// {
//         v2f out ;
// 
//         float4 pos = float4( vertext_data[ vertex_id ].position, 1.0 ) ;
// 
//         out.position = pos + instance_data[ instance_id ].instance_offset ;
//         out.color    = half3( instance_data[ instance_id ].instance_color.rgb ) ;
// 
//         return out ;
// }

struct index_data
{
        float4x4 instance_transform ;
        float4   instance_color     ;
} ;

v2f vertex vertex_main ( device const   vertex_data *   _vertex_data_ [[ buffer( 0 ) ]] ,
                         device const instance_data * _instance_data_ [[ buffer( 1 ) ]] ,
                         uint vertex_id [[ vertex_id ]], uint instance_id [[ instance_id ]] )
{
        v2f out ;

        float4 pos = float4( vertext_data[ vertex_id ].position, 1.0 ) ;

        out.position = instance_data[ instance_id ].instance_transform * pos ;
        out.color    = half3( instance_data[ instance_id ].instance_color.rgb ) ;

        return out ;
}

half4 fragment fragment_main ( v2f in [[ stage_in ]] )
{
        return half4( in.color, 1.0 ) ;
}

)" ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
