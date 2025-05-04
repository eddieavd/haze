//
//
//      haze
//      image/image_view.hxx
//

#pragma once

#include <haze/common/types.hxx>
#include <haze/image/meta.hxx>


namespace haze
{


template< meta::image_like Image >
class image_view
{
public:
        using image_type = Image ;
        using pixel_type = typename image_type::pixel_type ;
        using ssize_type = typename image_type::ssize_type ;
        using value_type = pixel_type ;

        using         pointer = value_type       * ;
        using   const_pointer = value_type const * ;
        using       reference = value_type       & ;
        using const_reference = value_type const & ;

        using       iterator = typename image_type::      iterator ;
        using const_iterator = typename image_type::const_iterator ;

        constexpr image_view ( image_type const & _image_ ) noexcept
                : begin_ ( _image_. begin() )
                ,   end_ ( _image_.   end() )
                , width_ ( _image_. width() )
                , height_( _image_.height() )
        {}

        constexpr image_view             ( image_view const &  ) noexcept = default ;
        constexpr image_view             ( image_view       && ) noexcept = default ;
        constexpr image_view & operator= ( image_view const &  ) noexcept = default ;
        constexpr image_view & operator= ( image_view       && ) noexcept = default ;

        constexpr ~image_view () noexcept = default ;

        template< typename Self >
        UTI_NODISCARD constexpr decltype( auto ) at ( this Self && self, ssize_type _row_, ssize_type _col_ ) noexcept
        { return UTI_FWD( self ).begin_[ _row_ * UTI_FWD( self ).width_ + _col_ ] ; }

        UTI_NODISCARD constexpr uti::view< value_type > row ( ssize_type _row_ ) noexcept
        { return uti::view< pixel_type >( begin_ + _row_ * width_, width_ ) ; }

        UTI_NODISCARD constexpr uti::view< value_type const > row ( ssize_type _row_ ) const noexcept
        { return uti::view< pixel_type const >( begin_ + _row_ * width_, width_ ) ; }

        UTI_NODISCARD constexpr ssize_type  width () const noexcept { return  width_ ; }
        UTI_NODISCARD constexpr ssize_type height () const noexcept { return height_ ; }

        UTI_NODISCARD constexpr       iterator  begin ()       noexcept { return begin_  ; }
        UTI_NODISCARD constexpr const_iterator  begin () const noexcept { return begin_  ; }
        UTI_NODISCARD constexpr const_iterator cbegin () const noexcept { return begin() ; }

        UTI_NODISCARD constexpr       iterator  end ()       noexcept { return end_  ; }
        UTI_NODISCARD constexpr const_iterator  end () const noexcept { return end_  ; }
        UTI_NODISCARD constexpr const_iterator cend () const noexcept { return end() ; }
private:
        iterator    begin_ ;
        iterator      end_ ;
        ssize_type  width_ ;
        ssize_type height_ ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template< meta::image_like Image >
class const_image_view
{
public:
        using image_type = Image ;
        using pixel_type = uti::remove_cv_t< typename image_type::pixel_type > ;
        using ssize_type =                   typename image_type::ssize_type   ;

        using         pointer = pixel_type       * ;
        using   const_pointer = pixel_type const * ;
        using       reference = pixel_type       & ;
        using const_reference = pixel_type const & ;

        using const_iterator = typename image_type::const_iterator ;

        constexpr const_image_view () noexcept = default ;

        constexpr const_image_view ( image_type const & _image_ ) noexcept
                : begin_ ( _image_.cbegin() )
                , end_   ( _image_.  cend() )
                , width_ ( _image_. width() )
                , height_( _image_.height() )
        {}

        constexpr const_image_view             ( const_image_view const &  ) noexcept = default ;
        constexpr const_image_view             ( const_image_view       && ) noexcept = default ;
        constexpr const_image_view & operator= ( const_image_view const &  ) noexcept = default ;
        constexpr const_image_view & operator= ( const_image_view       && ) noexcept = default ;

        constexpr ~const_image_view () noexcept = default ;

        UTI_NODISCARD constexpr const_reference at ( ssize_type _row_, ssize_type _col_ ) const noexcept
        { return begin_[ _row_ * width_ + _col_ ] ; }

        UTI_NODISCARD constexpr uti::view< pixel_type const > row ( ssize_type const _row_ ) const noexcept
        { return uti::view< pixel_type const >( begin_ + _row_ * width_, width_ ) ; }

        UTI_NODISCARD constexpr ssize_type  width () const noexcept { return  width_ ; }
        UTI_NODISCARD constexpr ssize_type height () const noexcept { return height_ ; }

        UTI_NODISCARD constexpr const_iterator  begin () const noexcept { return begin_  ; }
        UTI_NODISCARD constexpr const_iterator cbegin () const noexcept { return begin() ; }

        UTI_NODISCARD constexpr const_iterator  end () const noexcept { return end_  ; }
        UTI_NODISCARD constexpr const_iterator cend () const noexcept { return end() ; }
private:
        const_iterator begin_ ;
        const_iterator   end_ ;
        ssize_type     width_ ;
        ssize_type    height_ ;
} ;


} // namespace haze
