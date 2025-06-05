//
//
//      haze
//      gfx/index_buffer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/buffer.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

class index_buffer
{
public:
        using buffer_type = buffer ; 

        constexpr index_buffer () noexcept = default ;

        constexpr index_buffer ( buffer_type && _buffer_, u32_t _count_ ) noexcept
                : buffer_( UTI_MOVE( _buffer_ ) ), count_{ _count_ } {}

        constexpr buffer_type       & buffer ()       noexcept { return buffer_ ; }
        constexpr buffer_type const & buffer () const noexcept { return buffer_ ; }

        constexpr u32_t       & count ()       noexcept { return count_ ; }
        constexpr u32_t const & count () const noexcept { return count_ ; }
private:
        buffer_type   buffer_ ;
        u32_t          count_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
