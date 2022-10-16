//
//
//      HAZElib
//      ops.metal
//


#include <metal_stdlib>


using namespace metal;


kernel void copy_image ( device char const * src, device char * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        int img_width  = meta[ 0 ];

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        dest[ index ] = src[ row * img_width + col ];
}

kernel void copy_pixel_field ( device unsigned long const * src, device unsigned long * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        int img_width = meta[ 0 ];

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned long val;

        if( row == 0 && col == 0 )
        {
                val = static_cast< char >( src[ row * img_width + col ] );
        }
        else if( row == 0 )
        {
                val = static_cast< char >( src[ row * img_width + col ] - src[ row * img_width + col - 1 ] );
        }
        else if( col == 0 )
        {
                val = static_cast< char >( src[ row * img_width + col ] - src[ ( row - 1 ) * img_width + col ] );
        }
        else
        {
                val = src[ row * img_width + col ] - src[ ( row - 1 ) * img_width + col ] - src[ row * img_width + col - 1 ] + src[ ( row - 1 ) * img_width + col - 1 ];
        }

        dest[ index ] = val;
}

kernel void mean_blur_kern ( device char const * src, device char * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
/*
        int img_width           = meta[ 0 ];
        int img_height          = meta[ 1 ];
        int current_blur_radius = meta[ 2 ];

        long row = index / img_width;
        long col = index % img_width;

        long avg = 0;
*/

        int src_width   = meta[ 0 ];
        int dest_width  = meta[ 1 ];
        int blur_radius = meta[ 2 ];

        long x_dest = index % dest_width;
        long y_dest = index / dest_width;

        long x_src = x_dest + blur_radius / 2;
        long y_src = y_dest + blur_radius / 2;

        long avg = 0;

        for( long i = y_src - blur_radius / 2; i < y_src + blur_radius / 2; ++i )
        {
                for( long j = x_src - blur_radius / 2; j < x_src + blur_radius / 2; ++j )
                {
                        avg += src[ i * src_width + j ];
                }
        }

        avg /= blur_radius * blur_radius;

        dest[ index ] = avg;
}

kernel void mean_blur_prefix ( device unsigned long const * src, device char * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        int src_width   = meta[ 0 ];
        int dest_width  = meta[ 1 ];
        int blur_radius = meta[ 2 ];
        int half_blr_r  = blur_radius / 2;

        unsigned long x_dest = index % dest_width;
        unsigned long y_dest = index / dest_width;

        unsigned long x_src = x_dest + half_blr_r;
        unsigned long y_src = y_dest + half_blr_r;

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

kernel void lens_blur_prefix ( device unsigned long const * src, device char * dest, device int const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        int src_width   = meta[ 0 ];
        int dest_width  = meta[ 1 ];
        int blur_radius = meta[ 2 ];
        int half_blur_r = blur_radius / 2;

        unsigned long x_dest = index % dest_width;
        unsigned long y_dest = index / dest_width;

        unsigned long x_src = x_dest + half_blur_r;
        unsigned long y_src = y_dest + half_blur_r;

        float avg = 0;

        int x1 = x_src - half_blur_r - 1;
        int y1 = y_src - half_blur_r - 1;
        int x2 = x_src + half_blur_r;
        int y2 = y_src + half_blur_r;

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

        int excl_cnt = 0;

        x1++;
        y1++;

        for( int i = 0; i <= half_blur_r; ++i )
        {
                for( int j = 0; j <= half_blur_r; ++j )
                {
                        int xx = half_blur_r - j;
                        int yy = half_blur_r - i;

                        if( xx * xx + yy * yy > half_blur_r * half_blur_r )
                        {
                                if( y1 + i == 0 && x1 + j == 0 )
                                {
                                        avg -= src[ ( y1 + i ) * src_width + x1 + j ];
                                        avg -= src[ ( y1 + i ) * src_width + x2 - j ] - src[ ( y1 + i ) * src_width + x2 - j - 1 ];
                                        avg -= src[ ( y2 - i ) * src_width + x1 + j ] - src[ ( y2 - i - 1 ) * src_width + x1 + j ];
                                        avg -= src[ ( y2 - i ) * src_width + x2 - j ] - src[ ( y2 - i - 1 ) * src_width + x2 - j ] - src[ ( y2 - i ) * src_width + x2 - j - 1 ] + src[ ( y2 - i - 1 ) * src_width + x2 - j - 1 ];
                                }
                                else if( y1 + i == 0 )
                                {
                                        avg -= src[ ( y1 + i ) * src_width + x1 + j ] - src[ ( y1 + i ) * src_width + x1 + j - 1 ];
                                        avg -= src[ ( y1 + i ) * src_width + x2 - j ] - src[ ( y1 + i ) * src_width + x2 - j - 1 ];
                                        avg -= src[ ( y2 - i ) * src_width + x1 + j ] - src[ ( y2 - i - 1 ) * src_width + x1 + j ] - src[ ( y2 - i ) * src_width + x1 + j - 1 ] + src[ ( y2 - i - 1 ) * src_width + x1 + j - 1 ];
                                        avg -= src[ ( y2 - i ) * src_width + x2 - j ] - src[ ( y2 - i - 1 ) * src_width + x2 - j ] - src[ ( y2 - i ) * src_width + x2 - j - 1 ] + src[ ( y2 - i - 1 ) * src_width + x2 - j - 1 ];
                                }
                                else if( x1 + j == 0 )
                                {
                                        avg -= src[ ( y1 + i ) * src_width + x1 + j ] - src[ ( y1 + i - 1 ) * src_width + x1 + j ];
                                        avg -= src[ ( y1 + i ) * src_width + x2 - j ] - src[ ( y1 + i ) * src_width + x2 - j - 1 ] - src[ ( y1 + i - 1 ) * src_width + x2 - j ] + src[ ( y1 + i - 1 ) * src_width + x2 - j - 1 ];
                                        avg -= src[ ( y2 - i ) * src_width + x1 + j ] - src[ ( y2 - i - 1 ) * src_width + x1 + j ];
                                        avg -= src[ ( y2 - i ) * src_width + x2 - j ] - src[ ( y2 - i - 1 ) * src_width + x2 - j ] - src[ ( y2 - i ) * src_width + x2 - j - 1 ] + src[ ( y2 - i - 1 ) * src_width + x2 - j - 1 ];
                                }
                                else
                                {
                                        avg -= src[ ( y1 + i ) * src_width + x1 + j ] - src[ ( y1 + i - 1 ) * src_width + x1 + j ] - src[ ( y1 + i ) * src_width + x1 + j - 1 ] + src[ ( y1 + i - 1 ) * src_width + x1 + j - 1 ];
                                        avg -= src[ ( y1 + i ) * src_width + x2 - j ] - src[ ( y1 + i - 1 ) * src_width + x2 - j ] - src[ ( y1 + i ) * src_width + x2 - j - 1 ] + src[ ( y1 + i - 1 ) * src_width + x2 - j - 1 ];
                                        avg -= src[ ( y2 - j ) * src_width + x1 + j ] - src[ ( y2 - j - 1 ) * src_width + x1 + j ] - src[ ( y2 - i ) * src_width + x1 + j - 1 ] + src[ ( y2 - i - 1 ) * src_width + x1 + j - 1 ];
                                        avg -= src[ ( y2 - j ) * src_width + x2 - j ] - src[ ( y2 - j - 1 ) * src_width + x2 - j ] - src[ ( y2 - i ) * src_width + x2 - j - 1 ] + src[ ( y2 - i - 1 ) * src_width + x2 - j - 1 ];
                                }

                                excl_cnt += 4;
                        }
                }
        }

        avg /= blur_radius * blur_radius - excl_cnt;

        avg *= 2;

        dest[ index ] = avg;
}
