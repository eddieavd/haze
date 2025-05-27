//
//
//      haze
//      core/common/types.hxx
//

#pragma once

#include <haze/core/common/config.hxx>
#include <haze/core/common/log.hxx>

#include <uti/core/allocator/meta.hxx>
#include <uti/core/allocator/default.hxx>
#include <uti/core/allocator/new.hxx>
#include <uti/core/allocator/resource.hxx>
#include <uti/core/allocator/freelist_resource.hxx>

#include <uti/core/container/vector.hxx>
#include <uti/core/container/prefix_array.hxx>

#include <uti/core/string/string.hxx>
#include <uti/core/string/string_view.hxx>


namespace haze
{


////////////////////////////////////////////////////////////////////////////////

using  i8_t = uti:: i8_t ;
using  u8_t = uti:: u8_t ;
using i16_t = uti::i16_t ;
using u16_t = uti::u16_t ;
using i32_t = uti::i32_t ;
using u32_t = uti::u32_t ;
using i64_t = uti::i64_t ;
using u64_t = uti::u64_t ;

using  size_t = uti:: size_t ;
using ssize_t = uti::ssize_t ;

////////////////////////////////////////////////////////////////////////////////

using memory_resource = uti::malloc_resource ;
//using memory_resource = uti::static_freelist_resource< 10 * 1024 * 1024 > ;

template< typename T >
using allocator = uti::allocator< T, memory_resource > ;

// template< typename T >
// using allocator = uti::new_allocator< T > ;

////////////////////////////////////////////////////////////////////////////////

template< typename T >
using vector = uti::vector< T, allocator< T > > ;

////////////////////////////////////////////////////////////////////////////////

template< typename T >
using prefix_array = uti::prefix_array< T, allocator< T > > ;

////////////////////////////////////////////////////////////////////////////////

template< typename CharType >
using generic_string = uti::generic_string< CharType, allocator< CharType > > ;

using string = generic_string< char > ;

////////////////////////////////////////////////////////////////////////////////

using string_view = uti::string_view ;

////////////////////////////////////////////////////////////////////////////////


} // namespace haze
