//
//
//      haze
//      core/geometry/column_iterator.hxx
//

#pragma once

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::shape_like ShapeType >
class column_iterator
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

        constexpr column_iterator () noexcept = default ;

        constexpr column_iterator ( shape_type _rect_, begin_tag ) noexcept : rect_{ _rect_ }, curr_{ _rect_.begin_point()                                   } {}
        constexpr column_iterator ( shape_type _rect_,   end_tag ) noexcept : rect_{ _rect_ }, curr_{ _rect_.  end_point().x() + 1, _rect_.begin_point().y() } {}

        constexpr column_iterator ( shape_type _rect_, point_type _curr_ ) noexcept : rect_{ _rect_ }, curr_{ _curr_ } {}

        constexpr column_iterator             ( column_iterator const &  ) noexcept = default ;
        constexpr column_iterator             ( column_iterator       && ) noexcept = default ;
        constexpr column_iterator & operator= ( column_iterator const &  ) noexcept = default ;
        constexpr column_iterator & operator= ( column_iterator       && ) noexcept = default ;

        constexpr ~column_iterator () noexcept = default ;

        constexpr column_iterator & operator++ () noexcept
        {
                if( curr_.y() >= rect_.end_y() )
                {
                          curr_.y() = rect_.begin_y() ;
                        ++curr_.x() ;
                }
                else
                {
                        ++curr_.y() ;
                }
                return *this ;
        }
        constexpr column_iterator operator++ ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

        constexpr column_iterator & operator-- () noexcept
        {
                if( curr_.y() <= rect_.begin_y() )
                {
                          curr_.y() = rect_.end_y() ;
                        --curr_.x() ;
                }
                else
                {
                        --curr_.y() ;
                }
                return *this ;
        }
        constexpr column_iterator operator-- ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

//      constexpr column_iterator & operator+= ( difference_type _diff_ ) noexcept
//      {

//      }
        constexpr column_iterator operator+ ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter += _diff_ ;
                return iter ;
        }
        friend constexpr column_iterator operator+ ( difference_type _diff_, column_iterator _iter_ ) noexcept
        { return _iter_ + _diff_ ; }

//      constexpr column_iterator & operator-= ( difference_type _diff_ ) noexcept
//      {

//      }
        constexpr column_iterator operator- ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter -= _diff_ ;
                return iter ;
        }
        friend constexpr column_iterator operator- ( difference_type _diff_, column_iterator const & _iter_ ) noexcept
        { return _iter_ - _diff_ ; }

//      constexpr column_iterator & operator+= ( column_iterator const & _other_ ) noexcept
//      {

//      }
        constexpr column_iterator operator+ ( column_iterator const & _other_ ) const noexcept
        {
                auto iter = *this ;
                iter += _other_ ;
                return iter ;
        }

//      constexpr column_iterator & operator-= ( column_iterator const & _other_ ) noexcept
//      {

//      }
//      constexpr difference_type operator- ( column_iterator const & _other_ ) const noexcept
//      {

//      }

        constexpr bool operator== ( column_iterator const & _other_ ) const noexcept
        { return rect_ == _other_.rect_ && curr_ == _other_.curr_ ; }

        constexpr bool operator!= ( column_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr bool operator< ( column_iterator const & _other_ ) const noexcept
        {
                return  curr_.x() < _other_.curr_.x()
                        ? true
                        : curr_.x() == _other_.curr_.x()
                                ? curr_.y() < _other_.curr_.y()
                                : false ;
        }

        constexpr bool operator<= ( column_iterator const & _other_ ) const noexcept
        {
                return  curr_.x() < _other_.curr_.x()
                        ? true
                        : curr_.x() == _other_.curr_.x()
                                ? curr_.y() <= _other_.curr_.y()
                                : false ;
        }

        constexpr bool operator>  ( column_iterator const & _other_ ) const noexcept { return _other_.operator< ( *this ) ; }
        constexpr bool operator>= ( column_iterator const & _other_ ) const noexcept { return _other_.operator<=( *this ) ; }

        constexpr coordinate_type x () const noexcept { return curr_.x() ; }
        constexpr coordinate_type y () const noexcept { return curr_.y() ; }
protected:
        shape_type rect_ ;
        point_type curr_ ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
