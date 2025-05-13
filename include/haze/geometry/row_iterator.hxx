//
//
//      haze
//      geometry/row_iterator.hxx
//

#pragma once

#include <haze/geometry/meta.hxx>
#include <haze/geometry/point.hxx>
#include <haze/geometry/rectangle.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::shape_like ShapeType >
class row_iterator
{
public:
        struct begin_tag {} ;
        struct   end_tag {} ;

        using      shape_type = ShapeType ;
        using      point_type = typename shape_type::point_type ;
        using coordinate_type = typename point_type::coordinate_type ;
        using      ssize_type = coordinate_type ;
        using difference_type =      ssize_type ;

        using iterator_category = uti::random_access_iterator_tag ;

        constexpr row_iterator () noexcept = default ;

        constexpr row_iterator ( shape_type _rect_, begin_tag ) noexcept : rect_{ _rect_ }, curr_{ _rect_.begin_point()                                 } {}
        constexpr row_iterator ( shape_type _rect_,   end_tag ) noexcept : rect_{ _rect_ }, curr_{ _rect_.begin_point().x(), _rect_.end_point().y() + 1 } {}

        constexpr row_iterator ( shape_type _rect_, point_type _curr_ ) noexcept : rect_{ _rect_ }, curr_{ _curr_ } {}

        constexpr row_iterator             ( row_iterator const &  ) noexcept = default ;
        constexpr row_iterator             ( row_iterator       && ) noexcept = default ;
        constexpr row_iterator & operator= ( row_iterator const &  ) noexcept = default ;
        constexpr row_iterator & operator= ( row_iterator       && ) noexcept = default ;

        constexpr ~row_iterator () noexcept = default ;

        constexpr row_iterator & operator++ () noexcept
        {
                if( curr_.x() >= rect_.end_x() )
                {
                          curr_.x() = rect_.begin_x() ;
                        ++curr_.y() ;
                }
                else
                {
                        ++curr_.x() ;
                }
                return *this ;
        }
        constexpr row_iterator operator++ ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

        constexpr row_iterator & operator-- () noexcept
        {
                if( curr_.x() <= rect_.begin_x() )
                {
                          curr_.x() = rect_.end_x() ;
                        --curr_.y() ;
                }
                else
                {
                        --curr_.x() ;
                }
                return *this ;
        }
        constexpr row_iterator operator-- ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

//      constexpr row_iterator & operator+= ( difference_type _diff_ ) noexcept
//      {

//      }
        constexpr row_iterator operator+ ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter += _diff_ ;
                return iter ;
        }
        friend constexpr row_iterator operator+ ( difference_type _diff_, row_iterator _iter_ ) noexcept
        { return _iter_ + _diff_ ; }

//      constexpr row_iterator & operator-= ( difference_type _diff_ ) noexcept
//      {

//      }
        constexpr row_iterator operator- ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter -= _diff_ ;
                return iter ;
        }
        friend constexpr row_iterator operator- ( difference_type _diff_, row_iterator const & _iter_ ) noexcept
        { return _iter_ - _diff_ ; }

//      constexpr row_iterator & operator+= ( row_iterator const & _other_ ) noexcept
//      {

//      }
        constexpr row_iterator operator+ ( row_iterator const & _other_ ) const noexcept
        {
                auto iter = *this ;
                iter += _other_ ;
                return iter ;
        }

//      constexpr row_iterator & operator-= ( row_iterator const & _other_ ) noexcept
//      {

//      }
//      constexpr difference_type operator- ( row_iterator const & _other_ ) const noexcept
//      {

//      }

        constexpr bool operator== ( row_iterator const & _other_ ) const noexcept
        { return rect_ == _other_.rect_ && curr_ == _other_.curr_ ; }

        constexpr bool operator!= ( row_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr bool operator< ( row_iterator const & _other_ ) const noexcept
        {
                return  curr_.y() < _other_.curr_.y()
                        ? true
                        : curr_.y() == _other_.curr_.y()
                                ? curr_.x() < _other_.curr_.x()
                                : false ;
        }

        constexpr bool operator<= ( row_iterator const & _other_ ) const noexcept
        {
                return  curr_.y() < _other_.curr_.y()
                        ? true
                        : curr_.y() == _other_.curr_.y()
                                ? curr_.x() <= _other_.curr_.x()
                                : false ;
        }

        constexpr bool operator>  ( row_iterator const & _other_ ) const noexcept { return _other_.operator< ( *this ) ; }
        constexpr bool operator>= ( row_iterator const & _other_ ) const noexcept { return _other_.operator<=( *this ) ; }

        constexpr coordinate_type x () const noexcept { return curr_.x() ; }
        constexpr coordinate_type y () const noexcept { return curr_.y() ; }

        constexpr point_type point () const noexcept { return curr_ ; }
protected:
        shape_type rect_ ;
        point_type curr_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
