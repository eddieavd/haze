//
//
//      haze
//      gfx/backend/metal/vertex_array.cxx
//

#include <haze/gfx/backend/metal/vertex_array.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

void vertex_array::_push_vertex_buffer_impl ( vertex_buffer_type const & _vbuffer_ )
{
        HAZE_CORE_INFO_S( "vertex_array::push_vertex_buffer" ) ;

        MTL::VertexAttributeDescriptorArray * attributes = _v_desc()->attributes() ;
        MTL::VertexBufferLayoutDescriptorArray * layouts = _v_desc()->   layouts() ;

        HAZE_ASSERT( _vbuffer_.layout().elements().size() > 0, "vertex_array::push_vertex_buffer : vertex buffer has no layout" ) ;

        auto const & layout = _vbuffer_.layout() ;

        for( auto const & element : layout )
        {
                if( attribute_count_ > HAZE_MTL_MAX_VERTEX_ATTRIB_PER_VERTEX_DESC )
                {
                        HAZE_CORE_ERROR( "vertex_array::push_vertex_buffer : more attributes than supported by architecture" ) ;
                }
                HAZE_CORE_INFO( "vertex_array::push_vertex_buffer : processing element { " SV_FMT " }", SV_ARG( element.name ) ) ;

                MTL::VertexAttributeDescriptor * attribute = MTL::VertexAttributeDescriptor::alloc()->init() ;

                HAZE_CORE_INFO( "vertex_array::push_vertex_buffer : buffer index { %u }", _base::vertex_buffer_idx_ ) ;
                HAZE_CORE_INFO( "vertex_array::push_vertex_buffer : format       { " SV_FMT " }", SV_ARG( shader_data_type_string( element.type ) ) ) ;
                HAZE_CORE_INFO( "vertex_array::push_vertex_buffer : offset       { %zu }", element.offset ) ;

                attribute->setBufferIndex( _base::vertex_buffer_idx_++ ) ;
                attribute->setFormat( MTL::VertexFormat( element.type ) ) ;
                attribute->setOffset( element.offset ) ;

                attributes->setObject( attribute, attribute_count_++ ) ;

                attribute->release() ;
        }
        MTL::VertexBufferLayoutDescriptor * buff_layout = MTL::VertexBufferLayoutDescriptor::alloc()->init() ;

        HAZE_CORE_INFO( "vertex_array::push_vertex_buffer : layout stride { %u }", layout.stride() ) ;

        buff_layout->setStride( layout.stride() ) ;

        layouts->setObject( buff_layout, layout_count_++ ) ;

        buff_layout->release() ;
}

////////////////////////////////////////////////////////////////////////////////

void vertex_array::_push_vertex_buffer ( vertex_buffer_type const & _vbuffer_ )
{
        _push_vertex_buffer_impl( _vbuffer_ ) ;
        _base::vertex_buffers_.push_back( _vbuffer_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

void vertex_array::_push_vertex_buffer ( vertex_buffer_type && _vbuffer_ )
{
        _push_vertex_buffer_impl( _vbuffer_ ) ;
        _base::vertex_buffers_.push_back( UTI_MOVE( _vbuffer_ ) ) ;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void vertex_array::_set_index_buffer_impl ( index_buffer_type const & _idxbuff_ )
{

}

////////////////////////////////////////////////////////////////////////////////

void vertex_array::_set_index_buffer ( index_buffer_type const & _idxbuff_ )
{
        _set_index_buffer_impl( _idxbuff_ ) ;
        _base::index_buffer_  = _idxbuff_ ;
}

////////////////////////////////////////////////////////////////////////////////

void vertex_array::_set_index_buffer ( index_buffer_type && _idxbuff_ )
{
        _set_index_buffer_impl( _idxbuff_ ) ;
        _base::index_buffer_ = UTI_MOVE( _idxbuff_ ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
