//
//
//      haze
//      gfx/buffer_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>
#include <haze/core/object/image.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename BufferImpl >
class buffer_base
{
        using _impl = BufferImpl ;
public:
        using ssize_type = ssize_t ;

        UTI_NODISCARD constexpr bool is_loaded () const noexcept { return static_cast< _impl const * >( this )->_is_loaded() ; }

        constexpr void signal_modified () noexcept { static_cast< _impl * >( this )->_signal_modified() ; }

        UTI_NODISCARD constexpr void       * data ()       noexcept { return static_cast< _impl       * >( this )->_data() ; }
        UTI_NODISCARD constexpr void const * data () const noexcept { return static_cast< _impl const * >( this )->_data() ; }

        UTI_NODISCARD constexpr ssize_type         length () const noexcept { return static_cast< _impl const * >( this )->        _length() ; }
        UTI_NODISCARD constexpr ssize_type allocated_size () const noexcept { return static_cast< _impl const * >( this )->_allocated_size() ; }

        UTI_NODISCARD constexpr decltype( auto ) impl ()       noexcept { return static_cast< _impl       * >( this )->_impl() ; }
        UTI_NODISCARD constexpr decltype( auto ) impl () const noexcept { return static_cast< _impl const * >( this )->_impl() ; }

        constexpr void    init ( void * _impl_ )          { static_cast< _impl * >( this )->   _init( _impl_ ) ; }
        constexpr void release (               ) noexcept { static_cast< _impl * >( this )->_release(        ) ; }
protected:
        constexpr  buffer_base () noexcept = default ;
        constexpr ~buffer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
