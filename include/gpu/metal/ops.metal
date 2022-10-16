//
//
//      HAZElib
//      ops.metal
//


#include <metal_stdlib>


using namespace metal;


kernel void mean_blur_kern ( device unsigned char const * src, device unsigned char * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];

        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float avg = 0;

        for( unsigned i = row_src - half_blur_r; i < row_src + half_blur_r; ++i )
        {
                for( unsigned j = col_src - half_blur_r; j < col_src + half_blur_r; ++j )
                {
                        avg += src[ i * src_width + j ];
                }
        }

        avg /= blur_radius * blur_radius;

        dest[ index ] = static_cast< unsigned char >( avg );
}

kernel void mean_blur_kern_thicc ( device unsigned long const * src, device unsigned long * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];

        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float avg = 0;

        for( unsigned i = row_src - half_blur_r; i < row_src + half_blur_r; ++i )
        {
                for( unsigned j = col_src - half_blur_r; j < col_src + half_blur_r; ++j )
                {
                        avg += src[ i * src_width + j ];
                }
        }

        avg /= blur_radius * blur_radius;

        dest[ index ] = static_cast< unsigned long >( avg );
}

kernel void lens_blur_kern ( device unsigned char const * src, device unsigned char * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];

        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float    avg      = 0;
        unsigned incl_cnt = 0;

        for( unsigned i = row_src - half_blur_r; i < row_src + half_blur_r; ++i )
        {
                for( unsigned j = col_src - half_blur_r; j < col_src + half_blur_r; ++j )
                {
                        unsigned kern_x = j + half_blur_r - col_src;
                        unsigned kern_y = i + half_blur_r - row_src;

                        unsigned xx = half_blur_r - kern_x;
                        unsigned yy = half_blur_r - kern_y;

                        if( xx * xx + yy * yy > half_blur_r * half_blur_r )
                        {
                                avg += src[ i * src_width + j ];

                                incl_cnt++;
                        }
                }
        }

        avg /= incl_cnt;

        dest[ index ] = static_cast< unsigned char >( avg );
}

kernel void lens_blur_kern_thicc ( device unsigned long const * src, device unsigned long * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];

        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float    avg      = 0;
        unsigned incl_cnt = 0;

        for( unsigned i = row_src - half_blur_r; i < row_src + half_blur_r; ++i )
        {
                for( unsigned j = col_src - half_blur_r; j < col_src + half_blur_r; ++j )
                {
                        unsigned kern_x = j + half_blur_r - col_src;
                        unsigned kern_y = i + half_blur_r - row_src;

                        unsigned xx = half_blur_r - kern_x;
                        unsigned yy = half_blur_r - kern_y;

                        if( xx * xx + yy * yy > half_blur_r * half_blur_r )
                        {
                                avg += src[ i * src_width + j ];

                                incl_cnt++;
                        }
                }
        }

        avg /= incl_cnt;

        dest[ index ] = static_cast< unsigned long >( avg );
}

kernel void mean_blur_field ( device unsigned long const * src, device unsigned char * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];
        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float avg = 0;

        if( row == 0 && col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ];
        }
        else if( row == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }
        else if( col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ];
        }
        else
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ]
                                                                                               - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ]
                                                                                               + src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }

        avg /= blur_radius * blur_radius;

        if( avg > 255 )
        {
                avg = 255;
        }

        dest[ index ] = static_cast< unsigned char >( avg );
}

kernel void mean_blur_thicc_field ( device unsigned long const * src, device unsigned long * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];
        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float avg = 0;

        if( row == 0 && col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ];
        }
        else if( row == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }
        else if( col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ];
        }
        else
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ]
                                                                                               - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ]
                                                                                               + src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }

        avg /= blur_radius * blur_radius;

        if( avg > 255 )
        {
                avg = 255;
        }

        dest[ index ] = static_cast< unsigned long >( avg );
}

kernel void lens_blur_field ( device unsigned long const * src, device unsigned char * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];
        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float avg = 0;

        if( row == 0 && col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ];
        }
        else if( row == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }
        else if( col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ];
        }
        else
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ]
                                                                                               - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ]
                                                                                               + src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }

        unsigned x1 = col_src - half_blur_r;
        unsigned y1 = row_src - half_blur_r;
        unsigned x2 = col_src + half_blur_r;
        unsigned y2 = row_src + half_blur_r;

        unsigned excl_cnt = 0;

        for( unsigned y = 0; y < half_blur_r; ++y )
        {
                for( unsigned x = 0; x < half_blur_r; ++x )
                {
                        unsigned xx = half_blur_r - x;
                        unsigned yy = half_blur_r - y;

                        if( xx * xx + yy * yy > half_blur_r * half_blur_r )
                        {
                                if( x1 + x == 0 && y1 + y == 0 )
                                {
                                        avg -= src[ ( y1 + y ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y1 + y ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];
                                }
                                else if( x1 + x == 0 )
                                {
                                        avg -= src[ ( y1 + y     ) * src_width + ( x1 + x ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y1 + y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                }
                                else if( y1 + y == 0 )
                                {
                                        avg -= src[ ( y1 + y ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y1 + y ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y1 + y ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x1 + x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];
                                }
                                else
                                {
                                        avg -= src[ ( y1 + y     ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y1 + y     ) * src_width + ( x1 + x - 1 ) ] +
                                               src[ ( y1 + y - 1 ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y1 + y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x1 + x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                }

                                excl_cnt += 4;
                        }
                        else
                        {
                                break;
                        }
                }
        }

        avg /= ( blur_radius * blur_radius ) - excl_cnt;

        if( avg > 255 )
        {
                avg = 255;
        }

        dest[ index ] = static_cast< unsigned char >( avg );
}

kernel void lens_blur_thicc_field ( device unsigned long const * src, device unsigned long * dest, device unsigned const * meta, unsigned index [[ thread_position_in_grid ]] )
{
        unsigned img_width   = meta[ 0 ];
        unsigned blur_radius = meta[ 1 ];
        unsigned half_blur_r = blur_radius / 2;
        unsigned src_width   = img_width + blur_radius;

        unsigned row = index / img_width;
        unsigned col = index % img_width;

        unsigned row_src = row + half_blur_r;
        unsigned col_src = col + half_blur_r;

        float avg = 0;

        if( row == 0 && col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ];
        }
        else if( row == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }
        else if( col == 0 )
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ];
        }
        else
        {
                avg = src[ ( row_src + half_blur_r ) * src_width + ( col_src + half_blur_r ) ] - src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src + half_blur_r ) ]
                                                                                               - src[ ( row_src + half_blur_r ) * src_width + ( col_src - half_blur_r - 1 ) ]
                                                                                               + src[ ( row_src - half_blur_r - 1 ) * src_width + ( col_src - half_blur_r - 1 ) ];
        }

        unsigned x1 = col_src - half_blur_r;
        unsigned y1 = row_src - half_blur_r;
        unsigned x2 = col_src + half_blur_r;
        unsigned y2 = row_src + half_blur_r;

        unsigned excl_cnt = 0;

        for( unsigned y = 0; y < half_blur_r; ++y )
        {
                for( unsigned x = 0; x < half_blur_r; ++x )
                {
                        unsigned xx = half_blur_r - x;
                        unsigned yy = half_blur_r - y;

                        if( xx * xx + yy * yy > half_blur_r * half_blur_r )
                        {
                                if( x1 + x == 0 && y1 + y == 0 )
                                {
                                        avg -= src[ ( y1 + y ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y1 + y ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];
                                }
                                else if( x1 + x == 0 )
                                {
                                        avg -= src[ ( y1 + y     ) * src_width + ( x1 + x ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y1 + y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                }
                                else if( y1 + y == 0 )
                                {
                                        avg -= src[ ( y1 + y ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y1 + y ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y1 + y ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x1 + x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];
                                }
                                else
                                {
                                        avg -= src[ ( y1 + y     ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y1 + y     ) * src_width + ( x1 + x - 1 ) ] +
                                               src[ ( y1 + y - 1 ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y1 + y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y1 + y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y1 + y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x1 + x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x1 + x - 1 ) ];

                                        avg -= src[ ( y2 - y     ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x     ) ] -
                                               src[ ( y2 - y     ) * src_width + ( x2 - x - 1 ) ] +
                                               src[ ( y2 - y - 1 ) * src_width + ( x2 - x - 1 ) ];

                                }

                                excl_cnt += 4;
                        }
                        else
                        {
                                break;
                        }
                }
        }

        avg /= ( blur_radius * blur_radius ) - excl_cnt;

        if( avg > 255 )
        {
                avg = 255;
        }

        dest[ index ] = static_cast< unsigned long >( avg );
}
