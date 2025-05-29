//
//
//      haze
//      gfx/backend/metal/shaders/basic_triangle.metal
//

#pragma once

#include "common.metal"


namespace haze::mtl
{

////////////////////////////////////////////////////////////////////////////////

v2f vertex vertex_main ( device const vertex_data * _vertex_data_ [[ buffer( 0 ) ]], uint vertex_id [[ vertex_id ]] )
{
        v2f out ;

        out.position = float4( _vertex_data_->positions[ vertex_id ], 1.0 ) ;
        out.   color =  half4( _vertex_data_->   colors[ vertex_id ]      ) ;

        return out ;
}

half4 fragment fragment_main ( v2f in [[ stage_in ]] )
{
        return in.color ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
