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
badd +37 Platform/Private/Platform_SDL/Public/SDL3_window.cpp
badd +1 Platform/Private/Platform_SDL/Public/SDL3_libraries.h
badd +20 Platform/Private/Platform_SDL/Public/SDL3_WindowDefinitions.h
badd +37 Platform/Private/Platform_SDL/Private/Linux/SDL3_WindowDef_Linux.cpp
badd +54 Platform/CMakeLists.txt
badd +234 term://~/CPP_proyects/Engine_Test//49351:/usr/bin/bash
badd +13 Graphics/Private/RHI_Settings_Linux/RHI_Factory_Linux.cpp
badd +36 Platform/Private/Platform_SDL/Public/SDL3_WindowDefinitions.cpp
badd +26 brii_CORE/Settings/Settings_WindowEngine.h
badd +1 brii_CORE/Settings/Settings_UiEngine.h
badd +23 brii_CORE/Settings/Settings_BriiEngine.h
badd +20 brii_CORE/Application_Engine/Public/Application.h
badd +48 Platform/Public/Platform_Brii/Platform_Window.h
badd +44 brii_CORE/dataManager/dataTypes_brii.h
badd +37 brii_CORE/dataManager/convertion_DataManager.h
badd +19 brii_CORE/dataManager/algorithms_brii.h
badd +1 brii_CORE/dataManager/convertion_DataManager.cpp
badd +27 brii_CORE/dataManager/dataTypes_brii.cpp
badd +29 brii_CORE/files_CoreManager/files_Core.h
badd +233 build/release/_deps/sdl3-src/include/SDL3/SDL_video.h
badd +28 brii_CORE/Application_Engine/Private/Application_PC.cpp
badd +47 brii_COOKER/manager_briiCooker.h
badd +22 brii_COOKER/manager_briiCooker.cpp
badd +125 brii_CORE/model_binFormat.h
badd +1 ~/CPP_proyects/Engine_Test
badd +1 brii_CORE/dataManager/containerTypes_manager.h
badd +6 learningOpenGL/optimize_Algorithmics/optimizeAlgorithmics.h
badd +20 brii_CORE/String/stringID_brii.h
badd +65 brii_CORE/String/stringID_brii.cpp
badd +14 learningOpenGL/learningOpenGL.h
badd +93 learningOpenGL/CMakeLists.txt
badd +7 Platform/Private/FileSystem_OS/FileSystem_POSIX.cpp
badd +10 Platform/Public/Platform_Brii/Platform_FileSystem.h
badd +3 brii_CORE/material_binFormat.h
badd +13 brii_CORE/systemManager/platform_Manager.h
badd +1 Platform/Public/Platform_Brii/Platform_Input.h
badd +7 Platform/Private/FileSystem_OS/Linux/FileSystem_POSIX.cpp
badd +220 brii_CORE/mesh_binFormat.h
badd +9 brii_CORE/Settings/Settings_FileSystem.h
badd +1 brii_CORE/texturesPack_binFormat.h
badd +1 assets_engine/Localization/English_Lenguage/Window_EngineBrii/Principal_Window.csv
badd +55 CMakeLists.txt
badd +62 brii_CORE/CMakeLists.txt
badd +1 brii_CORE/main_core.h
badd +1 brii_COOKER/main_Cooker.cpp
badd +1 brii_COOKER/dataCooker.h
badd +1 learningOpenGL/STB_IMAGE_IMPLEMENT.cpp
badd +14 brii_COOKER/CMakeLists.txt
badd +41 Graphics/CMakeLists.txt
argglobal
%argdel
$argadd ~/CPP_proyects/Engine_Test
edit brii_CORE/String/stringID_brii.cpp
argglobal
balt brii_CORE/String/stringID_brii.h
setlocal foldmethod=manual
setlocal foldexpr=v:lua.vim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
sil! 11,14fold
sil! 6,19fold
sil! 27,31fold
sil! 21,34fold
sil! 42,46fold
sil! 36,49fold
sil! 51,55fold
sil! 4,69fold
let &fdl = &fdl
4
sil! normal! zo
let s:l = 57 - ((22 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 57
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
