//
//
//      haze
//      gfx/backend/metal/vertex_array.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/vertex_array_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class vertex_array : public vertex_array_base<  vertex_array >
                   , public ns_object< MTL::VertexDescriptor >
{
        using _self = vertex_array               ;
        using _base = vertex_array_base< _self > ;

        using _ns_base = ns_object< MTL::VertexDescriptor > ;
public:
        using  _base::_base ;
        friend _base        ;

        using vertex_buffer_type = typename _base::vertex_buffer_type ;
        using  index_buffer_type = typename _base:: index_buffer_type ;

        using vertex_buffer_storage = typename _base::vertex_buffer_storage ;

        constexpr vertex_array () : _base(), _ns_base( MTL::VertexDescriptor::alloc()->init() ) {}
private:
        ssize_t attribute_count_ {} ;
        ssize_t    layout_count_ {} ;

        constexpr MTL::VertexDescriptor * _v_desc () noexcept { return static_cast< MTL::VertexDescriptor * >( static_cast< _ns_base & >( *this ) ) ; }

        void _push_vertex_buffer ( vertex_buffer_type const &  _vbuffer_ ) ;
        void _push_vertex_buffer ( vertex_buffer_type       && _vbuffer_ ) ;

        void _push_vertex_buffer_impl ( vertex_buffer_type const & _vbuffer_ ) ;

        void _set_index_buffer ( index_buffer_type const &  _idxbuff_ ) ;
        void _set_index_buffer ( index_buffer_type       && _idxbuff_ ) ;

        void _set_index_buffer_impl ( index_buffer_type const & _idxbuff_ ) ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
