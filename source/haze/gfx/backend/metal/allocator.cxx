//
//
//      haze
//      gfx/backend/metal/allocator.cxx
//

#include <haze/gfx/backend/metal/allocator.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

constexpr gpu_memory_resource::gpu_memory_resource ( context & _ctx_, allocator_config const & _config_ )
        : ctx_   (    _ctx_ )
        , config_( _config_ )
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' ) : ctor", SV_ARG( name() ) ) ;
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
                return buffer( buff, this ) ;
        }
        HAZE_CORE_DBG( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer : successfully allocated %ld byte buffer", SV_ARG( name() ), _size_ ) ;
        return buffer( buff, this ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr buffer gpu_memory_resource::_alloc_buffer_at_offset ( ssize_type _size_, u64_t _offset_ )
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset", SV_ARG( name() ) ) ;

        HAZE_CORE_DBG( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : allocating buffer of %ld bytes at offset %ld...", SV_ARG( name() ), _size_, _offset_ ) ;
        MTL::Buffer * buff = heap_->newBuffer( _size_, heap_->resourceOptions(), _offset_ ) ;

        if( !buff ) HAZE_CORE_FATAL( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : failed creating buffer!", SV_ARG( name() ) ) ;

        if( buff->length() < NS::UInteger( _size_ ) )
        {
                HAZE_CORE_WARN( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : buffer creation successful,"
                                "but allocated size is smaller than expected { %ldb vs expected %ldb }", SV_ARG( name() ), buff->length(), _size_ ) ;
                return buffer( buff, this ) ;
        }
        HAZE_CORE_DBG( "mtl::gpu_memory_resource( '" SV_FMT "' )::alloc_buffer_at_offset : successfully allocated %ld byte buffer at offset %ld",
                         SV_ARG( name() ), _size_, _offset_ ) ;
        return buffer( buff, this ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void gpu_memory_resource::_log_memory_usage () const noexcept
{
        auto total_size = capacity() ;

        if( total_size < 1024 )
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : capacity        : { %ldb }", SV_ARG( name() ), capacity() ) ;
        }
        else if( total_size < 1024 * 1024 )
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : capacity        : { %.2fkb }", SV_ARG( name() ), ( float ) capacity() / 1024 ) ;
        }
        else if( total_size < 1024 * 1024 * 1024 )
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : capacity        : { %.2fMb }", SV_ARG( name() ), ( float ) capacity() / 1024 / 1024 ) ;
        }
        else
        {
                HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : capacity        : { %.2fGb }", SV_ARG( name() ), ( float ) capacity() / 1024 / 1024 / 1024 ) ;
        }
        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : bytes in use    : { %ldb }", SV_ARG( name() ), bytes_in_use   (   ) ) ;
        HAZE_CORE_INFO( "mtl::gpu_memory_resource( '" SV_FMT "' ) : bytes available : { %ldb }", SV_ARG( name() ), bytes_available( 1 ) ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void gpu_memory_resource::_init ( ssize_type _init_size_ )
{
        HAZE_CORE_TRACE_S( "mtl::gpu_memory_resource( '" SV_FMT "' )::init", SV_ARG( name() ) ) ;

        if( _init_size_ == 0 )
        {
                HAZE_CORE_WARN( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : requested size is zero, skipping initialization", SV_ARG( name() ) ) ;
                return ;
        }
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : using placement strategy   : { " SV_FMT " }", SV_ARG( name() ), SV_ARG(   placement_strategy_string( config_.  placement_strategy_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : using cpu cache mode       : { " SV_FMT " }", SV_ARG( name() ), SV_ARG(       cpu_cache_mode_string( config_.      cpu_cache_mode_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : using storage mode         : { " SV_FMT " }", SV_ARG( name() ), SV_ARG(         storage_mode_string( config_.        storage_mode_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : using hazard tracking mode : { " SV_FMT " }", SV_ARG( name() ), SV_ARG( hazard_tracking_mode_string( config_.hazard_tracking_mode_ ) ) ) ;
        HAZE_CORE_TRACE  ( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : using sparse page size     : { %dkb }"      , SV_ARG( name() ),                sparse_page_size_val( config_.    sparse_page_size_   ) ) ;

        MTL::HeapDescriptor * desc = MTL::HeapDescriptor::alloc()->init() ;

        desc->setType              ( MTL::HeapType          ( uti::to_underlying( config_.  placement_strategy_ ) ) ) ;
        desc->setCpuCacheMode      ( MTL::CPUCacheMode      ( uti::to_underlying( config_.      cpu_cache_mode_ ) ) ) ;
        desc->setStorageMode       ( MTL::StorageMode       ( uti::to_underlying( config_.        storage_mode_ ) ) ) ;
        desc->setHazardTrackingMode( MTL::HazardTrackingMode( uti::to_underlying( config_.hazard_tracking_mode_ ) ) ) ;
        desc->setSparsePageSize    ( MTL::SparsePageSize    ( uti::to_underlying( config_.    sparse_page_size_ ) ) ) ;

        desc->setSize( _init_size_ ) ;

        heap_ = static_cast< MTL::Device * >( ctx_ )->newHeap( desc ) ;

        desc->release() ;

        if( !heap_ ) HAZE_CORE_FATAL( "mtl::gpu_memory_resource( '" SV_FMT "' )::init : failed allocating heap!", SV_ARG( name() ) ) ;

        auto max_size = capacity() ;

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

constexpr void gpu_memory_resource::_destroy () noexcept
{
        HAZE_CORE_INFO_S( "mtl::gpu_memory_resource(' " SV_FMT " ')::destroy", SV_ARG( name() ) ) ;
        {
        HAZE_CORE_INFO_S( "mtl::gpu_memory_resource( '" SV_FMT "' ) : usage on release", SV_ARG( name() ) ) ;
        _log_memory_usage() ;
        }
        if( heap_ )
        {
                heap_->release() ; 
                heap_ = nullptr ;
                HAZE_CORE_TRACE( "mtl::gpu_memory_resource(' " SV_FMT " ')::destroy : heap released", SV_ARG( name() ) ) ;
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
