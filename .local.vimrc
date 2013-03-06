"""""""""""""""""""""""""""""""""""""""""""""""""
""
"" Project configuration for Wintermute.
""
"" @author Jacky Alcine <me@jalcine.me>
"" @date   2013-04-03 02:00 AM UTC -05:00
""
"""""""""""""""""""""""""""""""""""""""""""""""""

"{{{ Spacing
" If NERDTree can be found, open it.
if has('NERDTree')
  :NERDTree
end  
" We use two spaces here.
set expandtab ts=2 sw=2 ai
set shiftwidth=2
set tabstop=2
set wildignore+=build/*
"}}}
