//
//
//      haze
//      render/meta.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/common/time.hxx>
#include <haze/image/meta.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

struct animation_params
{
        double frame_rate ;
        duration   length ;
} ;

////////////////////////////////////////////////////////////////////////////////

namespace meta
{


template< typename Animator >
concept animator_like = requires( Animator anim )
{
        typename Animator::image_type ;
        typename Animator::pixel_type ;
//      typename Animator::shape_type ;
        typename Animator::point_type ;
        typename Animator::ssize_type ;

        { anim.frame() } ;
        image_like< uti::remove_cvref_t< decltype( anim.frame() ) > > ;

        { anim.              reset() } ;
        { anim.generate_next_frame() } ;
        { anim.           finished() } -> uti::meta::same_as< bool > ;
        { anim.release() } -> uti::meta::same_as< typename Animator::image_type && > ;
        { anim.set_frame( uti::declval< typename Animator::image_type const &  >() ) } ;
        { anim.set_frame( uti::declval< typename Animator::image_type       && >() ) } ;
} ;


} // namespace meta

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
