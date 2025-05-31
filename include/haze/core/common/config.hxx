//
//
//      haze
//      core/common/config.hxx
//

#pragma once

// #ifndef UTI_HAS_STL
// #define UTI_HAS_STL
// #endif

#include <uti/core/util/config.hxx>

////////////////////////////////////////////////////////////////////////////////

#ifndef HAZE_LOGS
#define HAZE_LOGS
#endif

#ifndef HAZE_ASSERTS
#define HAZE_ASSERTS
#endif

#ifndef HAZE_USE_METAL
#define HAZE_USE_METAL
#endif

////////////////////////////////////////////////////////////////////////////////

#if !defined( UTI_TARGET_MAC )
#       error "haze::core : unsupported platform!"
#endif

#if defined( __arm__ ) || defined( __aarch64__ )
#       define HAZE_DBG_BREAK() asm volatile( "BRK #0" )
#elif defined( __i386__ ) || defined( _M_IX86 ) || defined( __x86_64__ ) || defined( _M_X64 )
#       define HAZE_DBG_BREAK() asm volatile( "int3" )
#else
#       error "haze::core : unsupported architecture!"
#endif

////////////////////////////////////////////////////////////////////////////////

#if !defined( HAZE_USE_METAL ) && !defined( HAZE_USE_OPENGL ) && !defined( HAZE_USE_RAYLIB )
#       error "haze::err : no graphics backend defined!"
#else
#       ifdef HAZE_USE_METAL
#              if defined( HAZE_USE_OPENGL ) || defined( HAZE_USE_RAYLIB )
#                      error "haze::err : multiple graphics backends defined!"
#              endif
#       endif
#       ifdef HAZE_USE_OPENGL
#              if defined( HAZE_USE_METAL ) || defined( HAZE_USE_RAYLIB )
#                      error "haze::err : multiple graphics backends defined!"
#              endif
#       endif
#       ifdef HAZE_USE_RAYLIB
#              if defined( HAZE_USE_METAL ) || defined( HAZE_USE_OPENGL )
#                      error "haze::err : multiple graphics backends defined!"
#              endif
#       endif
#endif

////////////////////////////////////////////////////////////////////////////////
