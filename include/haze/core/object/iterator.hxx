//
//
//      haze
//      core/object/iterator.hxx
//

#pragma once

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>
#include <haze/core/geometry/rectangle.hxx>
#include <haze/core/geometry/row_iterator.hxx>
#include <haze/core/geometry/column_iterator.hxx>

#include <haze/core/object/meta.hxx>
#include <haze/core/object/pixel.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, typename ShapeObject >
class shape_row_iterator : public row_iterator< typename ShapeObject::shape_type >
{
        using _base = row_iterator< typename ShapeObject::shape_type > ;

        static_assert( meta::shape_object< ShapeObject >, "haze::shape_row_iterator::error : object not a shape object" ) ;
public:
        struct begin_tag {} ;
        struct   end_tag {} ;

        using     pixel_type = PixelType ;
        using     value_type = pixel_type ;
        using raw_pixel_type = uti::remove_cv_t< pixel_type > ;
        using    object_type = ShapeObject ;

        using      shape_type = typename _base::     shape_type ;
        using      point_type = typename _base::     point_type ;
        using coordinate_type = typename _base::coordinate_type ;
        using      ssize_type = typename _base::     ssize_type ;
        using difference_type = typename _base::difference_type ;

        using iterator_category = typename _base::iterator_category ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        using shape_object_pointer   = uti::add_pointer_t         < uti::copy_cv_t< pixel_type, object_type > > ;
        using shape_object_reference = uti::add_lvalue_reference_t< uti::copy_cv_t< pixel_type, object_type > > ;

        using _base::_base ;

        constexpr shape_row_iterator () noexcept = default ;

        constexpr shape_row_iterator ( shape_object_reference _obj_, begin_tag ) noexcept
                : _base( _obj_.shape().bounding_box().normalized(), typename _base::begin_tag{} ), obj_( &_obj_ ) {}

        constexpr shape_row_iterator ( shape_object_reference _obj_, end_tag ) noexcept
                : _base( _obj_.shape().bounding_box().normalized(), typename _base::end_tag{} ), obj_( &_obj_ ) {}

        constexpr shape_row_iterator ( shape_object_reference _obj_, point_type _curr_ ) noexcept
                : _base( _obj_.shape().bounding_box().normalized(), _curr_ ), obj_( &_obj_ ) {}

        constexpr shape_row_iterator             ( shape_row_iterator const &  ) noexcept = default ;
        constexpr shape_row_iterator             ( shape_row_iterator       && ) noexcept = default ;
        constexpr shape_row_iterator & operator= ( shape_row_iterator const &  ) noexcept = default ;
        constexpr shape_row_iterator & operator= ( shape_row_iterator       && ) noexcept = default ;

        constexpr ~shape_row_iterator () noexcept = default ;

        constexpr shape_row_iterator & operator++ (     ) noexcept { _base::operator++() ; return *this ; }
        constexpr shape_row_iterator   operator++ ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

        constexpr shape_row_iterator & operator-- () noexcept { _base::operator--() ; return *this ; }
        constexpr shape_row_iterator operator-- ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

        constexpr shape_row_iterator & operator+= ( difference_type _diff_ ) noexcept { _base::operator+=( _diff_ ) ; return *this ; }

        constexpr shape_row_iterator operator+ ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter += _diff_ ;
                return iter ;
        }
        friend constexpr shape_row_iterator operator+ ( difference_type _diff_, shape_row_iterator _iter_ ) noexcept
        { return _iter_ + _diff_ ; }

        constexpr shape_row_iterator & operator-= ( difference_type _diff_ ) noexcept { _base::operator-=( _diff_ ) ; return *this ; }

        constexpr shape_row_iterator operator- ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter -= _diff_ ;
                return iter ;
        }
        friend constexpr shape_row_iterator operator- ( difference_type _diff_, shape_row_iterator const & _iter_ ) noexcept
        { return _iter_ - _diff_ ; }

        constexpr shape_row_iterator & operator+= ( shape_row_iterator const & _other_ ) noexcept { _base::operator+=( _other_ ) ; return *this ; }

        constexpr shape_row_iterator operator+ ( shape_row_iterator const & _other_ ) const noexcept
        {
                auto iter = *this ;
                iter += _other_ ;
                return iter ;
        }

//      constexpr shape_row_iterator & operator-= ( shape_row_iterator const & _other_ ) noexcept
//      {

//      }
//      constexpr difference_type operator- ( shape_row_iterator const & _other_ ) const noexcept
//      {

//      }

        constexpr bool operator== ( shape_row_iterator const & _other_ ) const noexcept
        { return _base::operator==( _other_ ) && obj_ == _other_.obj_ ; }

        constexpr bool operator!= ( shape_row_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr reference operator*  () const noexcept { return  obj_->at( _base::curr_ ) ; }
        constexpr pointer   operator-> () const noexcept { return &obj_->at( _base::curr_ ) ; }

        constexpr reference operator[] ( ssize_type _idx_ ) const noexcept { return *( *this + _idx_ ) ; }

        constexpr shape_object_reference obj_ref () const noexcept { return *obj_ ; }
private:
        shape_object_pointer obj_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< meta::pixel_like PixelType, typename ShapeObject >
class shape_column_iterator : public column_iterator< typename ShapeObject::shape_type >
{
        using _base = column_iterator< typename ShapeObject::shape_type > ;

        static_assert( meta::shape_object< ShapeObject >, "haze::shape_column_iterator::error : object not a shape object" ) ;
public:
        struct begin_tag {} ;
        struct   end_tag {} ;

        using     pixel_type = PixelType ;
        using     value_type = pixel_type ;
        using raw_pixel_type = uti::remove_cv_t< pixel_type > ;
        using    object_type = ShapeObject ;

        using      shape_type = typename _base::     shape_type ;
        using      point_type = typename _base::     point_type ;
        using coordinate_type = typename _base::coordinate_type ;
        using      ssize_type = typename _base::     ssize_type ;
        using difference_type = typename _base::difference_type ;

        using iterator_category = typename _base::iterator_category ;

        using   pointer = value_type * ;
        using reference = value_type & ;

        using shape_object_pointer   = uti::add_pointer_t         < uti::copy_cv_t< pixel_type, object_type > > ;
        using shape_object_reference = uti::add_lvalue_reference_t< uti::copy_cv_t< pixel_type, object_type > > ;

        using _base::_base ;

        constexpr shape_column_iterator () noexcept = default ;

        constexpr shape_column_iterator ( shape_object_reference _obj_, begin_tag ) noexcept
                : _base( _obj_.shape().bounding_box().normalized(), typename _base::begin_tag{} ), obj_( &_obj_ ) {}

        constexpr shape_column_iterator ( shape_object_reference _obj_, end_tag ) noexcept
                : _base( _obj_.shape().bounding_box().normalized(), typename _base::end_tag{} ), obj_( &_obj_ ) {}

        constexpr shape_column_iterator ( shape_object_reference _obj_, point_type _curr_ ) noexcept
                : _base( _obj_.shape().bounding_box().normalized(), _curr_ ), obj_( &_obj_ ) {}

        constexpr shape_column_iterator             ( shape_column_iterator const &  ) noexcept = default ;
        constexpr shape_column_iterator             ( shape_column_iterator       && ) noexcept = default ;
        constexpr shape_column_iterator & operator= ( shape_column_iterator const &  ) noexcept = default ;
        constexpr shape_column_iterator & operator= ( shape_column_iterator       && ) noexcept = default ;

        constexpr ~shape_column_iterator () noexcept = default ;

        constexpr shape_column_iterator & operator++ (     ) noexcept { _base::operator++() ; return *this ; }
        constexpr shape_column_iterator   operator++ ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

        constexpr shape_column_iterator & operator-- () noexcept { _base::operator--() ; return *this ; }
        constexpr shape_column_iterator operator-- ( int ) noexcept { auto prev = *this ; ++*this ; return prev ; }

        constexpr shape_column_iterator & operator+= ( difference_type _diff_ ) noexcept { _base::operator+=( _diff_ ) ; return *this ; }

        constexpr shape_column_iterator operator+ ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter += _diff_ ;
                return iter ;
        }
        friend constexpr shape_column_iterator operator+ ( difference_type _diff_, shape_column_iterator _iter_ ) noexcept
        { return _iter_ + _diff_ ; }

        constexpr shape_column_iterator & operator-= ( difference_type _diff_ ) noexcept { _base::operator-=( _diff_ ) ; return *this ; }

        constexpr shape_column_iterator operator- ( difference_type _diff_ ) const noexcept
        {
                auto iter = *this ;
                iter -= _diff_ ;
                return iter ;
        }
        friend constexpr shape_column_iterator operator- ( difference_type _diff_, shape_column_iterator const & _iter_ ) noexcept
        { return _iter_ - _diff_ ; }

        constexpr shape_column_iterator & operator+= ( shape_column_iterator const & _other_ ) noexcept { _base::operator+=( _other_ ) ; return *this ; }

        constexpr shape_column_iterator operator+ ( shape_column_iterator const & _other_ ) const noexcept
        {
                auto iter = *this ;
                iter += _other_ ;
                return iter ;
        }

//      constexpr shape_column_iterator & operator-= ( shape_column_iterator const & _other_ ) noexcept
//      {

//      }
//      constexpr difference_type operator- ( shape_column_iterator const & _other_ ) const noexcept
//      {

//      }

        constexpr bool operator== ( shape_column_iterator const & _other_ ) const noexcept
        { return _base::operator==( _other_ ) && obj_ == _other_.obj_ ; }

        constexpr bool operator!= ( shape_column_iterator const & _other_ ) const noexcept
        { return !operator==( _other_ ) ; }

        constexpr reference operator*  () const noexcept { return  obj_->at( _base::curr_ ) ; }
        constexpr pointer   operator-> () const noexcept { return &obj_->at( _base::curr_ ) ; }

        constexpr reference operator[] ( ssize_type _idx_ ) const noexcept { return *( *this + _idx_ ) ; }

        constexpr shape_object_reference obj_ref () const noexcept { return *obj_ ; }
private:
        shape_object_pointer obj_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
