//
//
//      haze
//      gfx/backend/metal/context.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/gfx/backend/metal/common.hxx>

#include <haze/gfx/context_base.hxx>


namespace haze::mtl
{


////////////////////////////////////////////////////////////////////////////////

class context : public context_base<  context       >
              , public ns_object< MTL::Device       >
              , public ns_object< MTL::CommandQueue >
{
        using     _self = context                        ;
        using _ctx_base = context_base<    _self       > ;
        using  _ns_base = ns_object< MTL::Device       > ;
        using  _cq_base = ns_object< MTL::CommandQueue > ;
public:
        using  _ctx_base::_ctx_base ;
        friend _ctx_base            ;

        constexpr context ()
                : _ctx_base()
                , _ns_base(  MTL::CreateSystemDefaultDevice() )
                , _cq_base( _ns_base::ptr_->newCommandQueue() ) {}
} ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::mtl
