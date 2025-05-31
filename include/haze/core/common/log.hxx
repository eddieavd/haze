//
//
//      haze
//      core/common/log.hxx
//

#pragma once

#include <haze/core/common/config.hxx>

#include <loguru.hpp>

////////////////////////////////////////////////////////////////////////////////

#ifndef HAZE_LOGS

#define HAZE_CORE_YAP(...)
#define HAZE_CORE_DBG(...)
#define HAZE_CORE_INFO(...)
#define HAZE_CORE_WARN(...)
#define HAZE_CORE_ERROR(...)
#define HAZE_CORE_FATAL(...)

#define HAZE_YAP(...)
#define HAZE_DBG(...)
#define HAZE_INFO(...)
#define HAZE_WARN(...)
#define HAZE_ERROR(...)
#define HAZE_FATAL(...)

////////////////////////////////////////////////////////////////////////////////

#define HAZE_CORE_YAP_S(...)
#define HAZE_CORE_DBG_S(...)
#define HAZE_CORE_INFO_S(...)
#define HAZE_CORE_WARN_S(...)
#define HAZE_CORE_ERROR_S(...)
#define HAZE_CORE_FATAL_S(...)

#define HAZE_YAP_S(...)
#define HAZE_DBG_S(...)
#define HAZE_INFO_S(...)
#define HAZE_WARN_S(...)
#define HAZE_ERROR_S(...)
#define HAZE_FATAL_S(...)

////////////////////////////////////////////////////////////////////////////////

#else // HAZE_LOGS

#define HAZE_CORE_YAP(...)  VLOG_F(       2, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_DBG(...)  VLOG_F(       1, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_INFO(...)  LOG_F(    INFO, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_WARN(...)  LOG_F( WARNING, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_ERROR(...) LOG_F(   ERROR, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_FATAL(...) LOG_F(   FATAL, "haze::core : " __VA_ARGS__ )

#define HAZE_YAP(...)  VLOG_F(       2, "haze : " __VA_ARGS__ )
#define HAZE_DBG(...)  VLOG_F(       1, "haze : " __VA_ARGS__ )
#define HAZE_INFO(...)  LOG_F(    INFO, "haze : " __VA_ARGS__ )
#define HAZE_WARN(...)  LOG_F( WARNING, "haze : " __VA_ARGS__ )
#define HAZE_ERROR(...) LOG_F(   ERROR, "haze : " __VA_ARGS__ )
#define HAZE_FATAL(...) LOG_F(   FATAL, "haze : " __VA_ARGS__ )

////////////////////////////////////////////////////////////////////////////////

#define HAZE_CORE_YAP_S(...)  VLOG_SCOPE_F(       2, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_DBG_S(...)  VLOG_SCOPE_F(       1, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_INFO_S(...)  LOG_SCOPE_F(    INFO, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_WARN_S(...)  LOG_SCOPE_F( WARNING, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_ERROR_S(...) LOG_SCOPE_F(   ERROR, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_FATAL_S(...) LOG_SCOPE_F(   FATAL, "haze::core : " __VA_ARGS__ )

#define HAZE_YAP_S(...)  VLOG_SCOPE_F(       2, "haze : " __VA_ARGS__ )
#define HAZE_DBG_S(...)  VLOG_SCOPE_F(       1, "haze : " __VA_ARGS__ )
#define HAZE_INFO_S(...)  LOG_SCOPE_F(    INFO, "haze : " __VA_ARGS__ )
#define HAZE_WARN_S(...)  LOG_SCOPE_F( WARNING, "haze : " __VA_ARGS__ )
#define HAZE_ERROR_S(...) LOG_SCOPE_F(   ERROR, "haze : " __VA_ARGS__ )
#define HAZE_FATAL_S(...) LOG_SCOPE_F(   FATAL, "haze : " __VA_ARGS__ )

#endif // HAZE_LOGS

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
        loguru::g_stderr_verbosity = loguru::Verbosity_1 ;

        loguru::init( argc, argv ) ;

        loguru::add_file( "haze.log", loguru::Append, loguru::Verbosity_1 ) ;

        HAZE_CORE_INFO( "logging system initialized" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::log
