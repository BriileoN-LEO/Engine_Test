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
badd +4125 term://~/CPP_proyects/Engine_Test//26815:/usr/bin/bash
badd +334 learningOpenGL/Model_Assimp/ModelAssimp.h
badd +1288 learningOpenGL/Model_Assimp/ModelAssimp.cpp
badd +263 learningOpenGL/resource_Manager/resourceManager.h
badd +1077 learningOpenGL/resource_Manager/resourceManager.cpp
badd +508 learningOpenGL/learningOpenGL.cpp
badd +575 learningOpenGL/Render/Render.cpp
badd +1817 learningOpenGL/Render/RenderData.cpp
badd +239 learningOpenGL/Render/RenderData.h
badd +147 brii_CORE/mesh_binFormat.h
badd +12 brii_CORE/model_binFormat.h
badd +1 brii_CORE/dataManager/convertion_DataManager.h
badd +1 brii_CORE/dataManager/convertion_DataManager.cpp
badd +71 brii_CORE/material_binFormat.h
badd +35 brii_CORE/texturesPack_binFormat.h
badd +68 brii_CORE/dataManager/algorithms_brii.h
badd +1 brii_CORE/dataManager/containerTypes_manager.h
badd +1 learningOpenGL/textureData/textureManager.cpp
badd +210 learningOpenGL/textureData/textureManager.h
badd +16 learningOpenGL/resource_Manager/material_Manager.h
badd +9 learningOpenGL/resource_Manager/material_Manager.cpp
badd +128 learningOpenGL/Render/Clustered_Shading_Render.h
badd +593 learningOpenGL/Render/Clustered_Shading_Render.cpp
badd +13 learningOpenGL/resource_Manager/Render_Manager.h
badd +55 learningOpenGL/SHADER_H.cpp
badd +139 learningOpenGL/SHADER_H.h
badd +124 learningOpenGL/resource_Manager/shader_Manager.h
badd +1 learningOpenGL/resource_Manager/shader_Manager.cpp
badd +15 shaders/SHADER_CLUSTERED_SHADING/BEAUTY_PASS/fShader_beautyPass.frag
badd +40 learningOpenGL/resource_Manager/bufferShader_Manager.h
badd +1 learningOpenGL/resource_Manager/bufferShader_Manager.cpp
badd +1 ~/CPP_proyects/Engine_Test
argglobal
%argdel
$argadd ~/CPP_proyects/Engine_Test
edit learningOpenGL/resource_Manager/material_Manager.h
argglobal
balt learningOpenGL/textureData/textureManager.h
setlocal foldmethod=manual
setlocal foldexpr=v:lua.vim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
sil! 4,8fold
sil! 12,16fold
sil! 18,28fold
sil! 31,50fold
sil! 10,53fold
sil! 1,59fold
let &fdl = &fdl
1
sil! normal! zo
10
sil! normal! zo
12
sil! normal! zc
let s:l = 47 - ((39 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 47
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
