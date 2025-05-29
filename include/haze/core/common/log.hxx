//
//
//      haze
//      core/common/log.hxx
//

#pragma once

#include <loguru.hpp>

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////


namespace haze::log
{


////////////////////////////////////////////////////////////////////////////////

static inline void init ( int & argc, char ** argv ) noexcept
{
        loguru::g_stderr_verbosity = loguru::Verbosity_1 ;

        loguru::init( argc, argv ) ;

        loguru::add_file( "haze.log", loguru::Append, loguru::Verbosity_1 ) ;

        HAZE_CORE_INFO( "logging system initialized" ) ;
}

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::log
