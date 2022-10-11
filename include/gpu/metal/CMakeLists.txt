message( "-- Finding Metal dependencies..." )

find_library( MetalLib      Metal      )
find_library( MetalKitLib   MetalKit   )
find_library( FoundationLib Foundation )

message( "-- LIB : Metal      : ${MetalLib}"      )
message( "-- LIB : MetalKit   : ${MetalKitLib}"   )
message( "-- LIB : Foundation : ${FoundationLib}" )

add_link_options( -fno-objc-arc )

list( APPEND EXTRA_LIBS ${MetalLib}      )
list( APPEND EXTRA_LIBS ${MetalKitLib}   )
list( APPEND EXTRA_LIBS ${FoundationLib} )

add_custom_command(
        OUTPUT  "mt_ops.air"
        COMMAND xcrun -sdk macosx metal -c ${CMAKE_SOURCE_DIR}/include/gpu/metal/ops.metal -o ${CMAKE_SOURCE_DIR}/lib/mt_ops.air
        DEPENDS ${CMAKE_SOURCE_DIR}/include/gpu/metal/ops.metal
        VERBATIM
)

add_custom_command(
        OUTPUT "mt_ops.metallib"
        COMMAND xcrun -sdk macosx metallib ${CMAKE_SOURCE_DIR}/lib/mt_ops.air -o ${CMAKE_SOURCE_DIR}/lib/mt_ops.metallib
        DEPENDS ${CMAKE_SOURCE_DIR}/lib/mt_ops.air
        VERBATIM
)

add_custom_command(
        OUTPUT "ops.metallib"
        COMMAND cp ${CMAKE_SOURCE_DIR}/lib/mt_ops.metallib ${PROJECT_BINARY_DIR}/ops.metallib
        DEPENDS ${CMAKE_SOURCE_DIR}/lib/mt_ops.metallib
        VERBATIM
)
