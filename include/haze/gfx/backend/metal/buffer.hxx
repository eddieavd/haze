//
//
//      haze
//      gfx/backend/metal/buffer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>

#include <haze/gfx/buffer_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class gpu_memory_resource ;

////////////////////////////////////////////////////////////////////////////////

class buffer : public buffer_base<    buffer >
             , public ns_object< MTL::Buffer >
{
        using      _self = buffer                   ;
        using _buff_base = buffer_base<    buffer > ;
        using   _ns_base = ns_object< MTL::Buffer > ;
public:
        using  _buff_base::_buff_base ;
        friend _buff_base             ;

        constexpr buffer () noexcept = default ;

        constexpr buffer ( MTL::Buffer * _buffer_, gpu_memory_resource * _owner_ ) : _buff_base(), _ns_base( _buffer_ ), owner_{ _owner_ } {}
private:
        gpu_memory_resource * owner_ ;

        constexpr bool _loaded () const noexcept { return _ns_base::exists() && owner_ != nullptr ; }

        constexpr void       * _data ()       noexcept { return _ns_base::ptr_->contents() ; }
        constexpr void const * _data () const noexcept { return _ns_base::ptr_->contents() ; }

        constexpr auto       * _owner ()       noexcept { return owner_ ; }
        constexpr auto const * _owner () const noexcept { return owner_ ; }

        constexpr ssize_t _capacity () const noexcept { return _ns_base::ptr_->allocatedSize() ; }

        constexpr u64_t      _gpu_address () const noexcept { return _ns_base::ptr_->gpuAddress() ; }
        constexpr u64_t _offset_in_memory () const noexcept { return _ns_base::ptr_->heapOffset() ; }

        constexpr bool   _is_aliasable () const noexcept { return _ns_base::ptr_->  isAliasable() ; }
        constexpr void _make_aliasable ()                {        _ns_base::ptr_->makeAliasable() ; }

        constexpr void _notify_update () noexcept { _ns_base::ptr_->didModifyRange( NS::Range{ 0, static_cast< NS::UInteger >( _capacity() ) } ) ; }

        constexpr void _dump () const noexcept ;

        constexpr void _destroy () noexcept { _ns_base::_release() ; owner_ = nullptr ; }
} ;

////////////////////////////////////////////////////////////////////////////////

constexpr void buffer::_dump () const noexcept
{
        uti::view<  u8_t > as_u8   ( (  u8_t * ) _data(), _capacity()     ) ;
        uti::view< u32_t > as_u32  ( ( u32_t * ) _data(), _capacity() / 4 ) ;
        uti::view< float > as_float( ( float * ) _data(), _capacity() / 4 ) ;

        HAZE_CORE_INFO_S( "buffer::dump" ) ;

        HAZE_CORE_INFO( "buffer::dump : as u8 :" ) ;
        for( auto const & val : as_u8 ) printf( "%d ", val ) ;
        printf( "\n" ) ;
        HAZE_CORE_INFO( "buffer::dump : as u32 :" ) ;
        for( auto const & val : as_u32 ) printf( "%d ", val ) ;
        printf( "\n" ) ;
        HAZE_CORE_INFO( "buffer::dump : as float :" ) ;
        for( auto const & val : as_float ) printf( "%.4f ", val ) ;
        printf( "\n" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
