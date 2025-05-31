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

#include <haze/gfx/common.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

enum class shader_data_type
{
        NONE = 0 ,
        FLOAT    ,
        FLOAT2   ,
        FLOAT3   ,
        FLOAT4   ,
        FLOAT3X3 ,
        FLOAT4X4 ,
        INT      ,
        INT2     ,
        INT3     ,
        INT4     ,
        BOOL     ,
} ;

constexpr u32_t shader_data_type_size ( shader_data_type _type_ ) noexcept
{
        switch( _type_ )
        {
                case shader_data_type::FLOAT    : return 4         ;
                case shader_data_type::FLOAT2   : return 4 * 2     ;
                case shader_data_type::FLOAT3   : return 4 * 3     ;
                case shader_data_type::FLOAT4   : return 4 * 4     ;
                case shader_data_type::FLOAT3X3 : return 4 * 3 * 3 ;
                case shader_data_type::FLOAT4X4 : return 4 * 3 * 3 ;
                case shader_data_type::INT      : return 4         ;
                case shader_data_type::INT2     : return 4 * 2     ;
                case shader_data_type::INT3     : return 4 * 3     ;
                case shader_data_type::INT4     : return 4 * 4     ;
                case shader_data_type::BOOL     : return 1         ;
                default                         :
                {
                        HAZE_CEXPR_ASSERT( uti::always_false_v< shader_data_type >, "unknown shader data type!" ) ;
                        return 0 ;
                }
        }
}

////////////////////////////////////////////////////////////////////////////////

struct buffer_element
{
        string           name ;
        shader_data_type type ;
        u32_t            size ;
        size_t         offset ;
        bool       normalized ;

        constexpr buffer_element () noexcept = default ;

        constexpr buffer_element ( shader_data_type _type_ , string_view _name_ , bool _normalized_ )
                : name(                        _name_   )
                , type(                        _type_   )
                , size( shader_data_type_size( _type_ ) )
                , offset(                           0   )
                , normalized(            _normalized_   )
        {}

        u32_t component_count () const noexcept
        {
                switch( type )
                {
                        case shader_data_type::FLOAT    : return 1 ;
                        case shader_data_type::FLOAT2   : return 2 ;
                        case shader_data_type::FLOAT3   : return 3 ;
                        case shader_data_type::FLOAT4   : return 4 ;
                        case shader_data_type::FLOAT3X3 : return 3 ;
                        case shader_data_type::FLOAT4X4 : return 4 ;
                        case shader_data_type::INT      : return 1 ;
                        case shader_data_type::INT2     : return 2 ;
                        case shader_data_type::INT3     : return 3 ;
                        case shader_data_type::INT4     : return 4 ;
                        case shader_data_type::BOOL     : return 1 ;
                        default                         :
                        {
                                HAZE_CEXPR_ASSERT( uti::always_false_v< shader_data_type >, "unknown shader data type!" ) ;
                                return 0 ;
                        }
                }
        }
} ;

////////////////////////////////////////////////////////////////////////////////

class buffer_layout
{
public:
        using           storage_type = vector< buffer_element >                      ;
        using               iterator = typename storage_type::              iterator ;
        using         const_iterator = typename storage_type::        const_iterator ;
        using       reverse_iterator = typename storage_type::      reverse_iterator ;
        using const_reverse_iterator = typename storage_type::const_reverse_iterator ;

        constexpr buffer_layout () noexcept = default ;

        template< typename... Elems >
                requires( uti::conjunction_v< uti::is_convertible< Elems, buffer_element >... > )
        constexpr buffer_layout ( Elems&&... _elems_ )
                : elements_( sizeof...( _elems_ ) )
        {
                ( ..., elements_.emplace_back( UTI_FWD( _elems_ ) ) ) ;
                _calculate_offsets_and_stride() ;
        }

        UTI_NODISCARD constexpr u32_t                  stride () const noexcept { return   stride_ ; }
        UTI_NODISCARD constexpr storage_type const & elements () const noexcept { return elements_ ; }

        UTI_NODISCARD constexpr       iterator  begin ()       noexcept { return elements_. begin() ; }
        UTI_NODISCARD constexpr const_iterator  begin () const noexcept { return elements_.cbegin() ; }
        UTI_NODISCARD constexpr const_iterator cbegin () const noexcept { return elements_.cbegin() ; }

        UTI_NODISCARD constexpr       iterator  end ()       noexcept { return elements_. end() ; }
        UTI_NODISCARD constexpr const_iterator  end () const noexcept { return elements_.cend() ; }
        UTI_NODISCARD constexpr const_iterator cend () const noexcept { return elements_.cend() ; }

        UTI_NODISCARD constexpr       reverse_iterator  rbegin ()       noexcept { return elements_. rbegin() ; }
        UTI_NODISCARD constexpr const_reverse_iterator  rbegin () const noexcept { return elements_.crbegin() ; }
        UTI_NODISCARD constexpr const_reverse_iterator crbegin () const noexcept { return elements_.crbegin() ; }

        UTI_NODISCARD constexpr       reverse_iterator  rend ()       noexcept { return elements_. rend() ; }
        UTI_NODISCARD constexpr const_reverse_iterator  rend () const noexcept { return elements_.crend() ; }
        UTI_NODISCARD constexpr const_reverse_iterator crend () const noexcept { return elements_.crend() ; }
private:
        storage_type elements_ ;
        u32_t          stride_ ;

        constexpr void _calculate_offsets_and_stride () noexcept
        {
                size_t offset { 0 } ;
                stride_       = 0   ;

                for( auto & elem : elements_ )
                {
                        elem.offset = offset ;
                        offset  += elem.size ;
                        stride_ += elem.size ;
                }
        }
} ;

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

        UTI_NODISCARD constexpr resource_options resource_opts () const noexcept { return static_cast< _impl const * >( this )->_resource_opts() ; }

        UTI_NODISCARD constexpr ssize_type         length () const noexcept { return static_cast< _impl const * >( this )->        _length() ; }
        UTI_NODISCARD constexpr ssize_type allocated_size () const noexcept { return static_cast< _impl const * >( this )->_allocated_size() ; }

        UTI_NODISCARD constexpr u64_t gpu_address () const noexcept { return static_cast< _impl const * >( this )->_gpu_address() ; }

        UTI_NODISCARD constexpr auto       * impl ()       noexcept { return static_cast< _impl       * >( this )->_impl() ; }
        UTI_NODISCARD constexpr auto const * impl () const noexcept { return static_cast< _impl const * >( this )->_impl() ; }

        UTI_NODISCARD constexpr auto       & owner ()       noexcept { return static_cast< _impl       * >( this )->_owner() ; }
        UTI_NODISCARD constexpr auto const & owner () const noexcept { return static_cast< _impl const * >( this )->_owner() ; }

        UTI_NODISCARD constexpr bool   is_aliasable () const noexcept { return static_cast< _impl const * >( this )->  _is_aliasable() ; }
                      constexpr void make_aliasable ()       noexcept {        static_cast< _impl       * >( this )->_make_aliasable() ; }

        UTI_NODISCARD constexpr u64_t offset_in_memory () const noexcept { return static_cast< _impl const * >( this )->_offset_in_memory() ; }

        constexpr void    init ( void * _impl_, void * _owner_, void * _ctx_ )          { static_cast< _impl * >( this )->   _init( _impl_, _owner_, _ctx_ ) ; }
        constexpr void release (                                             ) noexcept { static_cast< _impl * >( this )->_release(                        ) ; }
protected:
        constexpr  buffer_base () noexcept = default ;
        constexpr ~buffer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename BufferImpl >
class vertex_buffer_base
{
        using _impl = BufferImpl ;
public:
        constexpr void upload ( void const * _data_, ssize_t _size_ )
        { static_cast< _impl * >( this )->_upload( _data_, _size_ ) ; }

        constexpr auto       & buffer ()       noexcept { return static_cast< _impl       * >( this )->_buffer() ; }
        constexpr auto const & buffer () const noexcept { return static_cast< _impl const * >( this )->_buffer() ; }

        constexpr buffer_layout       & layout ()       noexcept { return static_cast< _impl       * >( this )->_layout() ; }
        constexpr buffer_layout const & layout () const noexcept { return static_cast< _impl const * >( this )->_layout() ; }

        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }

//      static constexpr auto create (                  ssize_t _size_ ) { return _impl::create(          _size_ ) ; }
//      static constexpr auto create ( float * _verts_, ssize_t _size_ ) { return _impl::create( _verts_, _size_ ) ; }
protected:
        constexpr  vertex_buffer_base () noexcept = default ;
        constexpr ~vertex_buffer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename BufferImpl >
class index_buffer_base
{
        using _impl = BufferImpl ;
public:
//      constexpr void   bind () const { static_cast< _impl const * >( this )->  _bind() ; }
//      constexpr void unbind () const { static_cast< _impl const * >( this )->_unbind() ; }

        constexpr auto       & buffer ()       noexcept { return static_cast< _impl       * >( this )->_buffer() ; }
        constexpr auto const & buffer () const noexcept { return static_cast< _impl const * >( this )->_buffer() ; }

        constexpr u32_t       & count ()       noexcept { return static_cast< _impl       * >( this )->_count() ; }
        constexpr u32_t const & count () const noexcept { return static_cast< _impl const * >( this )->_count() ; }

        constexpr void release () noexcept { static_cast< _impl * >( this )->_release() ; }

//      static constexpr auto create ( u32_t * _indices_, u32_t _count_ )
//      { return _impl::create( _indices_, _count_ ) ; }
protected:
        constexpr  index_buffer_base () noexcept = default ;
        constexpr ~index_buffer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
