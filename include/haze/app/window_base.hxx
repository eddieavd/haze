//
//
//      haze
//      app/window_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

struct window_options
{
        ssize_t  width ;
        ssize_t height ;
        string   title ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename WindowImpl >
class window_base
{
        using _impl = WindowImpl ;
public:
        UTI_NODISCARD constexpr bool closed () const noexcept { return static_cast< _impl const * >( this )->_closed() ; }

        UTI_NODISCARD constexpr auto       & clear_color ()       noexcept { return static_cast< _impl       * >( this )->_clear_color() ; }
        UTI_NODISCARD constexpr auto const & clear_color () const noexcept { return static_cast< _impl const * >( this )->_clear_color() ; }

        constexpr void destroy () noexcept { static_cast< _impl * >( this )->_destroy() ; }
protected:
        constexpr  window_base () noexcept = default ;
        constexpr ~window_base () noexcept = default ;

} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
