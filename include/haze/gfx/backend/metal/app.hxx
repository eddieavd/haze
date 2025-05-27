//
//
//      haze
//      gfx/backend/metal/app.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/app/app_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class app : public app_base< app >
{
        using _self = app ;
        using _base = app_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  app () noexcept ;
        constexpr ~app () noexcept ;
private:
        constexpr void _run () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
