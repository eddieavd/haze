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

        UTI_NODISCARD constexpr size_type  width () const noexcept { return static_cast< _impl const * >( this )-> _width() ; }
        UTI_NODISCARD constexpr size_type height () const noexcept { return static_cast< _impl const * >( this )->_height() ; }
protected:
        constexpr  window_base () noexcept = default ;
        constexpr ~window_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
