//
//
//      haze
//      gfx/backend/metal/buffer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/buffer_base.hxx>

#include <haze/gfx/backend/metal/common.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class metal_memory_resource ;
class               context ;

////////////////////////////////////////////////////////////////////////////////

class buffer : public buffer_base< buffer >
{
        using _self = buffer ;
        using _base = buffer_base< _self > ;
public:
        using ssize_type = typename _base::ssize_type ;

        using  _base::_base ;
        friend _base        ;

        constexpr buffer ( MTL::Buffer * _buffer_, metal_memory_resource & _owner_, context & _ctx_ ) noexcept
                : buffer_{ _buffer_ }, owner_( &_owner_ ), ctx_( &_ctx_ ) {}

        constexpr buffer ( buffer const &  _other_ ) noexcept : buffer_( _other_.buffer_ ), owner_( _other_.owner_ ), ctx_( _other_.ctx_ ) {         buffer_->retain() ; }
        constexpr buffer ( buffer       && _other_ ) noexcept : buffer_( _other_.buffer_ ), owner_( _other_.owner_ ), ctx_( _other_.ctx_ ) { _other_.buffer_ = nullptr ; }

        constexpr buffer & operator= ( buffer const &  _other_ ) noexcept
        { _release() ; buffer_ = _other_.buffer_ ; owner_ = _other_.owner_ ; ctx_ = _other_.ctx_ ;         buffer_->retain() ; return *this ; }
        constexpr buffer & operator= ( buffer       && _other_ ) noexcept
        { _release() ; buffer_ = _other_.buffer_ ; owner_ = _other_.owner_ ; ctx_ = _other_.ctx_ ; _other_.buffer_ = nullptr ; return *this ; }

        constexpr ~buffer () noexcept { _release() ; }

        constexpr operator MTL::Buffer       * ()       noexcept { return buffer_ ; }
        constexpr operator MTL::Buffer const * () const noexcept { return buffer_ ; }

//      constexpr void test () noexcept
//      {
//              buffer_->hash() ;
//              class Texture* newTexture(const class TextureDescriptor* descriptor, NS::UInteger offset, NS::UInteger bytesPerRow);
//              void           addDebugMarker(const NS::String* marker, NS::Range range);
//              void           removeAllDebugMarkers();
//      }
private:
        MTL::Buffer           * buffer_ ;
        metal_memory_resource *  owner_ ;
        context               *    ctx_ ;

        constexpr bool _is_loaded () const noexcept { return buffer_ != nullptr ; }

        constexpr void _release () noexcept { if( buffer_ ) { buffer_->release() ; buffer_ = nullptr ; } }

        constexpr void _init ( MTL::Buffer * _buffer_, metal_memory_resource * _owner_, context * _ctx_ ) noexcept
        {
                if( buffer_ ) { HAZE_CORE_ERROR( "buffer::init : called on already initialized buffer" ) ; return ; }

                buffer_ = _buffer_ ;
                owner_  =  _owner_ ;
                ctx_    =    _ctx_ ;
        }

        constexpr void _signal_modified () noexcept { buffer_->didModifyRange( NS::Range::Make( 0, _length() ) ) ; }

        constexpr MTL::Buffer       * _impl ()       noexcept { return buffer_ ; }
        constexpr MTL::Buffer const * _impl () const noexcept { return buffer_ ; }

        constexpr metal_memory_resource       * owner ()       noexcept { return owner_ ; }
        constexpr metal_memory_resource const * owner () const noexcept { return owner_ ; }

        constexpr void       * _data ()       noexcept { return buffer_->contents() ; }
        constexpr void const * _data () const noexcept { return buffer_->contents() ; }

        constexpr ssize_type _length         () const noexcept { return buffer_->       length() ; }
        constexpr ssize_type _allocated_size () const noexcept { return buffer_->allocatedSize() ; }

        constexpr u64_t _gpu_address () const noexcept { return buffer_->gpuAddress() ; }

        constexpr bool   _is_aliasable () const noexcept { return buffer_->  isAliasable() ; }
        constexpr void _make_aliasable ()       noexcept {        buffer_->makeAliasable() ; }

        constexpr u64_t _offset_in_memory () const noexcept { return buffer_->heapOffset() ; }
} ;

////////////////////////////////////////////////////////////////////////////////

class vertex_buffer : public vertex_buffer_base< vertex_buffer >
{
        using _self = vertex_buffer               ;
        using _base = vertex_buffer_base< _self > ;
public:
        constexpr vertex_buffer (                     u32_t _size_ ) ;
        constexpr vertex_buffer ( float * _vertices_, u32_t _size_ ) ;

        constexpr ~vertex_buffer () noexcept { _release() ; }
private:
        class buffer  buffer_ ;
        buffer_layout layout_ ;

        constexpr void _upload ( void const * _data_, ssize_t _size_ ) ;

        constexpr auto       & _buffer ()       noexcept { return buffer_ ; }
        constexpr auto const & _buffer () const noexcept { return buffer_ ; }

        constexpr buffer_layout       & _layout ()       noexcept { return layout_ ; }
        constexpr buffer_layout const & _layout () const noexcept { return layout_ ; }

        constexpr void _release () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////

class index_buffer : public index_buffer_base< index_buffer >
{
        using _self = index_buffer               ;
        using _base = index_buffer_base< _self > ;
public:
        constexpr index_buffer ( u32_t * _indices_, u32_t _count_ ) ;

        constexpr ~index_buffer () noexcept { _release() ; }
private:
        class buffer buffer_ ;
        u32_t         count_ ;

        constexpr auto       & _buffer ()       noexcept { return buffer_ ; }
        constexpr auto const & _buffer () const noexcept { return buffer_ ; }

        constexpr u32_t       & _count ()       noexcept { return count_ ; }
        constexpr u32_t const & _count () const noexcept { return count_ ; }

        constexpr void _release () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
