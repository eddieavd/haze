//
//
//      haze
//      gfx/backend/metal/common.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>

// #define  NS_PRIVATE_IMPLEMENTATION
// #define  CA_PRIVATE_IMPLEMENTATION
// #define MTL_PRIVATE_IMPLEMENTATION
// #define MTK_PRIVATE_IMPLEMENTATION

UTI_DIAGS_PUSH()
UTI_DIAGS_DISABLE( -Wunused-parameter )
UTI_DIAGS_DISABLE( -Wgnu-anonymous-struct )

#include <metalcpp/Foundation/Foundation.hpp>
#include <metalcpp/Metal/Metal.hpp>
#include <metalcpp/QuartzCore/CAMetalLayer.hpp>
#include <metalcpp/QuartzCore/QuartzCore.hpp>
#include <metalcppext/AppKit/AppKit.hpp>
#include <metalcppext/MetalKit/MetalKit.hpp>

UTI_DIAGS_POP()

#include <simd/simd.h>

#define HAZE_MTL_MAX_VERTEX_ATTRIB_PER_VERTEX_DESC 31


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

        constexpr ns_object             ( ns_object const &  _other_ ) noexcept :              ptr_ { _other_.ptr_ } {         ptr_->retain() ;                }
        constexpr ns_object             ( ns_object       && _other_ ) noexcept :              ptr_ { _other_.ptr_ } { _other_.ptr_ = nullptr ;                }
        constexpr ns_object & operator= ( ns_object const &  _other_ ) noexcept { _release() ; ptr_ = _other_.ptr_ ;           ptr_->retain() ; return *this ; }
        constexpr ns_object & operator= ( ns_object       && _other_ ) noexcept { _release() ; ptr_ = _other_.ptr_ ;   _other_.ptr_ = nullptr ; return *this ; }

        constexpr ~ns_object () noexcept { HAZE_CORE_INFO_S( "ns_object::dtor" ) ; _release() ; }

        constexpr bool exists () const noexcept { return ptr_ != nullptr ; }

        constexpr operator       pointer ()       noexcept { return ptr_ ; }
        constexpr operator const_pointer () const noexcept { return ptr_ ; }
protected:
        pointer ptr_ {} ;

        constexpr void _release () noexcept { if( ptr_ ) { HAZE_CORE_INFO( "ns_object : releasing ns object" ) ; ptr_->release() ; ptr_ = nullptr ; } }
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
////////////////////////////////////////////////////////////////////////////////

enum class shader_data_type
{
        NONE = 0 ,

        CHAR     = MTL::VertexFormatChar  ,
        CHAR2    = MTL::VertexFormatChar2 ,
        CHAR3    = MTL::VertexFormatChar3 ,
        CHAR4    = MTL::VertexFormatChar4 ,

        UCHAR    = MTL::VertexFormatUChar  ,
        UCHAR2   = MTL::VertexFormatUChar2 ,
        UCHAR3   = MTL::VertexFormatUChar3 ,
        UCHAR4   = MTL::VertexFormatUChar4 ,

        NORMCHAR     = MTL::VertexFormatCharNormalized  ,
        NORMCHAR2    = MTL::VertexFormatChar2Normalized ,
        NORMCHAR3    = MTL::VertexFormatChar3Normalized ,
        NORMCHAR4    = MTL::VertexFormatChar4Normalized ,

        NORMUCHAR    = MTL::VertexFormatUCharNormalized  ,
        NORMUCHAR2   = MTL::VertexFormatUChar2Normalized ,
        NORMUCHAR3   = MTL::VertexFormatUChar3Normalized ,
        NORMUCHAR4   = MTL::VertexFormatUChar4Normalized ,

        SHORT    = MTL::VertexFormatShort  ,
        SHORT2   = MTL::VertexFormatShort2 ,
        SHORT3   = MTL::VertexFormatShort3 ,
        SHORT4   = MTL::VertexFormatShort4 ,

        USHORT   = MTL::VertexFormatUShort  ,
        USHORT2  = MTL::VertexFormatUShort2 ,
        USHORT3  = MTL::VertexFormatUShort3 ,
        USHORT4  = MTL::VertexFormatUShort4 ,

        NORMSHORT    = MTL::VertexFormatShortNormalized  ,
        NORMSHORT2   = MTL::VertexFormatShort2Normalized ,
        NORMSHORT3   = MTL::VertexFormatShort3Normalized ,
        NORMSHORT4   = MTL::VertexFormatShort4Normalized ,

        NORMUSHORT   = MTL::VertexFormatUShortNormalized  ,
        NORMUSHORT2  = MTL::VertexFormatUShort2Normalized ,
        NORMUSHORT3  = MTL::VertexFormatUShort3Normalized ,
        NORMUSHORT4  = MTL::VertexFormatUShort4Normalized ,

        INT      = MTL::VertexFormatInt  ,
        INT2     = MTL::VertexFormatInt2 ,
        INT3     = MTL::VertexFormatInt3 ,
        INT4     = MTL::VertexFormatInt4 ,

        UINT     = MTL::VertexFormatUInt  ,
        UINT2    = MTL::VertexFormatUInt2 ,
        UINT3    = MTL::VertexFormatUInt3 ,
        UINT4    = MTL::VertexFormatUInt4 ,

        HALF     = MTL::VertexFormatHalf  ,
        HALF2    = MTL::VertexFormatHalf2 ,
        HALF3    = MTL::VertexFormatHalf3 ,
        HALF4    = MTL::VertexFormatHalf4 ,

        FLOAT    = MTL::VertexFormatFloat  ,
        FLOAT2   = MTL::VertexFormatFloat2 ,
        FLOAT3   = MTL::VertexFormatFloat3 ,
        FLOAT4   = MTL::VertexFormatFloat4 ,

//      FLOAT3X3 ,
//      FLOAT4X4 ,
} ;

constexpr u32_t shader_data_type_size ( shader_data_type _type_ ) noexcept
{
        switch( _type_ )
        {
                case shader_data_type::     CHAR : [[ fallthrough ]] ;
                case shader_data_type::    UCHAR : [[ fallthrough ]] ;
                case shader_data_type:: NORMCHAR : [[ fallthrough ]] ;
                case shader_data_type::NORMUCHAR :       return 1 * 1 ;

                case shader_data_type::     CHAR2 : [[ fallthrough ]] ;
                case shader_data_type::    UCHAR2 : [[ fallthrough ]] ;
                case shader_data_type:: NORMCHAR2 : [[ fallthrough ]] ;
                case shader_data_type::NORMUCHAR2 :      return 1 * 2 ;

                case shader_data_type::     CHAR3 : [[ fallthrough ]] ;
                case shader_data_type::    UCHAR3 : [[ fallthrough ]] ;
                case shader_data_type:: NORMCHAR3 : [[ fallthrough ]] ;
                case shader_data_type::NORMUCHAR3 :      return 1 * 3 ;

                case shader_data_type::     CHAR4 : [[ fallthrough ]] ;
                case shader_data_type::    UCHAR4 : [[ fallthrough ]] ;
                case shader_data_type:: NORMCHAR4 : [[ fallthrough ]] ;
                case shader_data_type::NORMUCHAR4 :      return 1 * 4 ;

                case shader_data_type::     SHORT : [[ fallthrough ]] ;
                case shader_data_type::    USHORT : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT :      return 2 * 1 ;

                case shader_data_type::     SHORT2 : [[ fallthrough ]] ;
                case shader_data_type::    USHORT2 : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT2 : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT2 :      return 2 * 2 ;

                case shader_data_type::     SHORT3 : [[ fallthrough ]] ;
                case shader_data_type::    USHORT3 : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT3 : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT3 :      return 2 * 3 ;

                case shader_data_type::     SHORT4 : [[ fallthrough ]] ;
                case shader_data_type::    USHORT4 : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT4 : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT4 :      return 2 * 4 ;

                case shader_data_type:: INT  : [[ fallthrough ]] ;
                case shader_data_type::UINT  :      return 4 * 1 ;
                case shader_data_type:: INT2 : [[ fallthrough ]] ;
                case shader_data_type::UINT2 :      return 4 * 2 ;
                case shader_data_type:: INT3 : [[ fallthrough ]] ;
                case shader_data_type::UINT3 :      return 4 * 3 ;
                case shader_data_type:: INT4 : [[ fallthrough ]] ;
                case shader_data_type::UINT4 :      return 4 * 4 ;

                case shader_data_type::HALF    : return 2     ;
                case shader_data_type::HALF2   : return 2 * 2 ;
                case shader_data_type::HALF3   : return 2 * 3 ;
                case shader_data_type::HALF4   : return 2 * 4 ;

                case shader_data_type::FLOAT    : return 4     ;
                case shader_data_type::FLOAT2   : return 4 * 2 ;
                case shader_data_type::FLOAT3   : return 4 * 4 ; // I_CAN_FIX_HER: metal specific, make all this generic
                case shader_data_type::FLOAT4   : return 4 * 4 ;

//              case shader_data_type::FLOAT3X3 : return 4 * 3 * 3 ;
//              case shader_data_type::FLOAT4X4 : return 4 * 3 * 3 ;
//              case shader_data_type::BOOL     : return 1         ;
                default                         :
                {
                        HAZE_CEXPR_ASSERT( uti::always_false_v< shader_data_type >, "unknown shader data type!" ) ;
                        return 0 ;
                }
        }
}

////////////////////////////////////////////////////////////////////////////////

constexpr u32_t shader_data_type_component_count ( shader_data_type _type_ ) noexcept
{
        switch( _type_ )
        {
                case shader_data_type::      CHAR : [[ fallthrough ]] ;
                case shader_data_type::     UCHAR : [[ fallthrough ]] ;
                case shader_data_type::  NORMCHAR : [[ fallthrough ]] ;
                case shader_data_type:: NORMUCHAR : [[ fallthrough ]] ;

                case shader_data_type::     SHORT : [[ fallthrough ]] ;
                case shader_data_type::    USHORT : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT : [[ fallthrough ]] ;

                case shader_data_type::       INT : [[ fallthrough ]] ;
                case shader_data_type::      UINT : [[ fallthrough ]] ;
                case shader_data_type::      HALF : [[ fallthrough ]] ;
                case shader_data_type::     FLOAT :          return 1 ;

                case shader_data_type::      CHAR2 : [[ fallthrough ]] ;
                case shader_data_type::     UCHAR2 : [[ fallthrough ]] ;
                case shader_data_type::  NORMCHAR2 : [[ fallthrough ]] ;
                case shader_data_type:: NORMUCHAR2 : [[ fallthrough ]] ;

                case shader_data_type::     SHORT2 : [[ fallthrough ]] ;
                case shader_data_type::    USHORT2 : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT2 : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT2 : [[ fallthrough ]] ;

                case shader_data_type::       INT2 : [[ fallthrough ]] ;
                case shader_data_type::      UINT2 : [[ fallthrough ]] ;
                case shader_data_type::      HALF2 : [[ fallthrough ]] ;
                case shader_data_type::     FLOAT2 : return 2 ;

                case shader_data_type::      CHAR3 : [[ fallthrough ]] ;
                case shader_data_type::     UCHAR3 : [[ fallthrough ]] ;
                case shader_data_type::  NORMCHAR3 : [[ fallthrough ]] ;
                case shader_data_type:: NORMUCHAR3 : [[ fallthrough ]] ;

                case shader_data_type::     SHORT3 : [[ fallthrough ]] ;
                case shader_data_type::    USHORT3 : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT3 : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT3 : [[ fallthrough ]] ;

                case shader_data_type::       INT3 : [[ fallthrough ]] ;
                case shader_data_type::      UINT3 : [[ fallthrough ]] ;
                case shader_data_type::      HALF3 : [[ fallthrough ]] ;
                case shader_data_type::     FLOAT3 : return 3 ;

                case shader_data_type::      CHAR4 : [[ fallthrough ]] ;
                case shader_data_type::     UCHAR4 : [[ fallthrough ]] ;
                case shader_data_type::  NORMCHAR4 : [[ fallthrough ]] ;
                case shader_data_type:: NORMUCHAR4 : [[ fallthrough ]] ;

                case shader_data_type::     SHORT4 : [[ fallthrough ]] ;
                case shader_data_type::    USHORT4 : [[ fallthrough ]] ;
                case shader_data_type:: NORMSHORT4 : [[ fallthrough ]] ;
                case shader_data_type::NORMUSHORT4 : [[ fallthrough ]] ;

                case shader_data_type::       INT4 : [[ fallthrough ]] ;
                case shader_data_type::      UINT4 : [[ fallthrough ]] ;
                case shader_data_type::      HALF4 : [[ fallthrough ]] ;
                case shader_data_type::     FLOAT4 : return 4 ;

//              case shader_data_type::FLOAT3X3 : return 4 * 3 * 3 ;
//              case shader_data_type::FLOAT4X4 : return 4 * 3 * 3 ;
//              case shader_data_type::BOOL     : return 1         ;
                default                         :
                {
                        HAZE_CEXPR_ASSERT( uti::always_false_v< shader_data_type >, "unknown shader data type!" ) ;
                        return 0 ;
                }
        }
}

////////////////////////////////////////////////////////////////////////////////

constexpr string_view shader_data_type_string ( shader_data_type _type_ ) noexcept
{
        switch( _type_ )
        {
                case shader_data_type::      CHAR : return      "CHAR" ;
                case shader_data_type::     UCHAR : return     "UCHAR" ;
                case shader_data_type::  NORMCHAR : return  "NORMCHAR" ;
                case shader_data_type:: NORMUCHAR : return "NORMUCHAR" ;

                case shader_data_type::     SHORT : return      "SHORT" ;
                case shader_data_type::    USHORT : return     "USHORT" ;
                case shader_data_type:: NORMSHORT : return  "NORMSHORT" ;
                case shader_data_type::NORMUSHORT : return "NORMUSHORT" ;

                case shader_data_type::       INT : return   "INT" ;
                case shader_data_type::      UINT : return  "UINT" ;
                case shader_data_type::      HALF : return  "HALF" ;
                case shader_data_type::     FLOAT : return "FLOAT" ;

                case shader_data_type::      CHAR2 : return      "CHAR2" ;
                case shader_data_type::     UCHAR2 : return     "UCHAR2" ;
                case shader_data_type::  NORMCHAR2 : return  "NORMCHAR2" ;
                case shader_data_type:: NORMUCHAR2 : return "NORMUCHAR2" ;

                case shader_data_type::     SHORT2 : return      "SHORT2" ;
                case shader_data_type::    USHORT2 : return     "USHORT2" ;
                case shader_data_type:: NORMSHORT2 : return  "NORMSHORT2" ;
                case shader_data_type::NORMUSHORT2 : return "NORMUSHORT2" ;

                case shader_data_type::       INT2 : return   "INT2" ;
                case shader_data_type::      UINT2 : return  "UINT2" ;
                case shader_data_type::      HALF2 : return  "HALF2" ;
                case shader_data_type::     FLOAT2 : return "FLOAT2" ;

                case shader_data_type::      CHAR3 : return      "CHAR3" ;
                case shader_data_type::     UCHAR3 : return     "UCHAR3" ;
                case shader_data_type::  NORMCHAR3 : return  "NORMCHAR3" ;
                case shader_data_type:: NORMUCHAR3 : return "NORMUCHAR3" ;

                case shader_data_type::     SHORT3 : return      "SHORT3" ;
                case shader_data_type::    USHORT3 : return     "USHORT3" ;
                case shader_data_type:: NORMSHORT3 : return  "NORMSHORT3" ;
                case shader_data_type::NORMUSHORT3 : return "NORMUSHORT3" ;

                case shader_data_type::       INT3 : return   "INT3" ;
                case shader_data_type::      UINT3 : return  "UINT3" ;
                case shader_data_type::      HALF3 : return  "HALF3" ;
                case shader_data_type::     FLOAT3 : return "FLOAT3" ;

                case shader_data_type::      CHAR4 : return      "CHAR4" ;
                case shader_data_type::     UCHAR4 : return     "UCHAR4" ;
                case shader_data_type::  NORMCHAR4 : return  "NORMCHAR4" ;
                case shader_data_type:: NORMUCHAR4 : return "NORMUCHAR4" ;

                case shader_data_type::     SHORT4 : return      "SHORT4" ;
                case shader_data_type::    USHORT4 : return     "USHORT4" ;
                case shader_data_type:: NORMSHORT4 : return  "NORMSHORT4" ;
                case shader_data_type::NORMUSHORT4 : return "NORMUSHORT4" ;

                case shader_data_type::       INT4 : return   "INT4" ;
                case shader_data_type::      UINT4 : return  "UINT4" ;
                case shader_data_type::      HALF4 : return  "HALF4" ;
                case shader_data_type::     FLOAT4 : return "FLOAT4" ;

//              case shader_data_type::FLOAT3X3 : return 4 * 3 * 3 ;
//              case shader_data_type::FLOAT4X4 : return 4 * 3 * 3 ;
//              case shader_data_type::BOOL     : return 1         ;
                default                         :
                {
                        HAZE_CEXPR_ASSERT( uti::always_false_v< shader_data_type >, "unknown shader data type!" ) ;
                        return 0 ;
                }
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
