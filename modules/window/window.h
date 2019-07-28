#ifndef __INC_WINDOW__
#define __INC_WINDOW__

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "../../core.h"

//函数定义
const char* mod_window_getname(void);
void mod_window_luareg(lua_State* L);
void mod_window_sethwnd(HWND hwnd);
void mod_window_setins(HINSTANCE hInstance);
void mod_window_expose(lua_State* L,char* name);
void mod_window_new(Phony* p);

#endif
