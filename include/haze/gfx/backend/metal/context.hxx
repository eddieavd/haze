//
//
//      haze
//      gfx/backend/metal/context.hxx
//

#pragma once

#include <haze/gfx/backend/metal/common.hxx>
#include <haze/gfx/backend/metal/compat.hxx>

#include <haze/gfx/context_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class context : public context_base< context >
{
        using _self = context               ;
        using _base = context_base< _self > ;
public:
        using  _base::_base ;
        friend _base        ;

        constexpr  context () noexcept = default ;
        constexpr ~context () noexcept { if( device_ ) device_->release() ; }

        constexpr context             ( context const &  _other_ ) noexcept :              device_ ( _other_.device_ ) {                           ; device_->retain() ;                }
        constexpr context             ( context       && _other_ ) noexcept :              device_ ( _other_.device_ ) { _other_.device_ = nullptr ;                   ;                }
        constexpr context & operator= ( context const &  _other_ ) noexcept { _release() ; device_ = _other_.device_ ;                             ; device_->retain() ; return *this ; }
        constexpr context & operator= ( context       && _other_ ) noexcept { _release() ; device_ = _other_.device_ ;   _other_.device_ = nullptr ;                   ; return *this ; }

        constexpr MTL::Device       * device ()       noexcept { return device_ ; }
        constexpr MTL::Device const * device () const noexcept { return device_ ; }
private:
        MTL::Device * device_ ;

        constexpr void    _init ()          ;
        constexpr void _release () noexcept ;
} ;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

constexpr void context::_init ()
{
        device_ = MTL::CreateSystemDefaultDevice() ;

        if( !device_ ) HAZE_CORE_FATAL( "context::init : failed to initialize GPU device context!" ) ;
        else           HAZE_CORE_INFO ( "context::init : initialized default GPU device context"   ) ;
}

////////////////////////////////////////////////////////////////////////////////

constexpr void context::_release () noexcept
{
        if( device_ )
        {
                device_->release() ;
                device_ = nullptr ;
        }
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
