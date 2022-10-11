//
//
//      HAZElib
//      ops.metal
//


#include <metal_stdlib>


using namespace metal;


kernel void mean_blur_kern ( device char const * src, device char * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        int src_width   = meta[ 0 ];
        int dest_width  = meta[ 1 ];
        int blur_radius = meta[ 2 ];

        unsigned x_dest = index % dest_width;
        unsigned y_dest = index / dest_width;

        unsigned x_src = x_dest + blur_radius / 2;
        unsigned y_src = y_dest + blur_radius / 2;

        float avg = 0;

        for( unsigned i = y_src - blur_radius / 2; i < y_src + blur_radius / 2; ++i )
        {
                for( unsigned j = x_src - blur_radius / 2; j < x_src + blur_radius / 2; ++j )
                {
                        avg += src[ i * src_width + j ];
                }
        }

        avg /= blur_radius * blur_radius;

        dest[ index ] = avg;
}

kernel void mean_blur_prefix ( device float const * src, device char * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        int src_width   = meta[ 0 ];
        int dest_width  = meta[ 1 ];
        int blur_radius = meta[ 2 ];
        int half_blr_r  = blur_radius / 2;

        unsigned x_dest = index % dest_width;
        unsigned y_dest = index / dest_width;

        unsigned x_src = x_dest + half_blr_r;
        unsigned y_src = y_dest + half_blr_r;

        float avg = 0;

        int x1 = x_src - half_blr_r - 1;
        int y1 = y_src - half_blr_r - 1;
        int x2 = x_src + half_blr_r;
        int y2 = y_src + half_blr_r;

        if( x1 < 0 && y1 < 0 )
        {
                avg = src[ y2 * src_width + x2 ];
        }
        else if( x1 < 0 )
        {
                avg = src[ y2 * src_width + x2 ] - src[ y1 * src_width + x2 ];
        }
        else if( y1 < 0 )
        {
                avg = src[ y2 * src_width + x2 ] - src[ y2 * src_width + x1 ];
        }
        else
        {
                avg = src[ y2 * src_width + x2 ] - src[ y1 * src_width + x2 ] - src[ y2 * src_width + x1 ] + src[ y1 * src_width + x1 ];
        }

        avg /= blur_radius * blur_radius;

        dest[ index ] = avg;
}
