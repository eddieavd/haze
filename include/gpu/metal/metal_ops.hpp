//
//
//      HAZElib
//      metal_ops.hpp
//

#pragma once


#include <iostream>
#include <vector>
#include <map>

#define  NS_PRIVATE_IMPLEMENTATION
#define  CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Metal.hpp"

#include "../../image.hpp"
#include "../../img_manip.hpp"
#include "../../img_transform.hpp"

#define DEFAULT_LIB_PATH "./lib/ops.metallib" // make this configurable

#define META_VERTICAL   0
#define META_HORIZONTAL 1


namespace haze
{


template< typename Pixel >
class metal_ops
{
public:
        metal_ops ( MTL::Device * device );

        template< std::size_t Size >
        image< Pixel > transform_image ( image< Pixel > const & src, kernel< Size, Size > const & kern );

        template< std::size_t Size >
        image< Pixel > transform_image ( image< Pixel > const & src, separable_kernel< Size > const & kern );

        image< Pixel > detect_edges ( image< Pixel > const & src );

        image< Pixel > mean_blur_kern ( image< Pixel > const & src, int const blur_radius );
        image< Pixel > lens_blur_kern ( image< Pixel > const & src, int const blur_radius );

        image< Pixel > mean_blur ( pixel_field< Pixel > const & src, int const blur_radius );
        image< Pixel > lens_blur ( pixel_field< Pixel > const & src, int const blur_radius );

        image< Pixel > lens_blur_direct ( pixel_field< Pixel > const & src, int const blur_radius );
private:
        std::map< std::string, MTL::Function * > f_map_;
        std::map< std::string, MTL::ComputePipelineState * > f_pipe_map_;

        MTL::Device       * device_;
        MTL::CommandQueue * command_q_;

        MTL::Buffer * _create_empty_buffer ( std::size_t const size );

        MTL::Buffer * _create_buffer ( image      < Pixel > const & src, std::size_t const channel );
        MTL::Buffer * _create_buffer ( pixel_field< Pixel > const & src, std::size_t const channel );

        MTL::Buffer * _create_meta_buffer ( unsigned const width, unsigned const blur_radius = 0 );

        template< std::size_t Size >
        MTL::Buffer * _create_meta_buffer ( unsigned const width, kernel< Size, Size > const & kern );

        template< std::size_t Size >
        MTL::Buffer * _create_meta_buffer ( unsigned const width, separable_kernel< Size > const & kern, int const orientation );

        void _exec_blocking ( std::vector< MTL::Buffer * > const & buffers, std::size_t arr_len, char const * method );

        void _read_buffer ( image< Pixel > & img, MTL::Buffer * buffer, std::size_t const width, std::size_t const height, std::size_t const channel );

        void _transform_image      ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
        void _transform_vertical   ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
        void _transform_horizontal ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );

        void _geometric_combine ( MTL::Buffer * lhs, MTL::Buffer * rhs, MTL::Buffer * dest, std::size_t const dest_len );

        void _mean_blur_kern ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
        void _lens_blur_kern ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );

        void _mean_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
        void _lens_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );

        void _lens_blur_direct ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
};

template< typename Pixel >
metal_ops< Pixel >::metal_ops ( MTL::Device * device )
{
        this->device_ = device;

        NS::Error * err = nullptr;

        auto filepath = NS::String::string( DEFAULT_LIB_PATH, NS::ASCIIStringEncoding );

        MTL::Library * op_lib = device_->newLibrary( filepath, &err );

        HAZE_ASSERT( op_lib != nullptr, "HAZEgpu::metal_ops::ctor: failed to find default library with error: " << err->description()->utf8String() );

        auto fn_names = op_lib->functionNames();

        for( std::size_t i = 0; i < fn_names->count(); ++i )
        {
                auto name_nsstring = fn_names->object( i )->description();
                auto name_utf8     = name_nsstring->utf8String();

                this->f_map_[ name_utf8 ] =
                        ( op_lib->newFunction( name_nsstring ) );

                this->f_pipe_map_[ name_utf8 ] =
                        this->device_->newComputePipelineState( this->f_map_[ name_utf8 ], &err );

                HAZE_ASSERT( this->f_pipe_map_[ name_utf8 ] != nullptr, "HAZEgpu::metal_ops::ctor: failed to create pipeline state object for " << name_utf8 << " with error: " << err->description()->utf8String() );
        }

        this->command_q_ = device_->newCommandQueue();

        HAZE_ASSERT( this->command_q_ != nullptr, "HAZEgpu::metal_ops::ctor: failed to create command queue" );
}

template< typename Pixel >
void metal_ops< Pixel >::_exec_blocking ( std::vector< MTL::Buffer * > const & buffers, std::size_t arr_len, char const * method )
{
        // create new buffer
        MTL::CommandBuffer * command_buffer = this->command_q_->commandBuffer();
        HAZE_ASSERT( command_buffer != nullptr, "HAZEgpu::metal_ops::blocking_id: failed to create command buffer" );

        // create new encoder
        MTL::ComputeCommandEncoder * compute_encoder = command_buffer->computeCommandEncoder();
        HAZE_ASSERT( compute_encoder != nullptr, "HAZEgpu::metal_ops::blocking_id: failed to create compute encoder" );

        // set pipeline
        compute_encoder->setComputePipelineState( this->f_pipe_map_[ method ] );

        for( std::size_t i = 0; i < buffers.size(); ++i )
        {
                compute_encoder->setBuffer( buffers[ i ], 0, i );
        }

        // compute thread_group_size
        NS::UInteger potential_thread_group_size =
                this->f_pipe_map_[ method ]->maxTotalThreadsPerThreadgroup();

        if( potential_thread_group_size > arr_len )
        {
                potential_thread_group_size = arr_len;
        }

        MTL::Size thread_group_size = MTL::Size::Make( potential_thread_group_size, 1, 1 );
        MTL::Size grid_size         = MTL::Size::Make(                     arr_len, 1, 1 );

        // encode compute command
        compute_encoder->dispatchThreads( grid_size, thread_group_size );
        compute_encoder->endEncoding();

        command_buffer->commit();
        command_buffer->waitUntilCompleted();
}

template< typename Pixel >
template< std::size_t Size >
image< Pixel > metal_ops< Pixel >::transform_image ( image< Pixel > const & src, kernel< Size, Size > const & kern )
{
        using pixel_v_t = typename Pixel::value_type;

        std::size_t kernel_size = Size;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - kernel_size;
        auto dest_height = src_height - kernel_size;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, kern );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _transform_image( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
template< std::size_t Size >
image< Pixel > metal_ops< Pixel >::transform_image ( image< Pixel > const & src, separable_kernel< Size > const & kern )
{
        using pixel_v_t = typename Pixel::value_type;

        std::size_t kernel_size = Size;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto tmp_width  = src_width;
        auto tmp_height = src_height - kernel_size;

//        auto dest_width  = tmp_width - kernel_size;
//        auto dest_height = tmp_height;

        auto dest_width  = tmp_width;
        auto dest_height = tmp_height;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer *    tmp_buffer = _create_empty_buffer(  tmp_width *  tmp_height * sizeof( pixel_v_t ) );
//        MTL::Buffer *   dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_v_buffer = _create_meta_buffer(  tmp_width, kern, META_VERTICAL   );
//        MTL::Buffer * meta_h_buffer = _create_meta_buffer( dest_width, kern, META_HORIZONTAL );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _transform_vertical  ( src_buffer,  tmp_buffer, meta_v_buffer,  tmp_width *  tmp_height );
//                _transform_horizontal( tmp_buffer, dest_buffer, meta_h_buffer, dest_width * dest_height );

                _read_buffer( dest_img, tmp_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
image< Pixel > metal_ops< Pixel >::detect_edges ( image< Pixel > const & src )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - sobel_v.size;
        auto dest_height = src_height - sobel_v.size;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * v_edge_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * h_edge_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer *   dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * v_meta_buffer = _create_meta_buffer( dest_width, sobel_v );
        MTL::Buffer * h_meta_buffer = _create_meta_buffer( dest_width, sobel_h );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _transform_image( src_buffer, v_edge_buffer, v_meta_buffer, dest_width * dest_height );
                _transform_image( src_buffer, h_edge_buffer, h_meta_buffer, dest_width * dest_height );

                _geometric_combine( v_edge_buffer, h_edge_buffer, dest_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
image< Pixel > metal_ops< Pixel >::mean_blur_kern ( image< Pixel > const & src, int const blur_radius )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - blur_radius;
        auto dest_height = src_height - blur_radius;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _mean_blur_kern( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
image< Pixel > metal_ops< Pixel >::lens_blur_kern ( image< Pixel > const & src, int const blur_radius )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - blur_radius;
        auto dest_height = src_height - blur_radius;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _lens_blur_kern( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
image< Pixel > metal_ops< Pixel >::mean_blur ( pixel_field< Pixel > const & src, int const blur_radius )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - blur_radius;
        auto dest_height = src_height - blur_radius;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _mean_blur( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
image< Pixel > metal_ops< Pixel >::lens_blur ( pixel_field< Pixel > const & src, int const blur_radius )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - blur_radius;
        auto dest_height = src_height - blur_radius;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _lens_blur( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
image< Pixel > metal_ops< Pixel >::lens_blur_direct ( pixel_field< Pixel > const & src, int const blur_radius )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - blur_radius;
        auto dest_height = src_height - blur_radius;

        image< Pixel > dest_img( dest_width, dest_height );

        MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
        MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer = _create_buffer( src, c );

                _lens_blur_direct( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
}

template< typename Pixel >
void metal_ops< Pixel >::_transform_image ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "transform_thicc";
        }
        else
        {
                method = "transform";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_transform_vertical ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "transform_vert_thicc";
        }
        else
        {
                method = "transform_vert";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_transform_horizontal ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "transform_hor_thicc";
        }
        else
        {
                method = "transform_hor";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_geometric_combine ( MTL::Buffer * lhs, MTL::Buffer * rhs, MTL::Buffer * dest, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { lhs, rhs, dest } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "geometric_combine_thicc";
        }
        else
        {
                method = "geometric_combine";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_mean_blur_kern ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "mean_blur_kern_thicc";
        }
        else
        {
                method = "mean_blur_kern";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_lens_blur_kern ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "lens_blur_kern_thicc";
        }
        else
        {
                method = "lens_blur_kern";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_mean_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "mean_blur_field_thicc";
        }
        else
        {
                method = "mean_blur_field";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_lens_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "lens_blur_field_thicc";
        }
        else
        {
                method = "lens_blur_field";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_lens_blur_direct ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "lens_blur_field_direct_thicc";
        }
        else
        {
                method = "lens_blur_field_direct";
        }

        this->_exec_blocking( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
MTL::Buffer * metal_ops< Pixel >::_create_empty_buffer ( std::size_t const size )
{
        MTL::Buffer * buff = device_->newBuffer( size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( buff != nullptr, "HAZEgpu::metal_ops::_create_empty_buffer: failed to create gpu buffer" );

        return buff;
}

template< typename Pixel >
MTL::Buffer * metal_ops< Pixel >::_create_buffer ( image< Pixel > const & src, std::size_t const channel )
{
        using pixel_v_t = typename Pixel::value_type;

        auto width  = src.width();
        auto height = src.height();

        unsigned const src_size = width * height * sizeof( pixel_v_t );

        MTL::Buffer * src_gpu = device_->newBuffer( src_size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( src_gpu != nullptr, "HAZEgpu::metal_ops::_create_buffer: failed to create gpu buffer" );

        pixel_v_t * src_ptr = static_cast< pixel_v_t * >( src_gpu->contents() );

        for( std::size_t i = 0; i < height; ++i )
        {
                for( std::size_t j = 0; j < width; ++j )
                {
                        src_ptr[ i * width + j ] = src.at( i, j ).values[ channel ];
                }
        }

        return src_gpu;
}

template< typename Pixel >
MTL::Buffer * metal_ops< Pixel >::_create_buffer ( pixel_field< Pixel > const & src, std::size_t const channel )
{
        using field_v_t = typename pixel_field< Pixel >::value_type;

        auto width  = src.width();
        auto height = src.height();

        unsigned const src_size = width * height * sizeof( field_v_t );

        MTL::Buffer * src_gpu = device_->newBuffer( src_size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( src_gpu != nullptr, "HAZEgpu::metal_ops::_create_buffer: failed to create gpu buffer" );

        field_v_t * src_ptr = static_cast< field_v_t * >( src_gpu->contents() );

        for( std::size_t i = 0; i < height; ++i )
        {
                std::memcpy( src_ptr + ( i * width ), src.channel_row_data( i, channel ), width * sizeof( field_v_t ) );
        }

        return src_gpu;
}

template< typename Pixel >
MTL::Buffer * metal_ops< Pixel >::_create_meta_buffer ( unsigned const width, unsigned const blur_radius )
{
        unsigned const meta_size = 2 * sizeof( unsigned );

        MTL::Buffer * meta_gpu = device_->newBuffer( meta_size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( meta_gpu != nullptr, "HAZEgpu::metal_ops::_create_meta_buffer: failed to create gpu buffer" );

        unsigned * meta_ptr = static_cast< unsigned * >( meta_gpu->contents() );

        meta_ptr[ 0 ] = width;
        meta_ptr[ 1 ] = blur_radius;

        return meta_gpu;
}

template< typename Pixel >
template< std::size_t Size >
MTL::Buffer * metal_ops< Pixel >::_create_meta_buffer ( unsigned const width, kernel< Size, Size > const & kern )
{
        unsigned const meta_size = ( 3 + Size * Size ) * sizeof( float );

        MTL::Buffer * meta_gpu = device_->newBuffer( meta_size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( meta_gpu != nullptr, "HAZEgpu::metal_ops::_create_meta_buffer: failed to create gpu buffer" );

        float * meta_ptr = static_cast< float * >( meta_gpu->contents() );

        meta_ptr[ 0 ] = static_cast< float >( width );
        meta_ptr[ 1 ] = static_cast< float >( Size );
        meta_ptr[ 2 ] = kern.weight;

        for( std::size_t i = 0; i < Size; ++i )
        {
                for( std::size_t j = 0; j < Size; ++j )
                {
                        meta_ptr[ i * Size + j + 3 ] = kern.vals[ i ][ j ];
                }
        }

        return meta_gpu;
}

template< typename Pixel >
template< std::size_t Size >
MTL::Buffer * metal_ops< Pixel >::_create_meta_buffer ( unsigned const width, separable_kernel< Size > const & kern, int const orientation )
{
        unsigned const meta_size = ( 3 + Size ) * sizeof( float );

        MTL::Buffer * meta_gpu = device_->newBuffer( meta_size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( meta_gpu != nullptr, "HAZEgpu::metal_ops::_create_meta_buffer: failed to create gpu buffer" );

        float * meta_ptr = static_cast< float * >( meta_gpu->contents() );

        meta_ptr[ 0 ] = static_cast< float >( width );
        meta_ptr[ 1 ] = static_cast< float >( Size );
        meta_ptr[ 2 ] = orientation == META_VERTICAL ? kern.weight_v : kern.weight_h;

        if( orientation == META_VERTICAL )
        {
                for( std::size_t i = 0; i < Size; ++i )
                {
                        meta_ptr[ i + 3 ] = kern.vertical[ i ];
                }
        }
        else if( orientation == META_HORIZONTAL )
        {
                for( std::size_t i = 0; i < Size; ++i )
                {
                        meta_ptr[ i + 3 ] = kern.horizontal[ i ];
                }
        }
        else
        {
                for( std::size_t i = 0; i < Size; ++i )
                {
                        meta_ptr[ i + 3 ] = 1;
                }
        }

        return meta_gpu;
}

template< typename Pixel >
void metal_ops< Pixel >::_read_buffer ( image< Pixel > & img, MTL::Buffer * buffer, std::size_t const width, std::size_t const height, std::size_t const channel )
{
        using pixel_v_t = typename Pixel::value_type;

        pixel_v_t * buff_ptr = static_cast< pixel_v_t * >( buffer->contents() );

        for( std::size_t i = 0; i < height; ++i )
        {
                for( std::size_t j = 0; j < width; ++j )
                {
                        img.at( i, j ).values[ channel ] = buff_ptr[ i * width + j ];
                }
        }
}


} // namespace haze
