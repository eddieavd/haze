//
//
//      haze
//      gfx/backend/metal/shaders/basic_triangle_source.hxx
//

#pragma once

#include <haze/core/common/types.hxx>


namespace haze::mtl
{

////////////////////////////////////////////////////////////////////////////////

static constexpr string_view basic_triangle_shader_source = R"(

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

v2f vertex vertex_main ( device const vertex_data * _vertex_data_ [[ buffer( 0 ) ]], uint vertex_id [[ vertex_id ]] )
{
        v2f out ;

        out.position = float4( _vertex_data_->positions[ vertex_id ], 1.0 ) ;
        out.   color =  half3( _vertex_data_->   colors[ vertex_id ]      ) ;

        return out ;
}

half4 fragment fragment_main ( v2f in [[ stage_in ]] )
{
        return half4( in.color, 1.0 ) ;
}

)" ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
