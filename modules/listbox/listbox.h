#ifndef __INC_LISTBOX__
#define __INC_LISTBOX__ 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "../../core.h"

const char* mod_listbox_getname(void);
void mod_listbox_luareg(lua_State* L);
void mod_listbox_new(Phony* p,HWND hparent,HINSTANCE hInstance,int x,int y,int w,int h);

#endif
