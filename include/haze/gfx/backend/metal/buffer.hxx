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

class buffer : public buffer_base< buffer >
{
        using _self = buffer ;
        using _base = buffer_base< _self > ;
public:
        using ssize_type = typename _base::ssize_type ;

        using  _base::_base ;
        friend _base        ;

        constexpr buffer (                        ) noexcept = default ;
        constexpr buffer ( MTL::Buffer * _buffer_ ) noexcept : buffer_{ _buffer_ } {}

        constexpr operator MTL::Buffer       * ()       noexcept { return buffer_ ; }
        constexpr operator MTL::Buffer const * () const noexcept { return buffer_ ; }

        constexpr ~buffer () noexcept { _release() ; }
private:
        MTL::Buffer * buffer_ {} ;

        constexpr bool _is_loaded () const noexcept { return buffer_ != nullptr ; }

        constexpr void _release () noexcept { if( buffer_ ) buffer_->release() ; }

        constexpr void _init ( MTL::Buffer * _buffer_ ) noexcept
        {
                if( buffer_ ) { HAZE_CORE_ERROR( "buffer::init : called on already initialized buffer" ) ; return ; }

                buffer_ = _buffer_ ;
        }

        constexpr void _signal_modified () noexcept { buffer_->didModifyRange( NS::Range::Make( 0, _length() ) ) ; }

        constexpr void       * _data ()       noexcept { return buffer_->contents() ; }
        constexpr void const * _data () const noexcept { return buffer_->contents() ; }

        constexpr ssize_type _length         () const noexcept { return buffer_->       length() ; }
        constexpr ssize_type _allocated_size () const noexcept { return buffer_->allocatedSize() ; }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
