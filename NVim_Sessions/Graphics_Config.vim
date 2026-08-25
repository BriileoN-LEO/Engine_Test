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
badd +13 Graphics/Public/Graphics_Interface/IGraphics_brii.h
badd +21 Graphics/Private/IG_OpenGL/IGraphics_OpenGL.h
badd +10 Graphics/Private/IG_OpenGL/IGraphics_OpenGL.cpp
badd +37 learningOpenGL/learningOpenGL.cpp
badd +10 brii_CORE/Settings/Settings_BriiEngine.h
badd +14 Graphics/Public/RHI_Settings/RHI_Factory.cpp
badd +18 Platform/Private/Platform_SDL/SDL3_window.cpp
badd +16 Platform/Public/Platform_Brii/Platform_Window.h
badd +11 brii_CORE/systemManager/platform_Manager.h
badd +8 brii_CORE/systemManager/logger.h
badd +35 brii_CORE/systemManager/logger.cpp
badd +4 Platform/Private/Platform_SDL/SDL3_libraries.h
badd +8 Graphics/Public/RHI_Settings/RHI_Factory.h
badd +38 brii_CORE/files_CoreManager/files_Core.h
badd +10 brii_CORE/Application_Engine/Private/Application_PC.cpp
badd +6 brii_CORE/Application_Engine/Public/Application.h
badd +91 brii_CORE/CMakeLists.txt
badd +5 Platform/Private/Platform_SDL/SDL3_Settings.h
badd +1 Platform/Private/Platform_SDL/SDL3_Settings.cpp
badd +19 Platform/Private/Platform_SDL/SDL3_WindowSettings.h
badd +1 brii_CORE/material_binFormat.h
badd +80 learningOpenGL/CMakeLists.txt
badd +29 brii_COOKER/CMakeLists.txt
badd +1 learningOpenGL/learningOpenGL.h
badd +7 brii_CORE/texturesPack_binFormat.h
badd +1 Platform/Private/Platform_SDL/SDL3_WindowDefinitions.h
badd +109 Platform/Private/Platform_SDL/SDL3_WindowDefinitions.cpp
badd +1 Platform/Private/Platform_SDL/SDL3_Input.cpp
badd +39 Platform/Private/Platform_SDL/SDL3_Definitions/SDL3_WindowDefinitions.h
badd +1 Platform/Private/Platform_SDL/SDL3_Definitions/SDL3_WindowDefinitions.cpp
badd +2 Platform/Private/Platform_SDL/SDL3_Definitions/SDL3_libraries.h
argglobal
%argdel
$argadd ~/CPP_proyects/Engine_Test/
edit Platform/Private/Platform_SDL/SDL3_Definitions/SDL3_WindowDefinitions.h
argglobal
balt Platform/Private/Platform_SDL/SDL3_window.cpp
setlocal foldmethod=manual
setlocal foldexpr=v:lua.vim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
sil! 3,4fold
sil! 7,9fold
sil! 16,33fold
sil! 41,48fold
sil! 39,50fold
let &fdl = &fdl
let s:l = 37 - ((27 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 37
normal! 03|
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
