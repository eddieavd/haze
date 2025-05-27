//
//
//      haze
//      core/common/config.hxx
//

#pragma once

#define HAZE_USE_METAL


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
