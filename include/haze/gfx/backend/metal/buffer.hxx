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
        constexpr buffer ( MTL::Buffer * _buffer_, gpu_memory_resource * _owner_ ) : _buff_base(), _ns_base( _buffer_ ), owner_{ _owner_ } {}
private:
        gpu_memory_resource * owner_ ;

        constexpr auto       * _owner ()       noexcept { return owner_ ; }
        constexpr auto const * _owner () const noexcept { return owner_ ; }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
