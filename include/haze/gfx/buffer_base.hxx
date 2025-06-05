//
//
//      haze
//      gfx/buffer.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/common.hxx>


namespace haze
{



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
                return shader_data_type_component_count( type ) ;
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
////////////////////////////////////////////////////////////////////////////////

template< typename BufferImpl >
class buffer_base
{
        using _impl = BufferImpl ;
public:
        UTI_NODISCARD constexpr bool loaded () const noexcept { return static_cast< _impl const * >( this )->_loaded() ; }

        UTI_NODISCARD constexpr void       * data ()       noexcept { return static_cast< _impl       * >( this )->_data() ; }
        UTI_NODISCARD constexpr void const * data () const noexcept { return static_cast< _impl const * >( this )->_data() ; }

        UTI_NODISCARD constexpr auto       * owner ()       noexcept { return static_cast< _impl       * >( this )->_owner() ; }
        UTI_NODISCARD constexpr auto const * owner () const noexcept { return static_cast< _impl const * >( this )->_owner() ; }

        UTI_NODISCARD constexpr resource_options resource_opts () const noexcept { return owner()->resource_opts() ; }

        UTI_NODISCARD constexpr ssize_t capacity () const noexcept { return static_cast< _impl const * >( this )->_capacity() ; }

        UTI_NODISCARD constexpr u64_t      gpu_address () const noexcept { return static_cast< _impl const * >( this )->     _gpu_address() ; }
        UTI_NODISCARD constexpr u64_t offset_in_memory () const noexcept { return static_cast< _impl const * >( this )->_offset_in_memory() ; }

        UTI_NODISCARD constexpr bool   is_aliasable () const noexcept { return static_cast< _impl const * >( this )->  _is_aliasable() ; }
                      constexpr void make_aliasable ()                {        static_cast< _impl       * >( this )->_make_aliasable() ; }

        constexpr void notify_update () noexcept { static_cast< _impl * >( this )->_notify_update() ; }

        constexpr void dump () const noexcept { static_cast< _impl const * >( this )->_dump() ; }

        constexpr void destroy () noexcept { static_cast< _impl * >( this )->_destroy() ; }
protected:
        constexpr  buffer_base () noexcept = default ;
        constexpr ~buffer_base () noexcept = default ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


} // namespace haze
