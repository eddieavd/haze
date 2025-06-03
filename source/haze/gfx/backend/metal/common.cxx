//
//
//      haze
//      gfx/backend/metal/common.hxx
//

#include <haze/core/common/types.hxx>


#define  NS_PRIVATE_IMPLEMENTATION
#define  CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION

UTI_DIAGS_PUSH()
UTI_DIAGS_DISABLE( -Wunused-parameter )
UTI_DIAGS_DISABLE( -Wgnu-anonymous-struct )

#include <metalcpp/Foundation/Foundation.hpp>
#include <metalcpp/Metal/Metal.hpp>
#include <metalcpp/QuartzCore/CAMetalLayer.hpp>
#include <metalcpp/QuartzCore/QuartzCore.hpp>
#include <metalcppext/AppKit/AppKit.hpp>
#include <metalcppext/MetalKit/MetalKit.hpp>

UTI_DIAGS_POP()
