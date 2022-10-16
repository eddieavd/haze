message( "-- Finding Metal dependencies..." )

find_library( MetalLib      Metal      )
find_library( MetalKitLib   MetalKit   )
find_library( FoundationLib Foundation )

message( "-- LIB : Metal      : ${MetalLib}"      )
message( "-- LIB : MetalKit   : ${MetalKitLib}"   )
message( "-- LIB : Foundation : ${FoundationLib}" )

add_custom_command(
        OUTPUT "ops.metallib"

        COMMAND xcrun -sdk macosx metal -c ${CMAKE_SOURCE_DIR}/include/gpu/metal/ops.metal -o ${CMAKE_SOURCE_DIR}/lib/ops.air
        COMMAND xcrun -sdk macosx metallib ${CMAKE_SOURCE_DIR}/lib/ops.air -o ${CMAKE_SOURCE_DIR}/lib/ops.metallib
        COMMAND cp ${CMAKE_SOURCE_DIR}/lib/ops.metallib ${PROJECT_BINARY_DIR}/ops.metallib
        COMMAND rm ${CMAKE_SOURCE_DIR}/lib/ops.air

        VERBATIM
)
add_custom_target(
        metal_lib
        DEPENDS "ops.metallib"
)

add_dependencies( HAZElib metal_lib )

target_link_libraries( HAZElib PUBLIC
        "-framework Metal"
        "-framework MetalKit"
        "-framework Foundation"
)
