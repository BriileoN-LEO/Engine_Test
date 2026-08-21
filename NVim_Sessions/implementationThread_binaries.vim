let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
doautoall SessionLoadPre
silent only
silent tabonly
cd ~/CPP_proyects/Engine_Test
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
set shortmess+=aoO
badd +295 term://~/CPP_proyects/Engine_Test//21048:/usr/bin/bash
badd +539 learningOpenGL/learningOpenGL.cpp
badd +4 learningOpenGL/threadSystem/thread_System.h
badd +47 learningOpenGL/threadSystem/thread_System.cpp
badd +20 brii_CORE/systemManager/threads_platform_Manager.h
badd +11 brii_CORE/files_CoreManager/files_Core.h
badd +17 brii_CORE/threadCore/thread_Manager.h
badd +36 brii_CORE/dataManager/dataTypes_brii.h
badd +3 brii_CORE/threadCore/thread_Manager.cpp
argglobal
%argdel
$argadd .
edit brii_CORE/threadCore/thread_Manager.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 40 + 104) / 208)
exe 'vert 2resize ' . ((&columns * 167 + 104) / 208)
argglobal
enew
file neo-tree\ filesystem\ \[1]
balt brii_CORE/systemManager/threads_platform_Manager.h
setlocal foldmethod=expr
setlocal foldexpr=v:lua.vim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
lcd ~/CPP_proyects/Engine_Test
wincmd w
argglobal
balt ~/CPP_proyects/Engine_Test/brii_CORE/files_CoreManager/files_Core.h
setlocal foldmethod=manual
setlocal foldexpr=v:lua.vim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
sil! 4,7fold
sil! 13,23fold
sil! 12,23fold
sil! 9,25fold
sil! 1,26fold
let &fdl = &fdl
let s:l = 21 - ((20 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 21
normal! 0
lcd ~/CPP_proyects/Engine_Test
wincmd w
2wincmd w
exe 'vert 1resize ' . ((&columns * 40 + 104) / 208)
exe 'vert 2resize ' . ((&columns * 167 + 104) / 208)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
