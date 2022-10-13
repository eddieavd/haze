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

#define DEFAULT_LIB_PATH "./lib/ops.metallib" // make this configurable


namespace haze
{


template< typename Pixel >
class metal_ops
{
public:
        MTL::Device * device_;

        metal_ops ( MTL::Device * device );

        void blocking_id ( std::vector< MTL::Buffer * > const & buffers, std::size_t arr_len, char const * method );

        void blur_image ( image< Pixel > const & src, image< Pixel > & dest, int blur_radius );

        void blur_image ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );

        void blur_pixel_field ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len );
private:
        std::map< std::string, MTL::Function * > f_map_;
        std::map< std::string, MTL::ComputePipelineState * > f_pipe_map_;

        MTL::CommandQueue * command_q_;
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
void metal_ops< Pixel >::blocking_id ( std::vector< MTL::Buffer * > const & buffers, std::size_t arr_len, char const * method )
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
        MTL::Size grid_size         = MTL::Size::Make(                      arr_len, 1, 1 );

        // encode compute command
        compute_encoder->dispatchThreads( grid_size, thread_group_size );
        compute_encoder->endEncoding();

        command_buffer->commit();
        command_buffer->waitUntilCompleted();
}

template< typename Pixel >
void metal_ops< Pixel >::blur_image ( image< Pixel > const & src, image< Pixel > & dest, int blur_radius )
{
        auto dest_width  = src.width()  - blur_radius;
        auto dest_height = src.height() - blur_radius;

        unsigned const  src_size = src.width() * src.height() * sizeof( char );
        unsigned const dest_size = dest_width  * dest_height  * sizeof( char );
        unsigned const meta_size = 3 * sizeof( int );

        MTL::Buffer *  src_gpu = device_->newBuffer(  src_size, MTL::ResourceStorageModeManaged );
        MTL::Buffer * dest_gpu = device_->newBuffer( dest_size, MTL::ResourceStorageModeManaged );
        MTL::Buffer * meta_gpu = device_->newBuffer( meta_size, MTL::ResourceStorageModeManaged );

        HAZE_ASSERT( src_gpu != nullptr && dest_gpu != nullptr && meta_gpu != nullptr, "HAZEgpu::metal_ops::blur_image: failed to create gpu buffers" );

        char *  src_ptr = static_cast< char * >(  src_gpu->contents() );
        char * dest_ptr = static_cast< char * >( dest_gpu->contents() );
        int  * meta_ptr = static_cast< int  * >( meta_gpu->contents() );

        meta_ptr[ 0 ] = src.width();
        meta_ptr[ 1 ] = src.width() - blur_radius;
        meta_ptr[ 2 ] = blur_radius;

        for( std::size_t c = 0; c < sizeof( Pixel ); ++c )
        {
                for( std::size_t i = 0; i < src.height(); ++i )
                {
                        for( std::size_t j = 0; j < src.width(); ++j )
                        {
                                src_ptr[ i * src.width() + j ] = src.at( i, j ).values[ c ];
                        }
                }

                blur_image( src_gpu, dest_gpu, meta_gpu, dest_width * dest_height );

                for( std::size_t i = 0; i < dest_height; ++i )
                {
                        for( std::size_t j = 0; j < dest_width; ++j )
                        {
                                dest.at( i, j ).values[ c ] = dest_ptr[ i * dest_width + j ];
                        }
                }
        }
}

template< typename Pixel >
void metal_ops< Pixel >::blur_image ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        char const * method = "mean_blur_kern";

        this->blocking_id( buffers, dest_len, method );
}

template< typename Pixel >
void metal_ops< Pixel >::blur_pixel_field ( MTL::Buffer * src, MTL::Buffer * dest, MTL::Buffer * metadata, std::size_t const dest_len )
{
        std::vector< MTL::Buffer * > buffers( { src, dest, metadata } );

        char const * method = "mean_blur_prefix";

        this->blocking_id( buffers, dest_len, method );
}


} // namespace haze
