#ifndef __INC_BUTTON__
#define __INC_BUTTON__ 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "../../core.h"

const char* mod_button_getname(void);
void mod_button_luareg(lua_State* L);
void mod_button_new(Phony* p,HWND hparent,HINSTANCE hInstance,char* caption,int x,int y,int w,int h);

#endif
