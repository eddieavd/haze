//
//
//      uti
//      core/container/range.hxx
//

#pragma once

#include <uti/core/iterator/meta.hxx>
#include <uti/core/iterator/base.hxx>
#include <uti/core/iterator/reverse_iterator.hxx>
#include <uti/core/algo/advance.hxx>
#include <uti/core/algo/distance.hxx>


namespace uti
{


template< typename Iter >
class range
{
        using _iter_traits = iterator_traits< Iter > ;
public:
        using     iterator_type =           Iter                           ;
        using        value_type = typename _iter_traits::       value_type ;
        using           pointer = typename _iter_traits::          pointer ;
        using         reference = typename _iter_traits::        reference ;
        using   difference_type = typename _iter_traits::  difference_type ;
        using iterator_category = typename _iter_traits::iterator_category ;

        using iterator = iterator_type ;

        constexpr range () noexcept = default ;

        constexpr range ( iterator_type _begin_, iterator_type _end_ ) noexcept
                : begin_( _begin_ ), end_( _end_ ) {}

        constexpr range             ( range const &  ) noexcept = default ;
        constexpr range             ( range       && ) noexcept = default ;
        constexpr range & operator= ( range const &  ) noexcept = default ;
        constexpr range & operator= ( range       && ) noexcept = default ;

        constexpr ~range () noexcept = default ;

        UTI_NODISCARD constexpr reference operator[] ( difference_type _idx_ ) const noexcept
        {
                if constexpr( meta::random_access_iterator< iterator_type > )
                {
                        return begin_[ _idx_ ] ;
                }
                else
                {
                        auto iter = begin_ ;
                        ::uti::advance( iter, _idx_ ) ;
                        return *iter ;
                }
        }

        UTI_NODISCARD constexpr reference at ( difference_type _idx_ ) const noexcept
        {
                if constexpr( meta::random_access_iterator< iterator_type > )
                {
                        return begin_[ _idx_ ] ;
                }
                else
                {
                        auto iter = begin_ ;
                        ::uti::advance( iter, _idx_ ) ;
                        return *iter ;
                }
        }

        UTI_NODISCARD constexpr iterator begin () const noexcept { return begin_ ; }
        UTI_NODISCARD constexpr iterator   end () const noexcept { return   end_ ; }

        UTI_NODISCARD constexpr difference_type size () const noexcept { return ::uti::distance( begin_, end_ ) ; }
private:
        iterator_type begin_ ;
        iterator_type   end_ ;
} ;


} // namespace uti
