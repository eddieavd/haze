nnoremap <leader>mc :CMakeSelectBuildType metal-debug<CR>
nnoremap <leader>mr :CMakeSelectBuildType metal-release<CR>

let g:cmake_variants =
    \{
    \   'metal-debug' : {
    \       'cmake_build_type' : 'Debug',
    \       'cmake_usr_args'   : '-DGPU=ON -DMETAL=ON -DHAZE_DEBUG=ON'
    \   },
    \   'metal-release' : {
    \       'cmake_build_type' : 'Release',
    \       'cmake_usr_args'   : '-DGPU=ON -DMETAL=ON -DHAZE_RELEASE=ON -DGBENCH=ON'
    \   },
    \   'debug': {
    \       'cmake_build_type' : 'Debug',
    \       'cmake_usr_args'   : '-DGTEST=ON -DGBENCH=ON -DHAZE_DEBUG=ON'
    \   },
    \   'dev-release': {
    \       'cmake_build_type' : 'Release',
    \       'cmake_usr_args'   : '-DGTEST=ON -DGBENCH=ON -DHAZE_RELEASE=ON'
    \   },
    \   'release': {
    \       'cmake_build_type' : 'Release',
    \       'cmake_usr_args'   : '-DGTEST=ON -DGBENCH=ON -DHAZE_RELEASE=ON'
    \   }
    \}
