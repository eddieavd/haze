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

class gpu_memory_resource : public gpu_memory_resource_base< gpu_memory_resource >
{
        using _self = gpu_memory_resource ;
        using _base = gpu_memory_resource_base< _self > ;
public:
        using  size_type = typename _base:: size_type ;
        using ssize_type = typename _base::ssize_type ;

        using  _base::_base ;
        friend _base        ;

        constexpr gpu_memory_resource ( context & _ctx_                                                            ) ;
        constexpr gpu_memory_resource ( context & _ctx_, allocator_config const & _config_, ssize_type _init_size_ ) ;

        constexpr ~gpu_memory_resource () noexcept { _release() ; }
private:
        context          &  ctx_ ;
        allocator_config config_ ;
        MTL::Heap        * heap_ ;

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

        constexpr string_view name () const noexcept { return config_.name_ ; }

        constexpr ssize_type           _size  (                    ) const noexcept { return heap_->                size(                         ) ; }
        constexpr ssize_type _allocated_size  (                    ) const noexcept { return heap_->currentAllocatedSize(                         ) ; }
        constexpr ssize_type _bytes_in_use    (                    ) const noexcept { return heap_->            usedSize(                         ) ; }
        constexpr ssize_type _bytes_available ( ssize_type _align_ ) const noexcept { return heap_->    maxAvailableSize( NS::UInteger( _align_ ) ) ; }

        constexpr resource_options _resource_opts () const noexcept
        {  return resource_options{ config_.storage_mode_, config_.cpu_cache_mode_, config_.hazard_tracking_mode_ } ; }

        constexpr void _log_memory_usage () const noexcept ;

        constexpr void _init ( allocator_config const & _config_, ssize_type _init_size_ ) ;

        constexpr void _release () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr gpu_memory_resource::gpu_memory_resource ( context & _ctx_ )
        : ctx_( _ctx_ )
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' ) : ctor_body", SV_ARG( name() ) ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr gpu_memory_resource::gpu_memory_resource ( context & _ctx_, allocator_config const & _config_, ssize_type _init_size_ )
        : ctx_   (    _ctx_ )
        , config_( _config_ )
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' ) : ctor_body", SV_ARG( name() ) ) ;
        _init( _config_, _init_size_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr buffer gpu_memory_resource::_alloc_buffer ( ssize_type _size_ )
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer", SV_ARG( name() ) ) ;

        HAZE_CORE_DBG( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer : allocating buffer of %ld bytes...", SV_ARG( name() ), _size_ ) ;
        MTL::Buffer * buff = heap_->newBuffer( _size_, heap_->resourceOptions() ) ;

        if( !buff ) HAZE_CORE_FATAL( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer : failed allocating %ld byte buffer!", SV_ARG( name() ), _size_ ) ;

        if( buff->length() < NS::UInteger( _size_ ) )
        {
                HAZE_CORE_WARN( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer : buffer creation successful,"
                                "but allocated size is smaller than expected { %ldb vs expected %ldb }", SV_ARG( name() ), buff->length(), _size_ ) ;
                return buffer( buff, *this, ctx_ ) ;
        }
        HAZE_CORE_DBG( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer : successfully allocated %ld byte buffer", SV_ARG( name() ), _size_ ) ;
        return buffer( buff, *this, ctx_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr buffer gpu_memory_resource::_alloc_buffer_at_offset ( ssize_type _size_, u64_t _offset_ )
{
        HAZE_CORE_INFO_S( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset", SV_ARG( name() ) ) ;

        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : allocating buffer of %ld bytes at offset %ld...", SV_ARG( name() ), _size_, _offset_ ) ;
        MTL::Buffer * buff = heap_->newBuffer( _size_, heap_->resourceOptions(), _offset_ ) ;

        if( !buff ) HAZE_CORE_FATAL( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : failed creating buffer!", SV_ARG( name() ) ) ;

        if( buff->length() < NS::UInteger( _size_ ) )
        {
                HAZE_CORE_WARN( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : buffer creation successful,"
                                "but allocated size is smaller than expected { %ldb vs expected %ldb }", SV_ARG( name() ), buff->length(), _size_ ) ;
                return buffer( buff, *this, ctx_ ) ;
        }
        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : successfully allocated %ld byte buffer at offset %ld",
                         SV_ARG( name() ), _size_, _offset_ ) ;
        return buffer( buff, *this, ctx_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void gpu_memory_resource::_log_memory_usage () const noexcept
{
        auto total_size = size() ;

        if( total_size < 1024 )
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :            size : { %ldb }", SV_ARG( name() ),           size() ) ;
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :  allocated size : { %ldb }", SV_ARG( name() ), allocated_size() ) ;
        }
        else if( total_size < 1024 * 1024 )
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :            size : { %.2fkb }", SV_ARG( name() ), ( float )           size() / 1024 ) ;
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :  allocated size : { %.2fkb }", SV_ARG( name() ), ( float ) allocated_size() / 1024 ) ;
        }
        else if( total_size < 1024 * 1024 * 1024 )
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :            size : { %.2fMb }", SV_ARG( name() ), ( float )           size() / 1024 / 1024 ) ;
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :  allocated size : { %.2fMb }", SV_ARG( name() ), ( float ) allocated_size() / 1024 / 1024 ) ;
        }
        else
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :            size : { %.2fGb }", SV_ARG( name() ), ( float )           size() / 1024 / 1024 / 1024 ) ;
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) :  allocated size : { %.2fGb }", SV_ARG( name() ), ( float ) allocated_size() / 1024 / 1024 / 1024 ) ;
        }
        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : bytes in use    : { %ldb }", SV_ARG( name() ), bytes_in_use   (   ) ) ;
        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : bytes available : { %ldb }", SV_ARG( name() ), bytes_available( 1 ) ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void gpu_memory_resource::_init ( allocator_config const & _config_, ssize_type _init_size_ )
{
        config_ = _config_ ;

        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' )::init", SV_ARG( name() ) ) ;

        if( _init_size_ == 0 )
        {
                HAZE_CORE_WARN( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : requested size is zero, skipping initialization", SV_ARG( name() ) ) ;
                return ;
        }

        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init :    placement strategy   : { " SV_FMT " }", SV_ARG( name() ), SV_ARG(   placement_strategy_string( config_.  placement_strategy_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init :    cpu cache mode       : { " SV_FMT " }", SV_ARG( name() ), SV_ARG(       cpu_cache_mode_string( config_.      cpu_cache_mode_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init :    storage mode         : { " SV_FMT " }", SV_ARG( name() ), SV_ARG(         storage_mode_string( config_.        storage_mode_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init :    hazard tracking mode : { " SV_FMT " }", SV_ARG( name() ), SV_ARG( hazard_tracking_mode_string( config_.hazard_tracking_mode_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init :    sparse page size     : { %dkb }"      , SV_ARG( name() ),                sparse_page_size_val( config_.    sparse_page_size_   ) ) ;

        MTL::HeapDescriptor * desc = MTL::HeapDescriptor::alloc()->init() ;

        desc->setType              ( MTL::HeapType          ( uti::to_underlying( config_.  placement_strategy_ ) ) ) ;
        desc->setCpuCacheMode      ( MTL::CPUCacheMode      ( uti::to_underlying( config_.      cpu_cache_mode_ ) ) ) ;
        desc->setStorageMode       ( MTL::StorageMode       ( uti::to_underlying( config_.        storage_mode_ ) ) ) ;
        desc->setHazardTrackingMode( MTL::HazardTrackingMode( uti::to_underlying( config_.hazard_tracking_mode_ ) ) ) ;
        desc->setSparsePageSize    ( MTL::SparsePageSize    ( uti::to_underlying( config_.    sparse_page_size_ ) ) ) ;

        desc->setSize( _init_size_ ) ;

        heap_ = ctx_.device()->newHeap( desc ) ;

        desc->release() ;

        if( !heap_ ) HAZE_CORE_FATAL( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : failed allocating heap!", SV_ARG( name() ) ) ;

        auto max_size = allocated_size() ;

        if( max_size < _init_size_ )
        {
                HAZE_CORE_WARN( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : heap creation successful,"
                                "but size of heap is smaller than expected { %ldb vs expected %ldb }", SV_ARG( name() ), max_size, _init_size_ ) ;
                log_memory_usage() ;
                return ;
        }
        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : successfully created", SV_ARG( name() ) ) ;
        log_memory_usage() ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void gpu_memory_resource::_release () noexcept
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource(' " SV_FMT " ')::release", SV_ARG( name() ) ) ;
        {
        HAZE_CORE_INFO_S( "mtl::gpu_memory_resource( '" SV_FMT "' ) : usage on release", SV_ARG( name() ) ) ;
        _log_memory_usage() ;
        }
        if( heap_ )
        {
                heap_->release() ; 
                heap_ = nullptr ;
                HAZE_CORE_TRACE( "mtl::gpu_memory_resource(' " SV_FMT " ')::release : heap released", SV_ARG( name() ) ) ;
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
