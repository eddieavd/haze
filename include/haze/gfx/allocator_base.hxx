//
//
//      haze
//      gfx/allocator_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/common.hxx>
#include <haze/gfx/context.hxx>
#include <haze/gfx/buffer.hxx>


namespace haze
{

////////////////////////////////////////////////////////////////////////////////

struct allocator_config
{
        string                               name_ ; // replace with small_string, force smaller name, avoid allocation
        placement_strategy     placement_strategy_ ;
        cpu_cache_mode             cpu_cache_mode_ ;
        storage_mode                 storage_mode_ ;
        hazard_tracking_mode hazard_tracking_mode_ ;
        sparse_page_size         sparse_page_size_ ;
} ;

////////////////////////////////////////////////////////////////////////////////

static /* constexpr */ const allocator_config dynamic_allocator_config =
{
        .                name_ = string( "dynamic" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy::automatic ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     dflt ,
        .        storage_mode_ = haze::mtl::        storage_mode::   shared ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode::untracked ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size::  size_16 ,
} ;
static /* constexpr */ const allocator_config static_allocator_config =
{
        .                name_ = string( "static" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy::placement ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     dflt ,
        .        storage_mode_ = haze::mtl::        storage_mode::   shared ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode::untracked ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size::  size_16 ,
} ;
static /* constexpr */ const allocator_config writeonly_dynamic_allocator_config =
{
        .                name_ = string( "writeonly_dynamic" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy:: automatic  ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     write  ,
        .        storage_mode_ = haze::mtl::        storage_mode::gpu_private ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode:: untracked  ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size::   size_16  ,
} ;
static /* constexpr */ const allocator_config writeonly_static_allocator_config =
{
        .                name_ = string( "writeonly_static" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy:: placement  ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     write  ,
        .        storage_mode_ = haze::mtl::        storage_mode::gpu_private ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode:: untracked  ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size::   size_16  ,
} ;
static /* constexpr */ const allocator_config sparse16_allocator_config =
{
        .                name_ = string( "sparse16" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy::automatic ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     dflt ,
        .        storage_mode_ = haze::mtl::        storage_mode::   shared ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode::untracked ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size::  size_16 ,
} ;
static /* constexpr */ const allocator_config sparse64_allocator_config =
{
        .                name_ = string( "sparse64" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy::automatic ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     dflt ,
        .        storage_mode_ = haze::mtl::        storage_mode::   shared ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode::untracked ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size::  size_64 ,
} ;
static /* constexpr */ const allocator_config sparse256_allocator_config =
{
        .                name_ = string( "sparse256" ) ,
        .  placement_strategy_ = haze::mtl::  placement_strategy::automatic ,
        .      cpu_cache_mode_ = haze::mtl::      cpu_cache_mode::     dflt ,
        .        storage_mode_ = haze::mtl::        storage_mode::   shared ,
        .hazard_tracking_mode_ = haze::mtl::hazard_tracking_mode::untracked ,
        .    sparse_page_size_ = haze::mtl::    sparse_page_size:: size_256 ,
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename ResourceImpl >
class gpu_memory_resource_base
{
        using _impl = ResourceImpl ;
public:
        using  size_type =  size_t ;
        using ssize_type = ssize_t ;

        constexpr        buffer alloc_buffer        (             ssize_type  _size_ ) { return static_cast< _impl * >( this )->_alloc_buffer       (          _size_ ) ; }
//      constexpr vertex_buffer alloc_vertex_buffer (                  u32_t  _size_ ) { return static_cast< _impl * >( this )->_alloc_vertex_buffer(          _size_ ) ; }
//      constexpr vertex_buffer alloc_vertex_buffer ( float * _verts_, u32_t  _size_ ) { return static_cast< _impl * >( this )->_alloc_vertex_buffer( _verts_, _size_ ) ; }
//      constexpr  index_buffer alloc_index_buffer  ( u32_t *  _idxs_, u32_t _count_ ) { return static_cast< _impl * >( this )->_alloc_index_buffer ( _idxs_, _count_ ) ; }

        constexpr buffer alloc_buffer_at_offset ( ssize_type _size_, u64_t _offset_ )
        { return static_cast< _impl * >( this )->_alloc_buffer_at_offset( _size_, _offset_ ) ; }

//      constexpr vertex_buffer alloc_vertex_buffer_at_offset ( u32_t _size_, u64_t _offset_ )
//      { return static_cast< _impl * >( this )->_alloc_vertex_buffer_at_offset( _size_, _offset_ ) ; }

//      constexpr vertex_buffer alloc_vertex_buffer_at_offset ( float * _verts_, u32_t _size_, u64_t _offset_ )
//      { return static_cast< _impl * >( this )->_alloc_vertex_buffer_at_offset( _verts_, _size_, _offset_ ) ; }

//      constexpr index_buffer alloc_index_buffer_at_offset ( u32_t * _idxs_, u32_t _count_, u64_t _offset_ )
//      { return static_cast< _impl * >( this )->_alloc_index_buffer_at_offset( _idxs_, _count_, _offset_ ) ; }

//      constexpr        texture alloc_texture        () { return static_cast< _impl * >( this )->_alloc_texture       () ; }
//      constexpr sparse_texture alloc_sparse_texture () { return static_cast< _impl * >( this )->_alloc_sparse_texture() ; }

//      constexpr texture texture_from_buffer ( buffer & _buffer_ ) { return static_cast< _impl * >( this )->_texture_from_buffer( _buffer_ ) ; }

//      constexpr ssize_type           size  (                    ) const noexcept { return static_cast< _impl const * >( this )->          _size (         ) ; }
//      constexpr ssize_type allocated_size  (                    ) const noexcept { return static_cast< _impl const * >( this )->_allocated_size (         ) ; }

        constexpr ssize_type capacity        (                    ) const noexcept { return static_cast< _impl const * >( this )->_capacity       (         ) ; }
        constexpr ssize_type bytes_in_use    (                    ) const noexcept { return static_cast< _impl const * >( this )->_bytes_in_use   (         ) ; }
        constexpr ssize_type bytes_available ( ssize_type _align_ ) const noexcept { return static_cast< _impl const * >( this )->_bytes_available( _align_ ) ; }

        constexpr resource_options resource_opts () const noexcept { return static_cast< _impl const * >( this )->_resource_opts() ; }

        constexpr void log_memory_usage () const noexcept { static_cast< _impl const * >( this )->_log_memory_usage() ; }

        constexpr void init ( ssize_type _init_size_ ) { static_cast< _impl * >( this )->_init( _init_size_ ) ; }

        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }
protected:
        constexpr  gpu_memory_resource_base () noexcept = default ;
        constexpr ~gpu_memory_resource_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
