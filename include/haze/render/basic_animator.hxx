//
//
//      haze
//      render/basic_animator.hxx
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

#include <haze/render/draw.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< typename Animator >
class animator_traits
{

} ;

////////////////////////////////////////////////////////////////////////////////

struct animation_params
{
        double frame_rate ;
        duration   length ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< typename Interpolator, meta::image_like ImageType >
class basic_line_animator
{
public:
        using interpolator_type = Interpolator ;

        using image_type =          ImageType              ;
        using pixel_type = typename image_type::pixel_type ;
        using ssize_type = typename image_type::ssize_type ;

        using  line_type =          line                  ;
        using shape_type =          line_type             ;
        using point_type = typename line_type::point_type ;

        constexpr basic_line_animator () noexcept = default ;

        constexpr basic_line_animator ( image_type const &  _frame_ ) noexcept : frame_(           _frame_   ) {}
        constexpr basic_line_animator ( image_type       && _frame_ ) noexcept : frame_( UTI_MOVE( _frame_ ) ) {}

        constexpr basic_line_animator             ( basic_line_animator const &  _other_ ) noexcept ;
        constexpr basic_line_animator             ( basic_line_animator       && _other_ ) noexcept ;
        constexpr basic_line_animator & operator= ( basic_line_animator const &  _other_ ) noexcept ;
        constexpr basic_line_animator & operator= ( basic_line_animator       && _other_ ) noexcept ;

        constexpr ~basic_line_animator () noexcept = default ;

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
basic_line_animator< Interpolator, ImageType >::set_animation ( shape_type _line_, pixel_type _stroke_, animation_params const & _params_ ) noexcept
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
basic_line_animator< Interpolator, ImageType >::_generate_next_frame () noexcept
{
        if( frame_.width() * frame_.height() <= 0 ) UTI_UNLIKELY return ;
        if( finished() ) return ;

        auto start_point = interpolator_type::interpolate( state_.shape_.begin_point(), state_.shape_.end_point(), state_.step_count_, state_.step_++ ) ;
        auto   end_point = interpolator_type::interpolate( state_.shape_.begin_point(), state_.shape_.end_point(), state_.step_count_, state_.step_   ) ;

        line_type segment{ start_point, end_point } ;

        haze::draw_shape( frame_, segment, state_.stroke_ ) ;
}

////////////////////////////////////////////////////////////////////////////////

template< meta::image_like ImageType >
class line_animator
{
public:
        using image_type =          ImageType              ;
        using pixel_type = typename image_type::pixel_type ;
        using ssize_type = typename image_type::ssize_type ;

        using  line_type =          line                  ;
        using point_type = typename line_type::point_type ;

        using shape_type = line_type ;

        constexpr line_animator () noexcept = default ;

        constexpr line_animator ( image_type const &  _frame_ ) noexcept : frame_(           _frame_   ) {}
        constexpr line_animator ( image_type       && _frame_ ) noexcept : frame_( UTI_MOVE( _frame_ ) ) {}

        constexpr line_animator             ( line_animator const &  _other_ ) noexcept : frame_(           _other_.frame_   ) , state_( _other_.state_ ) {                       }
        constexpr line_animator             ( line_animator       && _other_ ) noexcept : frame_( UTI_MOVE( _other_.frame_ ) ) , state_( _other_.state_ ) { _other_.state_ = {} ; }
        constexpr line_animator & operator= ( line_animator const &  _other_ ) noexcept { frame_= UTI_MOVE( _other_.frame_   ) ; state_= _other_.state_ ;                         }
        constexpr line_animator & operator= ( line_animator       && _other_ ) noexcept { frame_= UTI_MOVE( _other_.frame_   ) ; state_= _other_.state_ ;   _other_.state_ = {} ; }

        constexpr ~line_animator () noexcept = default ;

        constexpr void set_frame ( image_type const &  _frame_ ) noexcept { frame_ =           _frame_   ; }
        constexpr void set_frame ( image_type       && _frame_ ) noexcept { frame_ = UTI_MOVE( _frame_ ) ; }

        constexpr void set_animation ( line_type const & _line_, pixel_type _stroke_, animation_params const & _params_ ) noexcept
        {
                state_.params_ = _params_ ;
                state_.stroke_ = _stroke_ ;
                state_.  line_ =   _line_ ;

                state_.progress_ = state_.line_.begin_point() ;

                nanoseconds total_nanos = state_.params_.length.to_nanos() ;
                nanoseconds frame_nanos = seconds( 1.0 / state_.params_.frame_rate ) ;

                double num_steps  = total_nanos.val / frame_nanos.val ;
                state_.num_steps_ = static_cast< ssize_type >( num_steps ) ;
                state_. step_len_ = static_cast<     double >( _line_.length() ) / state_.num_steps_ ;

                double slope {} ;

                if(      _line_.begin_point().row() == _line_.end_point().row() ) slope =  0.0 ;
                else if( _line_.begin_point().col() == _line_.end_point().col() ) slope = 90.0 ;
                else
                {
                        fpoint_2d normal_point( _line_.end_point() - _line_.begin_point() ) ;

                        slope = normal_point.y() / normal_point.x() ;
                }
                double angle = std::atan( slope ) ;
                state_.step_.x() = state_.step_len_ * std::cos( angle ) ;
                state_.step_.y() = state_.step_len_ * std::sin( angle ) ;

                state_.is_finished_ = false ;
                state_. is_running_ =  true ;
        }

        constexpr void generate_next_frame () noexcept { _generate_next_frame() ; }

        constexpr void reset () noexcept { state_ = {} ; }

        UTI_NODISCARD constexpr bool finished () const noexcept { return state_.is_finished_ ; }

        UTI_NODISCARD constexpr image_type       &  frame ()       noexcept { return frame_ ; }
        UTI_NODISCARD constexpr image_type const &  frame () const noexcept { return frame_ ; }
        UTI_NODISCARD constexpr image_type const & cframe () const noexcept { return frame_ ; }

        UTI_NODISCARD constexpr image_type && release () noexcept { return UTI_MOVE( frame_ ) ; }

        UTI_NODISCARD constexpr ssize_type expected_frames () const noexcept { return state_.num_steps_ ; }
private:
        struct state
        {
                animation_params params_ { {}, { 0 } } ;
                pixel_type       stroke_ {           } ;
                fpoint_2d      progress_ {           } ;
                ssize_type    num_steps_ {           } ;
                fpoint_2d          step_ {           } ;
                double         step_len_ {       0.0 } ;
                line_type          line_ {           } ;
                bool        is_finished_ {      true } ;
                bool         is_running_ {     false } ;
        } ;

        image_type frame_ ;
        state      state_ ;

        constexpr void _generate_next_frame () noexcept
        {
                if( frame_.width() * frame_.height() <= 0 )      UTI_UNLIKELY return ;
                if( state_.is_finished_ || !state_.is_running_ ) UTI_UNLIKELY return ;

                fpoint_2d step_start{ state_.progress_ } ;
                state_.progress_ += state_.step_ ;

                state_.progress_.x() = std::ceil( state_.progress_.x() ) ;
                state_.progress_.y() = std::ceil( state_.progress_.y() ) ;

                if( state_.progress_.row() > state_.line_.end_point().row() ) state_.progress_.row() = state_.line_.end_point().row() ;
                if( state_.progress_.col() > state_.line_.end_point().col() ) state_.progress_.col() = state_.line_.end_point().col() ;

                line_type step{ { std::ceil( step_start.x() ), std::ceil( step_start.y() ) } ,
                                {      state_.progress_.x()  ,      state_.progress_.y() } } ;

                haze::draw_shape( frame_, step, state_.stroke_ ) ;

                if( state_.progress_.row() >= state_.line_.end_point().row() &&
                    state_.progress_.col() >= state_.line_.end_point().col()  )
                {
                        state_.is_finished_ =  true ;
                        state_.is_running_  = false ;
                }
        }
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze

















