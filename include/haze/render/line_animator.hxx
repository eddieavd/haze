//
//
//      haze
//      render/line_animator.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/common/time.hxx>

#include <haze/geo/meta.hxx>
#include <haze/geo/point.hxx>
#include <haze/geo/line.hxx>

#include <haze/image/meta.hxx>
#include <haze/image/pixel.hxx>
#include <haze/image/image.hxx>

#include <haze/render/meta.hxx>
#include <haze/render/draw.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename Interpolator, meta::image_like ImageType >
class line_animator
{
public:
        using interpolator_type = Interpolator ;

        using image_type =          ImageType              ;
        using pixel_type = typename image_type::pixel_type ;
        using ssize_type = typename image_type::ssize_type ;

        using  line_type =          line                  ;
        using shape_type =          line_type             ;
        using point_type = typename line_type::point_type ;

        constexpr line_animator () noexcept = default ;

        constexpr line_animator ( image_type const &  _frame_ ) noexcept : frame_(           _frame_   ) {}
        constexpr line_animator ( image_type       && _frame_ ) noexcept : frame_( UTI_MOVE( _frame_ ) ) {}

        constexpr line_animator             ( line_animator const &  _other_ ) noexcept ;
        constexpr line_animator             ( line_animator       && _other_ ) noexcept ;
        constexpr line_animator & operator= ( line_animator const &  _other_ ) noexcept ;
        constexpr line_animator & operator= ( line_animator       && _other_ ) noexcept ;

        constexpr ~line_animator () noexcept = default ;

        constexpr void set_frame ( image_type const &  _frame_ ) noexcept { frame_ = UTI_FWD( _frame_ ) ; }
        constexpr void set_frame ( image_type       && _frame_ ) noexcept { frame_ = UTI_FWD( _frame_ ) ; }

        constexpr void set_animation ( shape_type _line_, pixel_type _stroke_, animation_params const & _params_ ) noexcept ;

        constexpr void generate_next_frame () noexcept { _generate_next_frame() ; }

        constexpr void reset () noexcept { state_ = {} ; }

        UTI_NODISCARD constexpr bool finished () const noexcept { return state_.step_ == state_.step_count_ ; }

        UTI_NODISCARD constexpr image_type       &  frame ()       noexcept { return frame_  ; }
        UTI_NODISCARD constexpr image_type const &  frame () const noexcept { return frame_  ; }
        UTI_NODISCARD constexpr image_type const & cframe () const noexcept { return frame() ; }

        UTI_NODISCARD constexpr image_type && release () noexcept { return UTI_MOVE( frame_ ) ; }

        UTI_NODISCARD constexpr ssize_type expected_frames () const noexcept { return state_.step_count_ ; }
private:
        struct state_type
        {
                shape_type        shape_ {           } ;
                pixel_type       stroke_ {           } ;
                animation_params params_ { {}, { 0 } } ;
                ssize_type   step_count_ {           } ;
                ssize_type   step_       {           } ;
        } ;

        image_type frame_ ;
        state_type state_ ;

        constexpr void _generate_next_frame () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename Interpolator, meta::image_like ImageType >
constexpr void
line_animator< Interpolator, ImageType >::set_animation ( shape_type _line_, pixel_type _stroke_, animation_params const & _params_ ) noexcept
{
        state_. shape_ =   _line_ ;
        state_.stroke_ = _stroke_ ;
        state_.params_ = _params_ ;

        nanoseconds total_nanos = state_.params_.length.to_nanos() ;
        nanoseconds frame_nanos = seconds( 1.0 / state_.params_.frame_rate ) ;

        state_.step_count_ = total_nanos.val / frame_nanos.val ;
        state_.step_ = 0 ;
}

////////////////////////////////////////////////////////////////////////////////

template< typename Interpolator, meta::image_like ImageType >
constexpr void
line_animator< Interpolator, ImageType >::_generate_next_frame () noexcept
{
        if( frame_.width() * frame_.height() <= 0 ) UTI_UNLIKELY return ;
        if( finished() ) return ;

        auto start_point = interpolator_type::interpolate( state_.shape_.begin_point(), state_.shape_.end_point(), state_.step_count_, state_.step_++ ) ;
        auto   end_point = interpolator_type::interpolate( state_.shape_.begin_point(), state_.shape_.end_point(), state_.step_count_, state_.step_   ) ;

        line_type segment{ start_point, end_point } ;

        haze::draw_line( frame_, segment, state_.stroke_ ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
