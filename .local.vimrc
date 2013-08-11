"""""""""""""""""""""""""""""""""""""""""""""""""
""
"" Project configuration for Wintermute.
"" Use with git://github.com/jalcine/localrc.vim.
""
"" @author Jacky Alcine <me@jalcine.me>
"" @date   2013-04-03 02:00 AM UTC -05:00
""
"" TODO: Use `cmake.vim` if found to import build opts.
""
"""""""""""""""""""""""""""""""""""""""""""""""""
set expandtab ts=2 sw=2 ai
set shiftwidth=2
set tabstop=2
set wildignore+=build/*
set tags+=.git/tags

" Update Syntastic.
let g:syntastic_cpp_include_dirs+=[ "/usr/include/qt4", "$PWD/src", "$PWD/src/Wintermute" ]

" By default, assume Qt.
au FileReadPost *.cpp* set ft=qt.cpp
au FileReadPost *.hpp* set ft=qt.cpp
au FileReadPost * set tags+=$PWD/.git/tags

" Load tags whenver we 
