//
//
//      haze
//      gfx/backend/metal/common.hxx
//

#pragma once

#include <haze/core/common/types.hxx>

#include <haze/core/geometry/meta.hxx>
#include <haze/core/geometry/point.hxx>

#define  NS_PRIVATE_IMPLEMENTATION
#define  CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION

UTI_DIAGS_PUSH()
UTI_DIAGS_DISABLE( -Wunused-parameter )
UTI_DIAGS_DISABLE( -Wgnu-anonymous-struct )

#include <metalcpp/Metal/Metal.hpp>
#include <metalcppext/AppKit/AppKit.hpp>
#include <metalcppext/MetalKit/MetalKit.hpp>

UTI_DIAGS_POP()

#include <simd/simd.h>
