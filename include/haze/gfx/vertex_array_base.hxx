//
//
//      haze
//      gfx/vertex_array_base.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/buffer.hxx>
#include <haze/gfx/vertex_buffer.hxx>
#include <haze/gfx/index_buffer.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename VertexArrImpl >
class vertex_array_base
{
        using _impl = VertexArrImpl ;
public:
        using vertex_buffer_type = vertex_buffer ;
        using  index_buffer_type =  index_buffer ;

        using vertex_buffer_storage = vector< vertex_buffer_type > ;

        constexpr void push_vertex_buffer ( vertex_buffer_type const &  _vbuffer_ ) { static_cast< _impl * >( this )->_push_vertex_buffer(           _vbuffer_   ) ; }
        constexpr void push_vertex_buffer ( vertex_buffer_type       && _vbuffer_ ) { static_cast< _impl * >( this )->_push_vertex_buffer( UTI_MOVE( _vbuffer_ ) ) ; }

        constexpr void set_index_buffer ( index_buffer_type const &  _idxbuff_ ) { static_cast< _impl * >( this )->_set_index_buffer(           _idxbuff_   ) ; }
        constexpr void set_index_buffer ( index_buffer_type       && _idxbuff_ ) { static_cast< _impl * >( this )->_set_index_buffer( UTI_MOVE( _idxbuff_ ) ) ; }

        constexpr auto const & vertex_buffers () const noexcept { return vertex_buffers_ ; }
        constexpr auto const &  index_buffer  () const noexcept { return  index_buffer_  ; }
protected:
        u32_t              vertex_buffer_idx_ {} ;
        vertex_buffer_storage vertex_buffers_ {} ;
         index_buffer_type     index_buffer_  {} ;

        constexpr  vertex_array_base () noexcept = default ;
        constexpr ~vertex_array_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
