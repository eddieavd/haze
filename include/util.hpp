//
//
//      HAZElib
//      util.hpp
//

#pragma once


#define HAZE_MOVE(...) \
        static_cast< std::remove_reference_t< decltype(__VA_ARGS__)>&&>(__VA_ARGS__)

#define HAZE_FWD(...) \
        static_cast< decltype(__VA_ARGS__)&&>(__VA_ARGS__)

#ifdef HAZE_RELEASE
#       define HAZE_ASSERT(cond, ...) ((void)0)
#else
#       include <cstdlib>
#       include <iostream>

#       define HAZE_ASSERT(cond, ...)                                           \
                do                                                               \
                {                                                                 \
                        if( !( cond ) )                                            \
                        {                                                           \
                                std::cerr << "HAZElib: '" << #cond << "' failed in " \
                                          << __FILE__ << ":" << __LINE__              \
                                          << " - " << __VA_ARGS__ << std::endl;        \
                                std::abort();                                           \
                        }                                                                \
                } while( 0 )
#endif

