//
//
//      haze
//      render/sequential_animator.hxx
//

#pragma once

#include <haze/render/meta.hxx>

#include <uti/core/meta/list.hxx>
#include <uti/core/container/tuple.hxx>
#include <uti/core/container/variant.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////
/// TODO: get _invoke_on_current to return references
///       to avoid code duplication in frame() and release()
///
////////////////////////////////////////////////////////////////////////////////

template< meta::animator_like... Anims >
class sequential_animator
{
        static constexpr ssize_t animator_count { sizeof...( Anims ) } ;
public:
        using animator_types = uti::meta::type_list< Anims... > ;
        using animator_tuple = uti::          tuple< Anims... > ;

        using first_animator_type = uti::meta::list::front_t< animator_types > ;
        using  last_animator_type = uti::meta::list:: back_t< animator_types > ;

        using image_type = typename first_animator_type::image_type ;
        using pixel_type = typename first_animator_type::pixel_type ;
        using point_type = typename first_animator_type::point_type ;
        using ssize_type = typename first_animator_type::ssize_type ;

        constexpr sequential_animator () noexcept = default ;

        constexpr sequential_animator ( image_type const &  _frame_ ) noexcept : animators_() { _get_first().set_frame( UTI_FWD( _frame_ ) ) ; }
        constexpr sequential_animator ( image_type       && _frame_ ) noexcept : animators_() { _get_first().set_frame( UTI_FWD( _frame_ ) ) ; }

        constexpr sequential_animator ( Anims... _anims_ ) noexcept
                : animators_( UTI_FWD( _anims_ )... ) {}

        constexpr void set_frame ( image_type const &  _frame_ ) noexcept { _invoke_on_current( [ & ]( auto & anim ){ anim.set_frame( UTI_FWD( _frame_ ) ) ; } ) ; }
        constexpr void set_frame ( image_type       && _frame_ ) noexcept { _invoke_on_current( [ & ]( auto & anim ){ anim.set_frame( UTI_FWD( _frame_ ) ) ; } ) ; }

        constexpr void generate_next_frame () noexcept { _generate_next_frame() ; }

        constexpr void reset () noexcept { uti::for_each( animators_, []( auto & anim ){ anim.reset() ; } ) ; }

        UTI_NODISCARD constexpr bool finished () const noexcept
        {
                return _get_last().finished() ;
        }

//      UTI_NODISCARD constexpr image_type       & frame ()       noexcept { return _invoke_on_current( []( auto       & anim ) -> image_type       & { return anim.frame() ; } ) ; }
//      UTI_NODISCARD constexpr image_type const & frame () const noexcept { return _invoke_on_current( []( auto const & anim ) -> image_type const & { return anim.frame() ; } ) ; }

        UTI_NODISCARD constexpr image_type & frame () noexcept
        {
                using pointer_type = image_type * ;

                pointer_type frame =
                [ & ]< ssize_type... Idxs >( uti::index_sequence< Idxs... > )
                {
                        return ( ... ||
                        [ & ]
                        {
                                auto & anim = uti::get< Idxs >( animators_ ) ;

                                if( !anim.finished() )
                                {
                                        return &anim.frame() ;
                                }
                                return pointer_type{ nullptr } ;
                        }() ) ;
                }( uti::make_index_sequence< animator_count - 1 >{} ) ;

                if( !frame ) frame = &_get_last().frame() ;

                return static_cast< image_type & >( *frame ) ;
        }
        UTI_NODISCARD constexpr image_type const & frame () const noexcept
        {
                using pointer_type = image_type const * ;

                pointer_type frame =
                [ & ]< ssize_type... Idxs >( uti::index_sequence< Idxs... > )
                {
                        return ( ... ||
                        [ & ]
                        {
                                auto const & anim = uti::get< Idxs >( animators_ ) ;

                                if( !anim.finished() )
                                {
                                        return &anim.frame() ;
                                }
                                return pointer_type{ nullptr } ;
                        }() ) ;
                }( uti::make_index_sequence< animator_count - 1 >{} ) ;

                if( !frame ) frame = &_get_last().frame() ;

                return static_cast< image_type const & >( *frame ) ;
        }

        UTI_NODISCARD constexpr image_type const & cframe () const noexcept { return frame() ; }

//      UTI_NODISCARD constexpr image_type && release () noexcept { return _invoke_on_current( []( auto & anim ) -> image_type && { return anim.release() ; } ) ; }

        UTI_NODISCARD constexpr image_type && release () noexcept
        {
                using pointer_type = image_type * ;

                pointer_type frame =
                [ & ]< ssize_type... Idxs >( uti::index_sequence< Idxs... > )
                {
                        return ( ... ||
                        [ & ]
                        {
                                auto & anim = uti::get< Idxs >( animators_ ) ;

                                if( !anim.finished() )
                                {
                                        return &anim.frame() ;
                                }
                                return pointer_type{ nullptr } ;
                        }() ) ;
                }( uti::make_index_sequence< animator_count - 1 >{} ) ;

                if( !frame ) frame = &_get_last().frame() ;

                return static_cast< image_type && >( *frame ) ;
        }

        template< ssize_t Idx, typename Self >
        UTI_NODISCARD constexpr decltype( auto ) get ( this Self && self ) noexcept
        {
                return uti::get< Idx >( UTI_FWD( self ).animators_ ) ;
        }

        UTI_NODISCARD constexpr ssize_type expected_frames () const noexcept
        {
                ssize_type frame_count {} ;

                uti::for_each( animators_, [ & ]( auto const & anim ){ frame_count += anim.expected_frames() ; } ) ;

                return frame_count ;
        }
private:
        animator_tuple animators_ ;

        constexpr first_animator_type       & _get_first ()       noexcept { return uti::get< 0 >( animators_ ) ; }
        constexpr first_animator_type const & _get_first () const noexcept { return uti::get< 0 >( animators_ ) ; }

        constexpr last_animator_type       & _get_last ()       noexcept { return uti::get< animator_count - 1 >( animators_ ) ; }
        constexpr last_animator_type const & _get_last () const noexcept { return uti::get< animator_count - 1 >( animators_ ) ; }

        template< typename Callable, typename Self >
        requires( uti::meta::invocable< Callable, uti::add_lvalue_reference_t< uti::copy_cv_t< Self, Anims > > > && ... &&
                  uti::meta::same_as< decltype( uti::declval< Callable & >()( uti::declval< uti::add_lvalue_reference_t< uti::copy_cv_t< Self, first_animator_type > > >() ) ), void > )
        constexpr void _invoke_on_current ( this Self && self, Callable _fn_ ) noexcept
        {
                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ||
                        [ & ]
                        {
                                if constexpr( Idxs != animator_count - 1 )
                                {
                                        auto & anim = uti::get< Idxs >( UTI_FWD( self ).animators_ ) ;
                                        if( !anim.finished() )
                                        {
                                                _fn_( anim ) ;
                                                return true ;
                                        }
                                        return false ;
                                }
                                else
                                {
                                        _fn_( UTI_FWD( self )._get_last() ) ;
                                        return true ;
                                }
                        }() ) ;
                }( uti::make_index_sequence< animator_count >{} ) ;
        }

        template< typename Callable, typename Self >
        requires( uti::meta::invocable< Callable, uti::add_lvalue_reference_t< uti::copy_cv_t< Self, Anims > > > && ... &&
                 !uti::meta::same_as< decltype( uti::declval< Callable & >()( uti::declval< uti::add_lvalue_reference_t< uti::copy_cv_t< Self, first_animator_type > > >() ) ), void > )
        constexpr decltype( auto ) _invoke_on_current ( this Self && self, Callable _fn_ ) noexcept
        {
                using raw_result_type = decltype( uti::declval< Callable & >()( uti::declval< uti::add_lvalue_reference_t< uti::copy_cv_t< Self, first_animator_type > > >() ) ) ;

                static constexpr bool result_is_ref { uti::is_reference_v< raw_result_type > } ;

                using result_type = uti::conditional_t< result_is_ref, uti::reference_wrapper< uti::remove_reference_t< raw_result_type > >, raw_result_type > ;

                result_type result ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ||
                        [ & ]
                        {
                                if constexpr( Idxs != animator_count - 1 )
                                {
                                        auto & anim = uti::get< Idxs >( UTI_FWD( self ).animators_ ) ;
                                        if( !anim.finished() )
                                        {
                                                result = _fn_( anim ) ;
                                                return true ;
                                        }
                                        return false ;
                                }
                                else
                                {
                                        result = _fn_( UTI_FWD( self )._get_last() ) ;
                                        return true ;
                                }
                        }() ) ;
                }( uti::make_index_sequence< animator_count >{} ) ;

                return result ;
        }

        constexpr void _generate_next_frame () noexcept
        {
                [ & ]< ssize_type... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ||
                        [ & ]
                        {
                                auto & anim = uti::get< Idxs >( animators_ ) ;

                                if( anim.finished() ) return false ;

                                anim.generate_next_frame() ;

                                if constexpr( Idxs < animator_count - 1 )
                                {
                                        if( anim.finished() )
                                        {
                                                uti::get< Idxs + 1 >( animators_ ).set_frame( UTI_FWD( anim.release() ) ) ;
                                        }
                                }
                                return true ;
                        }() ) ;
                }( uti::make_index_sequence< animator_count >{} ) ;
        }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
