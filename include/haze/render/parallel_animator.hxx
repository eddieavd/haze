//
//
//      haze
//      render/parallel_animator.hxx
//

#pragma once

#include <haze/render/meta.hxx>

#include <uti/core/meta/list.hxx>
#include <uti/core/container/tuple.hxx>


namespace haze
{


template< meta::animator_like... Anims >
class parallel_animator
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

        constexpr parallel_animator () noexcept = default ;

        constexpr parallel_animator ( image_type const &  _frame_ ) noexcept : animators_() { _get_first().set_frame( UTI_FWD( _frame_ ) ) ; }
        constexpr parallel_animator ( image_type       && _frame_ ) noexcept : animators_() { _get_first().set_frame( UTI_FWD( _frame_ ) ) ; }

        constexpr parallel_animator ( Anims... _anims_ ) noexcept
                : animators_( UTI_FWD( _anims_ )... ) {}

        constexpr void set_frame ( image_type const &  _frame_ ) noexcept { _get_first().set_frame( UTI_FWD( _frame_ ) ) ; }
        constexpr void set_frame ( image_type       && _frame_ ) noexcept { _get_first().set_frame( UTI_FWD( _frame_ ) ) ; }

        constexpr void generate_next_frame () noexcept { _generate_next_frame() ; }

        constexpr void reset () noexcept { uti::for_each( animators_, []( auto & anim ){ anim.reset() ; } ) ; }

        UTI_NODISCARD constexpr bool finished () const noexcept
        {
                return [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        return ( uti::get< Idxs >( animators_ ).finished() && ... ) ;
                }( uti::make_index_sequence< animator_count >{} ) ;
        }

        UTI_NODISCARD constexpr image_type       &  frame ()       noexcept { return _get_first().frame() ; }
        UTI_NODISCARD constexpr image_type const &  frame () const noexcept { return _get_first().frame() ; }
        UTI_NODISCARD constexpr image_type const & cframe () const noexcept { return              frame() ; }

        UTI_NODISCARD constexpr image_type && release () noexcept { return _get_first().release() ; }

        template< ssize_t Idx, typename Self >
        UTI_NODISCARD constexpr decltype( auto ) get ( this Self && self ) noexcept
        {
                return uti::get< Idx >( UTI_FWD( self ).animators_ ) ;
        }

        UTI_NODISCARD constexpr ssize_type expected_frames () const noexcept
        {
                return [ & ]< ssize_type... Idxs >( uti::index_sequence< Idxs... > )
                {
                        return uti::max( uti::get< Idxs >( animators_ ).expected_frames()... ) ;
                }( uti::make_index_sequence< animator_count >{} ) ;
        }
private:
        animator_tuple animators_ ;

        constexpr first_animator_type       & _get_first ()       noexcept { return uti::get< 0 >( animators_ ) ; }
        constexpr first_animator_type const & _get_first () const noexcept { return uti::get< 0 >( animators_ ) ; }

        constexpr last_animator_type       & _get_last ()       noexcept { return uti::get< animator_count - 1 >( animators_ ) ; }
        constexpr last_animator_type const & _get_last () const noexcept { return uti::get< animator_count - 1 >( animators_ ) ; }

        constexpr void _generate_next_frame () noexcept
        {
                image_type frame( _get_first().release() ) ;

                [ & ]< ssize_t... Idxs >( uti::index_sequence< Idxs... > )
                {
                        ( ... ,
                        [ & ]
                        {
                                auto & animator = uti::get< Idxs >( animators_ ) ;
                                animator.set_frame( UTI_MOVE( frame ) ) ;
                                animator.generate_next_frame() ;
                                frame = animator.release() ;
                        }() ) ;
                }( uti::make_index_sequence< animator_count >{} ) ;

                _get_first().set_frame( UTI_MOVE( frame ) ) ;
        }
} ;


} // namespace haze
