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
badd +6 Graphics/Public/Graphics_Interface/IGraphics_brii.h
badd +9 Graphics/Private/IG_OpenGL/IGraphics_OpenGL.h
badd +6 Graphics/Private/IG_OpenGL/IGraphics_OpenGL.cpp
badd +440 learningOpenGL/learningOpenGL.cpp
badd +30 Graphics/CMakeLists.txt
badd +24 Platform/CMakeLists.txt
badd +161 CMakeLists.txt
badd +254 term://~/CPP_proyects/Engine_Test//15733:/usr/bin/bash
badd +4 brii_CORE/Settings/Settings_BriiEngine.h
badd +8 Graphics/Public/RHI_Settings/RHI_Factory.cpp
badd +17 Platform/Private/Platform_SDL/SDL3_window.cpp
badd +15 Platform/Public/Platform_Brii/Platform_Window.h
badd +11 brii_CORE/systemManager/platform_Manager.h
badd +26 brii_CORE/systemManager/logger.h
badd +35 brii_CORE/systemManager/logger.cpp
argglobal
%argdel
$argadd ./
edit Platform/Private/Platform_SDL/SDL3_window.cpp
argglobal
balt brii_CORE/systemManager/logger.h
setlocal foldmethod=manual
setlocal foldexpr=v:lua.vim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
sil! 3,6fold
sil! 63,71fold
sil! 50,72fold
sil! 37,75fold
sil! 14,77fold
sil! 12,79fold
sil! 82,85fold
sil! 101,103fold
sil! 105,106fold
sil! 99,111fold
sil! 113,117fold
sil! 97,119fold
sil! 133,136fold
let &fdl = &fdl
let s:l = 11 - ((10 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 11
normal! 0
lcd ~/CPP_proyects/Engine_Test
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
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
