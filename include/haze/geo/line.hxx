//
//
//      haze
//      geo/line.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/geo/point.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_basic_line
{
        using      value_type = T                                    ;
        using      point_type = generic_point_2d< value_type >       ;
        using coordinate_type = typename point_type::coordinate_type ;

        coordinate_type fixed_ ;
        coordinate_type begin_ ;
        coordinate_type   end_ ;
} ;

////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_horizontal_line : public generic_basic_line< T >
{
        using _base = generic_basic_line< T > ;
        using _self = generic_horizontal_line ;

        using      value_type = typename _base::     value_type ;
        using      point_type = typename _base::     point_type ;
        using coordinate_type = typename _base::coordinate_type ;

        UTI_NODISCARD constexpr point_type begin_point () const noexcept { return point_type{ this->begin_, this->fixed_ } ; }
        UTI_NODISCARD constexpr point_type   end_point () const noexcept { return point_type{ this->  end_, this->fixed_ } ; }

        UTI_NODISCARD constexpr coordinate_type begin_row () const noexcept { return this->fixed_ ; }
        UTI_NODISCARD constexpr coordinate_type begin_col () const noexcept { return this->begin_ ; }
        UTI_NODISCARD constexpr coordinate_type   end_row () const noexcept { return this->fixed_ ; }
        UTI_NODISCARD constexpr coordinate_type   end_col () const noexcept { return this->  end_ ; }

        UTI_NODISCARD constexpr value_type length () const noexcept { return this->end_ - this->begin_ ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                return _point_.row() == this->fixed_ &&
                       begin_col() <= _point_.col() && _point_.col() <= end_col() ;
        }
} ;

using  horizontal_line = generic_horizontal_line<  i64_t > ;
using fhorizontal_line = generic_horizontal_line< double > ;

////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_vertical_line : public generic_basic_line< T >
{
        using _base = generic_basic_line< T > ;
        using _self = generic_vertical_line ;

        using      value_type = typename _base::     value_type ;
        using      point_type = typename _base::     point_type ;
        using coordinate_type = typename _base::coordinate_type ;

        UTI_NODISCARD constexpr point_type begin_point () const noexcept { return point_type{ this->fixed_, this->begin_ } ; }
        UTI_NODISCARD constexpr point_type   end_point () const noexcept { return point_type{ this->fixed_, this->  end_ } ; }

        UTI_NODISCARD constexpr coordinate_type begin_row () const noexcept { return this->begin_ ; }
        UTI_NODISCARD constexpr coordinate_type begin_col () const noexcept { return this->fixed_ ; }
        UTI_NODISCARD constexpr coordinate_type   end_row () const noexcept { return this->  end_ ; }
        UTI_NODISCARD constexpr coordinate_type   end_col () const noexcept { return this->fixed_ ; }

        UTI_NODISCARD constexpr value_type length () const noexcept { return this->end_ - this->begin_ ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                return _point_.col() == this->fixed_ &&
                       begin_row() <= _point_.row() && _point_.row() <= end_row() ;
        }
} ;

using  vertical_line = generic_vertical_line<  i64_t > ;
using fvertical_line = generic_vertical_line< double > ;

////////////////////////////////////////////////////////////////////////////////

template< uti::meta::arithmetic T >
struct generic_line
{
        using      value_type = T                                    ;
        using      point_type = generic_point_2d< value_type >       ;
        using coordinate_type = typename point_type::coordinate_type ;

        point_type begin_ ;
        point_type   end_ ;

        UTI_NODISCARD constexpr point_type begin_point () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr point_type   end_point () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr coordinate_type begin_row () const noexcept { return begin_.row() ; }
        UTI_NODISCARD constexpr coordinate_type begin_col () const noexcept { return begin_.col() ; }
        UTI_NODISCARD constexpr coordinate_type   end_row () const noexcept { return   end_.row() ; }
        UTI_NODISCARD constexpr coordinate_type   end_col () const noexcept { return   end_.col() ; }

        UTI_NODISCARD constexpr value_type length () const noexcept { return begin_.distance( end_ ) ; }

        UTI_NODISCARD constexpr bool contains ( point_type _point_ ) const noexcept
        {
                if( _point_ == begin_ || _point_ == end_ ) return true ;

                auto dist_diff = begin_.distance( _point_ ) + _point_.distance( end_ ) - begin_.distance( end_ ) ;
                return -10.0 <= dist_diff && dist_diff <= 10.0 ;
        }
} ;

using  line = generic_line<  i64_t > ;
using fline = generic_line< double > ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
