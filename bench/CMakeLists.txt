if( GBENCH )
        add_executable(
                gbench_haze
                gbench_haze.cpp
        )

        find_package( benchmark REQUIRED )

        target_link_libraries( gbench_haze PUBLIC benchmark::benchmark      )
        target_link_libraries( gbench_haze PUBLIC benchmark::benchmark_main )

        if( METAL )
                target_link_libraries( gbench_haze PUBLIC "-framework Metal" "-framework MetalKit" "-framework Foundation" )
        endif()
endif()
