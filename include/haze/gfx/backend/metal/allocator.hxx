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
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/gfx/context.hxx>
#include <haze/gfx/allocator_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class metal_memory_resource : public gpu_memory_resource_base< metal_memory_resource >
{
        using _self = metal_memory_resource ;
        using _base = gpu_memory_resource_base< _self > ;
public:
        using  size_type = typename _base:: size_type ;
        using ssize_type = typename _base::ssize_type ;

        using  _base::_base ;
        friend _base        ;

        constexpr metal_memory_resource ( context & _ctx_                                                            ) ;
        constexpr metal_memory_resource ( context & _ctx_, allocator_config const & _config_, ssize_type _init_size_ ) ;

        constexpr ~metal_memory_resource () noexcept { _release() ; }
private:
        context          &  ctx_ ;
        allocator_config config_ ;
        MTL::Heap        * heap_ ;
        string             name_ ;     // replace with small_string to force small names and avoid allocations

        constexpr        buffer _alloc_buffer        (                ssize_t  _size_ ) ;
        constexpr vertex_buffer _alloc_vertex_buffer (                  u32_t  _size_ ) ;
        constexpr vertex_buffer _alloc_vertex_buffer ( float * _verts_, u32_t  _size_ ) ;
        constexpr  index_buffer _alloc_index_buffer  ( u32_t *  _idxs_, u32_t _count_ ) ;

        constexpr        buffer _alloc_buffer_at_offset        (                ssize_t  _size_, u64_t _offset_ ) ;
        constexpr vertex_buffer _alloc_vertex_buffer_at_offset (                  u32_t  _size_, u64_t _offset_ ) ;
        constexpr vertex_buffer _alloc_vertex_buffer_at_offset ( float * _verts_, u32_t  _size_, u64_t _offset_ ) ;
        constexpr  index_buffer _alloc_index_buffer_at_offset  ( u32_t *  _idxs_, u32_t _count_, u64_t _offset_ ) ;

//      constexpr texture _alloc_texture        () ;
//      constexpr texture _alloc_sparse_texture () ;

//      constexpr texture _texture_from_buffer ( buffer & _buffer_ ) ;

        constexpr ssize_type           _size  (                    ) const noexcept { return heap_->                size(                         ) ; }
        constexpr ssize_type _allocated_size  (                    ) const noexcept { return heap_->currentAllocatedSize(                         ) ; }
        constexpr ssize_type _bytes_in_use    (                    ) const noexcept { return heap_->            usedSize(                         ) ; }
        constexpr ssize_type _bytes_available ( ssize_type _align_ ) const noexcept { return heap_->    maxAvailableSize( NS::UInteger( _align_ ) ) ; }

        constexpr resource_options _resource_opts () const noexcept
        {  return resource_options{ config_.storage_mode_, config_.cpu_cache_mode_, config_.hazard_tracking_mode_ } ; }

        constexpr void _log_memory_usage () const noexcept ;

        constexpr void _init ( allocator_config const & _config_, ssize_type _init_size_ ) ;

        constexpr void _release () noexcept
        { if( heap_ ) { HAZE_CORE_INFO( "metal_memory_resource::release : releasing '" SV_FMT "' memory resource", SV_ARG( name_ ) ) ; heap_->release() ; heap_ = nullptr ; } }
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr metal_memory_resource::metal_memory_resource ( context & _ctx_ )
        : ctx_( _ctx_ )
{}

////////////////////////////////////////////////////////////////////////////////

constexpr metal_memory_resource::metal_memory_resource ( context & _ctx_, allocator_config const & _config_, ssize_type _init_size_ )
        : ctx_   (    _ctx_ )
        , config_( _config_ )
{
        _init( _config_, _init_size_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr buffer metal_memory_resource::_alloc_buffer ( ssize_type _size_ )
{
        HAZE_CORE_INFO( "metal_allocator::alloc_buffer : allocating buffer of %ld bytes...", _size_ ) ;
        MTL::Buffer * buff = heap_->newBuffer( _size_, heap_->resourceOptions() ) ;

        if( !buff ) HAZE_CORE_FATAL( "metal_allocator::alloc_buffer : failed creating buffer!" ) ;

        if( buff->length() < NS::UInteger( _size_ ) )
        {
                HAZE_CORE_WARN( "metal_allocator::alloc_buffer : buffer creation successful,"
                                "but allocated size is smaller than expected { %ldb vs expected %ldb }", buff->length(), _size_ ) ;
                return buffer( buff, *this, ctx_ ) ;
        }
        HAZE_CORE_INFO( "metal_allocator::alloc_buffer : successfully allocated %ld byte buffer", _size_ ) ;
        return buffer( buff, *this, ctx_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr buffer metal_memory_resource::_alloc_buffer_at_offset ( ssize_type _size_, u64_t _offset_ )
{
        HAZE_CORE_INFO( "metal_allocator::alloc_buffer : allocating buffer of %ld bytes at offset %ld...", _size_, _offset_ ) ;
        MTL::Buffer * buff = heap_->newBuffer( _size_, heap_->resourceOptions(), _offset_ ) ;

        if( !buff ) HAZE_CORE_FATAL( "metal_allocator::alloc_buffer : failed creating buffer!" ) ;

        if( buff->length() < NS::UInteger( _size_ ) )
        {
                HAZE_CORE_WARN( "metal_allocator::alloc_buffer : buffer creation successful,"
                                "but allocated size is smaller than expected { %ldb vs expected %ldb }", buff->length(), _size_ ) ;
                return buffer( buff, *this, ctx_ ) ;
        }
        HAZE_CORE_INFO( "metal_allocator::alloc_buffer : successfully allocated %ld byte buffer", _size_ ) ;
        return buffer( buff, *this, ctx_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void metal_memory_resource::_log_memory_usage () const noexcept
{
        auto total_size = size() ;

        if( total_size < 1024 )
        {
                HAZE_CORE_INFO( "metal_allocator :            size : { %ldb }",           size() ) ;
                HAZE_CORE_INFO( "metal_allocator :  allocated size : { %ldb }", allocated_size() ) ;
        }
        else if( total_size < 1024 * 1024 )
        {
                HAZE_CORE_INFO( "metal_allocator :            size : { %.2fkb }", ( float )           size() / 1024 ) ;
                HAZE_CORE_INFO( "metal_allocator :  allocated size : { %.2fkb }", ( float ) allocated_size() / 1024 ) ;
        }
        else if( total_size < 1024 * 1024 * 1024 )
        {
                HAZE_CORE_INFO( "metal_allocator :            size : { %.2fMb }", ( float )           size() / 1024 / 1024 ) ;
                HAZE_CORE_INFO( "metal_allocator :  allocated size : { %.2fMb }", ( float ) allocated_size() / 1024 / 1024 ) ;
        }
        else
        {
                HAZE_CORE_INFO( "metal_allocator :            size : { %.2fGb }", ( float )           size() / 1024 / 1024 / 1024 ) ;
                HAZE_CORE_INFO( "metal_allocator :  allocated size : { %.2fGb }", ( float ) allocated_size() / 1024 / 1024 / 1024 ) ;
        }
        HAZE_CORE_INFO( "metal_allocator : bytes in use    : { %ldb }", bytes_in_use   (   ) ) ;
        HAZE_CORE_INFO( "metal_allocator : bytes available : { %ldb }", bytes_available( 1 ) ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void metal_memory_resource::_init ( allocator_config const & _config_, ssize_type _init_size_ )
{
        if( _init_size_ == 0 )
        {
                HAZE_CORE_WARN( "metal_allocator::init : requested zero size heap, skipping initialization" ) ;
                return ;
        }
        HAZE_CORE_INFO_S( "metal_allocator::init : initializing metal_allocator..." ) ;
        {
        HAZE_CORE_INFO_S( "metal_allocator::init : '" SV_FMT "' config :"                   , SV_ARG(                              _config_.                name_   ) ) ;
        HAZE_CORE_INFO  ( "metal_allocator::init :    placement strategy   : { " SV_FMT " }", SV_ARG(   placement_strategy_string( _config_.  placement_strategy_ ) ) ) ;
        HAZE_CORE_INFO  ( "metal_allocator::init :    cpu cache mode       : { " SV_FMT " }", SV_ARG(       cpu_cache_mode_string( _config_.      cpu_cache_mode_ ) ) ) ;
        HAZE_CORE_INFO  ( "metal_allocator::init :    storage mode         : { " SV_FMT " }", SV_ARG(         storage_mode_string( _config_.        storage_mode_ ) ) ) ;
        HAZE_CORE_INFO  ( "metal_allocator::init :    hazard tracking mode : { " SV_FMT " }", SV_ARG( hazard_tracking_mode_string( _config_.hazard_tracking_mode_ ) ) ) ;
        HAZE_CORE_INFO  ( "metal_allocator::init :    sparse page size     : { %dkb }",                      sparse_page_size_val( _config_.    sparse_page_size_   ) ) ;
        }
        MTL::HeapDescriptor * desc = MTL::HeapDescriptor::alloc()->init() ;

        desc->setType              ( MTL::HeapType          ( uti::to_underlying( _config_.  placement_strategy_ ) ) ) ;
        desc->setCpuCacheMode      ( MTL::CPUCacheMode      ( uti::to_underlying( _config_.      cpu_cache_mode_ ) ) ) ;
        desc->setStorageMode       ( MTL::StorageMode       ( uti::to_underlying( _config_.        storage_mode_ ) ) ) ;
        desc->setHazardTrackingMode( MTL::HazardTrackingMode( uti::to_underlying( _config_.hazard_tracking_mode_ ) ) ) ;
        desc->setSparsePageSize    ( MTL::SparsePageSize    ( uti::to_underlying( _config_.    sparse_page_size_ ) ) ) ;

        desc->setSize( _init_size_ ) ;

        heap_ = ctx_.device()->newHeap( desc ) ;

        desc->release() ;

        if( !heap_ ) HAZE_CORE_FATAL( "metal_allocator::init : failed creating underlying heap!" ) ;

        auto max_size = allocated_size() ;

        if( max_size < _init_size_ )
        {
                HAZE_CORE_WARN( "metal_allocator::init : heap creation successful,"
                                "but size of heap is smaller than expected { %ldb vs expected %ldb }", max_size, _init_size_ ) ;
                log_memory_usage() ;
                return ;
        }
        HAZE_CORE_INFO( "metal_allocator::init : successfully created" ) ;
        log_memory_usage() ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
