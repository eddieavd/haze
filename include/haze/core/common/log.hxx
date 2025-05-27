//
//
//      haze
//      core/common/log.hxx
//

#pragma once

#include <loguru.hpp>

////////////////////////////////////////////////////////////////////////////////

#define HAZE_CORE_DBG(...)  VLOG_F(       1, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_INFO(...)  LOG_F(    INFO, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_WARN(...)  LOG_F( WARNING, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_ERROR(...) LOG_F(   ERROR, "haze::core : " __VA_ARGS__ )
#define HAZE_CORE_FATAL(...) LOG_F(   FATAL, "haze::core : " __VA_ARGS__ )

#define HAZE_DBG(...)  VLOG_F(       1, "haze : " __VA_ARGS__ )
#define HAZE_INFO(...)  LOG_F(    INFO, "haze : " __VA_ARGS__ )
#define HAZE_WARN(...)  LOG_F( WARNING, "haze : " __VA_ARGS__ )
#define HAZE_ERROR(...) LOG_F(   ERROR, "haze : " __VA_ARGS__ )
#define HAZE_FATAL(...) LOG_F(   FATAL, "haze : " __VA_ARGS__ )

////////////////////////////////////////////////////////////////////////////////


namespace haze::log
{


static constexpr void init ( int & argc, char ** argv ) noexcept
{
        loguru::init( argc, argv ) ;
}


////////////////////////////////////////////////////////////////////////////////
/// spdlog

/*
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class logger
{
public:
        using logger_type = spdlog::logger ;

        static constexpr void init () ;

        static constexpr std::shared_ptr< logger_type > core_logger () noexcept { return core_logger_ ; }
        static constexpr std::shared_ptr< logger_type >  app_logger () noexcept { return  app_logger_ ; }
private:
        inline static std::shared_ptr< logger_type > core_logger_ {} ;
        inline static std::shared_ptr< logger_type >  app_logger_ {} ;
} ;

////////////////////////////////////////////////////////////////////////////////

#define HAZE_CORE_DBG(...)   ::haze::logger::core_logger()->   trace(__VA_ARGS__)
#define HAZE_CORE_INFO(...)  ::haze::logger::core_logger()->    info(__VA_ARGS__)
#define HAZE_CORE_WARN(...)  ::haze::logger::core_logger()->    warn(__VA_ARGS__)
#define HAZE_CORE_ERROR(...) ::haze::logger::core_logger()->   error(__VA_ARGS__)
#define HAZE_CORE_FATAL(...) ::haze::logger::core_logger()->critical(__VA_ARGS__)

#define HAZE_DBG(...)   ::haze::logger::app_logger()->   trace(__VA_ARGS__)
#define HAZE_INFO(...)  ::haze::logger::app_logger()->    info(__VA_ARGS__)
#define HAZE_WARN(...)  ::haze::logger::app_logger()->    warn(__VA_ARGS__)
#define HAZE_ERROR(...) ::haze::logger::app_logger()->   error(__VA_ARGS__)
#define HAZE_FATAL(...) ::haze::logger::app_logger()->critical(__VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////

constexpr void logger::init ()
{
        std::vector< spdlog::sink_ptr > log_sinks ;

        log_sinks.emplace_back( std::make_shared< spdlog::sinks::stdout_color_sink_mt >() ) ;
        log_sinks.emplace_back( std::make_shared< spdlog::sinks::basic_file_sink_mt >( "haze.core.log", true ) ) ;

        log_sinks.front()->set_pattern( "%^[%T] %n: %v%$" ) ;
        log_sinks. back()->set_pattern( "[%T] [%l] %n: %v" ) ;

        core_logger_ = std::make_shared< logger_type >( "HAZE_CORE", begin( log_sinks ), end( log_sinks ) ) ;
        spdlog::register_logger( core_logger_ ) ;
        core_logger_->set_level( spdlog::level::trace ) ;
        core_logger_-> flush_on( spdlog::level::trace ) ;

        app_logger_ = std::make_shared< logger_type >( "HAZE_APP", begin( log_sinks ), end( log_sinks ) ) ;
        spdlog::register_logger( app_logger_ ) ;
        app_logger_->set_level( spdlog::level::trace ) ;
        app_logger_-> flush_on( spdlog::level::trace ) ;
}
*/

////////////////////////////////////////////////////////////////////////////////


} // namespace haze::log
