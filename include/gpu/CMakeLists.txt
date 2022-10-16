add_compile_options( -DGPU )

if( METAL )
        add_subdirectory( metal )
endif()

if( CUDA )
endif()

if( OPENCL )
endif()

if( VULKAN )
endif()

if( OPENGL )
endif()

if( DX11 )
endif()
