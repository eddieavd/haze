//
//
//      haze
//      app/window_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename WindowImpl >
class window_base
{
        using _impl = WindowImpl ;
public:
        using point_type = point_2d ;
        using shape_type = generic_rectangle< point_type > ;
        using  size_type = typename point_type::coordinate_type ;

        constexpr void init () noexcept { static_cast< _impl * >( this )->_init() ; }

        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }

        UTI_NODISCARD constexpr size_type   width () const noexcept { return static_cast< _impl const * >( this )-> _width() ; }
        UTI_NODISCARD constexpr size_type  height () const noexcept { return static_cast< _impl const * >( this )->_height() ; }
        UTI_NODISCARD constexpr string_view title () const noexcept { return static_cast< _impl const * >( this )-> _title() ; }
        UTI_NODISCARD constexpr auto        shape () const noexcept { return static_cast< _impl const * >( this )-> _shape() ; }

        UTI_NODISCARD constexpr decltype( auto ) impl ()       noexcept { return static_cast< _impl       * >( this )->_impl() ; }
        UTI_NODISCARD constexpr decltype( auto ) impl () const noexcept { return static_cast< _impl const * >( this )->_impl() ; }
protected:
        constexpr  window_base () noexcept = default ;
        constexpr ~window_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
