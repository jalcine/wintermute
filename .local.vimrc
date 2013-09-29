"""""""""""""""""""""""""""""""""""""""""""""""""
""
"" Project configuration for Wintermute.
"" Use with git://github.com/jalcine/localrc.vim.
""
"" @author Jacky Alciné <me@jalcine.me>
"" @date   2013-04-03 02:00 AM UTC -05:00
""
"" TODO: Use `cmake.vim` if found to import build opts.
""
"""""""""""""""""""""""""""""""""""""""""""""""""
set expandtab ts=2 sw=2 ai
set shiftwidth=2
set tabstop=2
set tags+=.git/tags

" {{{1 Options
" These options would only make sense if the plug-in in question is installed.
"
" {{{2 VIM-AUTOFORMAT
let g:formatprg_cpp="astyle"
let g:formatprg_args_cs="--options=$PWD/.astyle"

" {{{2 CMAKE.VIM
