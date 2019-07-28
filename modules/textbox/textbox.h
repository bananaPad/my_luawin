#ifndef __INC_TEXTBOX__
#define __INC_TEXTBOX__ 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "../../core.h"

const char* mod_textbox_getname(void);
void mod_textbox_luareg(lua_State* L);
void mod_textbox_new(Phony* p,HWND hparent,HINSTANCE hInstance,int x,int y,int w,int h);

#endif
