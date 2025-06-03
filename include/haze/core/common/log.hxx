//
//
//      haze
//      core/common/log.hxx
//

#pragma once

#include <uti/core/type/traits.hxx>

#include <haze/core/common/config.hxx>

#include <loguru.hpp>

////////////////////////////////////////////////////////////////////////////////

#define HAZE_LOG_LVL_YAP    3
#define HAZE_LOG_LVL_TRACE  2
#define HAZE_LOG_LVL_DBG    1
#define HAZE_LOG_LVL_INFO   0
#define HAZE_LOG_LVL_WARN  -1
#define HAZE_LOG_LVL_ERROR -2
#define HAZE_LOG_LVL_FATAL -3

#ifndef HAZE_LOG_LVL
#define HAZE_LOG_LVL HAZE_LOG_LVL_INFO
#endif

////////////////////////////////////////////////////////////////////////////////

#define HAZE_CORE_YAP(...)
#define HAZE_CORE_TRACE(...)
#define HAZE_CORE_DBG(...)
#define HAZE_CORE_INFO(...)
#define HAZE_CORE_WARN(...)
#define HAZE_CORE_ERROR(...)
#define HAZE_CORE_FATAL(...)

#define HAZE_YAP(...)
#define HAZE_TRACE(...)
#define HAZE_DBG(...)
#define HAZE_INFO(...)
#define HAZE_WARN(...)
#define HAZE_ERROR(...)
#define HAZE_FATAL(...)

////////////////////////////////////////////////////////////////////////////////

#define HAZE_CORE_YAP_S(...)
#define HAZE_CORE_TRACE_S(...)
#define HAZE_CORE_DBG_S(...)
#define HAZE_CORE_INFO_S(...)
#define HAZE_CORE_WARN_S(...)
#define HAZE_CORE_ERROR_S(...)
#define HAZE_CORE_FATAL_S(...)

#define HAZE_YAP_S(...)
#define HAZE_TRACE_S(...)
#define HAZE_DBG_S(...)
#define HAZE_INFO_S(...)
#define HAZE_WARN_S(...)
#define HAZE_ERROR_S(...)
#define HAZE_FATAL_S(...)

////////////////////////////////////////////////////////////////////////////////

#if HAZE_LOG_LVL >= HAZE_LOG_LVL_YAP
#undef  HAZE_CORE_YAP
#define HAZE_CORE_YAP(...)   VLOG_F(       3, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_TRACE
#undef  HAZE_CORE_TRACE
#define HAZE_CORE_TRACE(...) VLOG_F(       2, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_DBG
#undef  HAZE_CORE_DBG
#define HAZE_CORE_DBG(...)   VLOG_F(       1, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_INFO
#undef  HAZE_CORE_INFO
#define HAZE_CORE_INFO(...)   LOG_F(    INFO, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_WARN
#undef  HAZE_CORE_WARN
#define HAZE_CORE_WARN(...)   LOG_F( WARNING, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_ERROR
#undef  HAZE_CORE_ERROR
#define HAZE_CORE_ERROR(...)  LOG_F(   ERROR, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_FATAL
#undef  HAZE_CORE_FATAL
#define HAZE_CORE_FATAL(...)  LOG_F(   FATAL, "haze::core : " __VA_ARGS__ )
#endif

#if HAZE_LOG_LVL >= HAZE_LOG_LVL_YAP
#undef  HAZE_YAP
#define HAZE_YAP(...)   VLOG_F(       3, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_TRACE
#undef  HAZE_TRACE
#define HAZE_TRACE(...) VLOG_F(       2, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_DBG
#undef  HAZE_DBG
#define HAZE_DBG(...)   VLOG_F(       1, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_INFO
#undef  HAZE_INFO
#define HAZE_INFO(...)   LOG_F(    INFO, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_WARN
#undef  HAZE_WARN
#define HAZE_WARN(...)   LOG_F( WARNING, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_ERROR
#undef  HAZE_ERROR
#define HAZE_ERROR(...)  LOG_F(   ERROR, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_FATAL
#undef  HAZE_FATAL
#define HAZE_FATAL(...)  LOG_F(   FATAL, "haze : " __VA_ARGS__ )
#endif

////////////////////////////////////////////////////////////////////////////////

#if HAZE_LOG_LVL >= HAZE_LOG_LVL_YAP
#undef  HAZE_CORE_YAP_S
#define HAZE_CORE_YAP_S(...)   VLOG_SCOPE_F(       3, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_TRACE
#undef  HAZE_CORE_TRACE_S
#define HAZE_CORE_TRACE_S(...) VLOG_SCOPE_F(       2, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_DBG
#undef  HAZE_CORE_DBG_S
#define HAZE_CORE_DBG_S(...)   VLOG_SCOPE_F(       1, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_INFO
#undef  HAZE_CORE_INFO_S
#define HAZE_CORE_INFO_S(...)   LOG_SCOPE_F(    INFO, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_WARN
#undef  HAZE_CORE_WARN_S
#define HAZE_CORE_WARN_S(...)   LOG_SCOPE_F( WARNING, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_ERROR
#undef  HAZE_CORE_ERROR_S
#define HAZE_CORE_ERROR_S(...)  LOG_SCOPE_F(   ERROR, "haze::core : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_FATAL
#undef  HAZE_CORE_FATAL_S
#define HAZE_CORE_FATAL_S(...)  LOG_SCOPE_F(   FATAL, "haze::core : " __VA_ARGS__ )
#endif

#if HAZE_LOG_LVL >= HAZE_LOG_LVL_YAP
#undef  HAZE_YAP_S
#define HAZE_YAP_S(...)   VLOG_SCOPE_F(       3, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_TRACE
#undef  HAZE_TRACE_S
#define HAZE_TRACE_S(...) VLOG_SCOPE_F(       2, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_DBG
#undef  HAZE_DBG_S
#define HAZE_DBG_S(...)   VLOG_SCOPE_F(       1, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_INFO
#undef  HAZE_INFO_S
#define HAZE_INFO_S(...)   LOG_SCOPE_F(    INFO, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_WARN
#undef  HAZE_WARN_S
#define HAZE_WARN_S(...)   LOG_SCOPE_F( WARNING, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_ERROR
#undef  HAZE_ERROR_S
#define HAZE_ERROR_S(...)  LOG_SCOPE_F(   ERROR, "haze : " __VA_ARGS__ )
#endif
#if HAZE_LOG_LVL >= HAZE_LOG_LVL_FATAL
#undef  HAZE_FATAL_S
#define HAZE_FATAL_S(...)  LOG_SCOPE_F(   FATAL, "haze : " __VA_ARGS__ )
#endif

////////////////////////////////////////////////////////////////////////////////

#ifndef HAZE_ASSERT
#       ifndef HAZE_ASSERTS
#               define HAZE_ASSERT( ... ) ( ( void ) 0 )
#       else
#               define HAZE_ASSERT( cond, ... )                                                                     \
                do                                                                                                   \
                {                                                                                                     \
                        if( !( cond ) )                                                                                \
                        {                                                                                               \
                                HAZE_CORE_ERROR( #cond " failed in " __FILE__ " on line %d - " __VA_ARGS__, __LINE__ ) ; \
                                HAZE_DBG_BREAK() ;                                                                        \
                        }                                                                                                  \
                } while( 0 )
        #endif
#endif

////////////////////////////////////////////////////////////////////////////////

#ifndef HAZE_CEXPR_ASSERT
#       ifndef HAZE_ASSERTS
#               define HAZE_CEXPR_ASSERT( ... ) ( ( void ) 0 )
#       else
#       define HAZE_CEXPR_ASSERT( cond, ... )                \
                UTI_DIAGS_PUSH()                              \
                UTI_DIAGS_DISABLE( -Wunused-but-set-variable ) \
                do                                              \
                {                                                \
                        if( uti::is_constant_evaluated() )        \
                        {                                          \
                                int test { 1 } ;                    \
                                test /= ( cond ) ;                   \
                        }                                             \
                        else                                           \
                        {                                               \
                                HAZE_ASSERT( cond, __VA_ARGS__ ) ;       \
                        }                                                 \
                } while( 0 );                                              \
                UTI_DIAGS_POP()
#       endif
#endif

////////////////////////////////////////////////////////////////////////////////


namespace haze::log
{


////////////////////////////////////////////////////////////////////////////////

static constexpr void init ( int & argc, char ** argv ) noexcept
{
        loguru::g_stderr_verbosity = HAZE_LOG_LVL ;

        loguru::init( argc, argv ) ;

        loguru::add_file( "haze.log", loguru::Append, HAZE_LOG_LVL ) ;

        HAZE_CORE_INFO( "logging system initialized" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::log
