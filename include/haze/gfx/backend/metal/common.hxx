//
//
//      haze
//      gfx/backend/metal/common.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>

#define  NS_PRIVATE_IMPLEMENTATION
#define  CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION

UTI_DIAGS_PUSH()
UTI_DIAGS_DISABLE( -Wunused-parameter )
UTI_DIAGS_DISABLE( -Wgnu-anonymous-struct )

#include <metalcpp/Metal/Metal.hpp>
#include <metalcppext/AppKit/AppKit.hpp>
#include <metalcppext/MetalKit/MetalKit.hpp>

UTI_DIAGS_POP()

#include <simd/simd.h>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

template< typename T >
class ns_object
{
public:
        using value_type = T ;

        using       pointer = value_type       * ;
        using const_pointer = value_type const * ;

        static constexpr ns_object   retain ( pointer _ptr_ ) noexcept { ns_object obj( _ptr_ ) ; obj.ptr_->retain() ; return obj ; }
        static constexpr ns_object transfer ( pointer _ptr_ ) noexcept { ns_object obj( _ptr_ ) ;                      return obj ; }

        constexpr ns_object () noexcept = default ;

        constexpr ns_object ( pointer _ptr_ ) noexcept : ptr_{ _ptr_ } {}

        constexpr ns_object             ( ns_object const &  _other_ ) noexcept :             ptr_ { _other_.ptr_ } {         ptr_->retain() ;                }
        constexpr ns_object             ( ns_object       && _other_ ) noexcept :             ptr_ { _other_.ptr_ } { _other_.ptr_ = nullptr ;                }
        constexpr ns_object & operator= ( ns_object const &  _other_ ) noexcept { release() ; ptr_ = _other_.ptr_ ;           ptr_->retain() ; return *this ; }
        constexpr ns_object & operator= ( ns_object       && _other_ ) noexcept { release() ; ptr_ = _other_.ptr_ ;   _other_.ptr_ = nullptr ; return *this ; }

        constexpr ~ns_object () noexcept { release() ; }

        constexpr void release () noexcept { if( ptr_ ) { ptr_->release() ; ptr_ = nullptr ; } }

        constexpr operator       pointer ()       noexcept { return ptr_ ; }
        constexpr operator const_pointer () const noexcept { return ptr_ ; }
private:
        pointer ptr_ {} ;
} ;


namespace ns
{

template< typename T > constexpr ns_object< T >   retain ( typename ns_object< T >::pointer _ptr_ ) noexcept { return ns_object< T >::  retain( _ptr_ ) ; }
template< typename T > constexpr ns_object< T > transfer ( typename ns_object< T >::pointer _ptr_ ) noexcept { return ns_object< T >::transfer( _ptr_ ) ; }

} // namespace ns

////////////////////////////////////////////////////////////////////////////////

enum class placement_strategy
{
////////////////////////////////////////////////////////////////////////////////
        /// automatic
        /// - let haze determine the location of resources allocated from the heap
        /// - use for temporary resources which are written to often
        automatic = MTL::HeapTypeAutomatic ,
////////////////////////////////////////////////////////////////////////////////
        /// placement
        /// - for direct control over memory use and fragmentation
        /// - use for long living, rarely changed resources
        placement = MTL::HeapTypePlacement ,

////////////////////////////////////////////////////////////////////////////////
        /// sparse
        /// - only used for creating sparse textures
        sparse = MTL::HeapTypeSparse ,
////////////////////////////////////////////////////////////////////////////////
        count ,
} ;

constexpr string_view placement_strategy_string ( placement_strategy _strategy_ ) noexcept
{
        switch( _strategy_ )
        {
                case placement_strategy::automatic : return "automatic" ;
                case placement_strategy::placement : return "placement" ;
                case placement_strategy::   sparse : return    "sparse" ;
                default                            : return   "unknown" ;
        }
}

////////////////////////////////////////////////////////////////////////////////

enum class cpu_cache_mode
{
////////////////////////////////////////////////////////////////////////////////
        /// dflt
        /// - default mode
        /// - guarantees reads and writes are executed in the expected order
        dflt = MTL::CPUCacheModeDefaultCache ,
////////////////////////////////////////////////////////////////////////////////
        /// write
        /// Write-combined memory is optimized for resources that the CPU writes into, but never reads.
        /// On some implementations, writes may bypass caches to avoid cache pollution. Read actions may perform very poorly.
        ///
        /// Applications should use write combining only if writing to normally cached buffers
        /// is known to cause performance issues due to cache pollution,
        /// as write-combined memory can have surprising performance pitfalls.
        /// Another approach is to use non-temporal writes to normally cached memory (STNP on ARMv8, _mm_stream_* on x86_64).
        ///     https://developer.apple.com/documentation/metal/mtlcpucachemode/writecombined
        write = MTL::CPUCacheModeWriteCombined ,
////////////////////////////////////////////////////////////////////////////////
        count ,
} ;

constexpr string_view cpu_cache_mode_string ( cpu_cache_mode _cache_mode_ ) noexcept
{
        switch( _cache_mode_ )
        {
                case cpu_cache_mode:: dflt : return "default" ;
                case cpu_cache_mode::write : return   "write" ;
                default                    : return "unknown" ;
        }
}

////////////////////////////////////////////////////////////////////////////////

enum class storage_mode
{
        shared      = MTL::StorageModeShared     ,
        managed     = MTL::StorageModeManaged    ,
        gpu_private = MTL::StorageModePrivate    ,
        none        = MTL::StorageModeMemoryless ,
} ;

constexpr string_view storage_mode_string ( storage_mode _storage_mode_ ) noexcept
{
        switch( _storage_mode_ )
        {
                case storage_mode::       none : return  "memoryless" ;
                case storage_mode::     shared : return      "shared" ;
                case storage_mode::    managed : return     "managed" ;
                case storage_mode::gpu_private : return "gpu_private" ;
                default                        : return     "unknown" ;
        }
}

////////////////////////////////////////////////////////////////////////////////

enum class hazard_tracking_mode
{
        default_tracking = MTL::HazardTrackingModeDefault ,
////////////////////////////////////////////////////////////////////////////////
        /// untracked
        /// Metal does not do any dependency analysis on untracked resources.
        /// You are responsible for ensuring that resources are modified safely.
        untracked = MTL::HazardTrackingModeUntracked ,
////////////////////////////////////////////////////////////////////////////////
        /// tracked
        /// For a resource, Metal tracks dependencies on any accesses to the resource.
        /// If you submit a command that modifies the resource,
        /// Metal delays that command from executing until prior commands accessing that resource are complete,
        /// and prevents future commands from executing until the modifications are complete.
        ///
        /// For a heap, Metal tracks dependencies on accesses to any resources on the heap.
        /// If you submit a command that modifies a resource on a heap,
        /// Metal delays that command from executing until prior commands accessing the heap’s resources are complete,
        /// and prevents future commands accessing the heap’s resources from executing until the modifications are complete.
        ///
        /// For better performance, use untracked resources and synchronize access yourself.
        tracked = MTL::HazardTrackingModeTracked ,
////////////////////////////////////////////////////////////////////////////////
        count ,
} ;

constexpr string_view hazard_tracking_mode_string ( hazard_tracking_mode _tracking_mode_ ) noexcept
{
        switch( _tracking_mode_ )
        {
                case hazard_tracking_mode::untracked : return "untracked" ;
                case hazard_tracking_mode::  tracked : return   "tracked" ;
                default                              : return   "unknown" ;
        }
}

////////////////////////////////////////////////////////////////////////////////

enum class sparse_page_size
{
        size_16  = MTL::SparsePageSize16  ,
        size_64  = MTL::SparsePageSize64  ,
        size_256 = MTL::SparsePageSize256 ,
        count    ,
} ;

constexpr i32_t sparse_page_size_val ( sparse_page_size _cache_mode_ ) noexcept
{
        switch( _cache_mode_ )
        {
                case sparse_page_size::size_16  : return  16 ;
                case sparse_page_size::size_64  : return  64 ;
                case sparse_page_size::size_256 : return 256 ;
                default                         : return  16 ;
        }
}

////////////////////////////////////////////////////////////////////////////////

struct resource_options
{
        storage_mode                 storage_mode_ ;
        cpu_cache_mode             cpu_cache_mode_ ;
        hazard_tracking_mode hazard_tracking_mode_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
