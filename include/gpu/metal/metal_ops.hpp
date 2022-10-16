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

#define DEFAULT_LIB_PATH "./lib/ops.metallib" // make this configurable


namespace haze
{


template< typename Pixel >
class metal_ops
{
public:
        metal_ops ( MTL::Device * device );

        image< Pixel > mean_blur_kern ( image< Pixel > const & src, int const blur_radius );
        image< Pixel > lens_blur_kern ( image< Pixel > const & src, int const blur_radius );

        image< Pixel > mean_blur ( pixel_field< Pixel > const & src, int const blur_radius );
        image< Pixel > lens_blur ( pixel_field< Pixel > const & src, int const blur_radius );
private:
        std::map< std::string, MTL::Function * > f_map_;
        std::map< std::string, MTL::ComputePipelineState * > f_pipe_map_;

        MTL::Device       * device_;
        MTL::CommandQueue * command_q_;

        MTL::Buffer * _create_empty_buffer ( std::size_t const size );

        MTL::Buffer * _create_buffer ( image      < Pixel > const & src, std::size_t const channel );
        MTL::Buffer * _create_buffer ( pixel_field< Pixel > const & src, std::size_t const channel );

        MTL::Buffer * _create_meta_buffer ( unsigned const width, unsigned const blur_radius = 0 );

        void _blocking_id ( std::vector< MTL::Buffer * > const & buffers, std::size_t arr_len, char const * method );

        void _read_buffer ( image< Pixel > & img, MTL::Buffer * buffer, std::size_t const width, std::size_t const height, std::size_t const channel );

        void _mean_blur_kern ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
        void _lens_blur_kern ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );

        void _mean_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
        void _lens_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
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
void metal_ops< Pixel >::_blocking_id ( std::vector< MTL::Buffer * > const & buffers, std::size_t arr_len, char const * method )
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
image< Pixel > metal_ops< Pixel >::mean_blur_kern ( image< Pixel > const & src, int const blur_radius )
{
        using pixel_v_t = typename Pixel::value_type;

        auto src_width  = src.width();
        auto src_height = src.height();

        auto dest_width  = src_width  - blur_radius;
        auto dest_height = src_height - blur_radius;

        image< Pixel > dest_img( dest_width, dest_height );

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer  = _create_buffer( src, c );
                MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
                MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

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

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer  = _create_buffer( src, c );
                MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
                MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

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

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer  = _create_buffer( src, c );
                MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
                MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

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

        for( std::size_t c = 0; c < Pixel::channels; ++c )
        {
                MTL::Buffer * src_buffer  = _create_buffer( src, c );
                MTL::Buffer * dest_buffer = _create_empty_buffer( dest_width * dest_height * sizeof( pixel_v_t ) );
                MTL::Buffer * meta_buffer = _create_meta_buffer( dest_width, blur_radius );

                _lens_blur( src_buffer, dest_buffer, meta_buffer, dest_width * dest_height );

                _read_buffer( dest_img, dest_buffer, dest_width, dest_height, c );
        }

        return dest_img;
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

        this->_blocking_id( buffers, dest_len, method.c_str() );
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

        this->_blocking_id( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_mean_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "mean_blur_thicc_field";
        }
        else
        {
                method = "mean_blur_field";
        }

        this->_blocking_id( buffers, dest_len, method.c_str() );
}

template< typename Pixel >
void metal_ops< Pixel >::_lens_blur ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        std::string method;

        if constexpr( Pixel::is_thicc )
        {
                method = "lens_blur_thicc_field";
        }
        else
        {
                method = "lens_blur_field";
        }

        this->_blocking_id( buffers, dest_len, method.c_str() );
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
                for( std::size_t j = 0; j < width; ++j )
                {
                        src_ptr[ i * width + j ] = src.sum_at( i, j, channel );
                }
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
