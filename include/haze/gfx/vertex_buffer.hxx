//
//
//      haze
//      gfx/vertex_buffer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/buffer.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

class vertex_buffer
{
public:
        using buffer_type = buffer ; 

        constexpr vertex_buffer () noexcept = default ;

        constexpr vertex_buffer ( buffer_type && _buffer_, buffer_layout && _layout_ ) noexcept
                : buffer_( UTI_MOVE( _buffer_ ) )
                , layout_( UTI_MOVE( _layout_ ) )
        {}

        constexpr buffer_type       & buffer ()       noexcept { return buffer_ ; }
        constexpr buffer_type const & buffer () const noexcept { return buffer_ ; }

        constexpr buffer_layout       & layout ()       noexcept { return layout_ ; }
        constexpr buffer_layout const & layout () const noexcept { return layout_ ; }
private:
        buffer_type   buffer_ ;
        buffer_layout layout_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
