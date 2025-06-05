//
//
//      haze
//      gfx/backend/metal/allocator.hxx
//

#pragma once

#include <uti/core/allocator/meta.hxx>
#include <uti/core/allocator/resource.hxx>
#include <uti/core/allocator/default.hxx>

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>
//#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/gfx/context.hxx>
#include <haze/gfx/allocator_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class gpu_memory_resource : public gpu_memory_resource_base< gpu_memory_resource >
{
        using _self = gpu_memory_resource ;
        using _base = gpu_memory_resource_base< _self > ;
public:
        using ssize_type = typename _base::ssize_type ;

        using  _base::_base ;
        friend _base        ;

        gpu_memory_resource ( context & _ctx_, allocator_config const & _config_ ) ;

        ~gpu_memory_resource () noexcept ;
private:
        context          &  ctx_ ;
        allocator_config config_ ;
        MTL::Heap        * heap_ ;

               buffer _alloc_buffer        (                  ssize_type  _size_ ) ;
        vertex_buffer _alloc_vertex_buffer (                  ssize_type  _size_ ) ;
        vertex_buffer _alloc_vertex_buffer ( float * _verts_, ssize_type  _size_ ) ;
         index_buffer _alloc_index_buffer  ( u32_t *  _idxs_, ssize_type _count_ ) ;

               buffer _alloc_buffer_at_offset        (                  ssize_type  _size_, u64_t _offset_ ) ;
        vertex_buffer _alloc_vertex_buffer_at_offset (                  ssize_type  _size_, u64_t _offset_ ) ;
        vertex_buffer _alloc_vertex_buffer_at_offset ( float * _verts_, ssize_type  _size_, u64_t _offset_ ) ;
         index_buffer _alloc_index_buffer_at_offset  ( u32_t *  _idxs_, ssize_type _count_, u64_t _offset_ ) ;

//      texture _alloc_texture        () ;
//      texture _alloc_sparse_texture () ;

//      texture _texture_from_buffer ( buffer & _buffer_ ) ;

        constexpr string_view name () const noexcept { return config_.name_ ; }

        constexpr ssize_type _capacity        (                    ) const noexcept { return heap_->currentAllocatedSize(                         ) ; }
        constexpr ssize_type _bytes_in_use    (                    ) const noexcept { return heap_->            usedSize(                         ) ; }
        constexpr ssize_type _bytes_available ( ssize_type _align_ ) const noexcept { return heap_->    maxAvailableSize( NS::UInteger( _align_ ) ) ; }

        constexpr resource_options _resource_opts () const noexcept
        {  return resource_options{ config_.storage_mode_, config_.cpu_cache_mode_, config_.hazard_tracking_mode_ } ; }

        void _log_memory_usage () const noexcept ;

        void _init ( ssize_type _init_size_ ) ;

        void _destroy () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
