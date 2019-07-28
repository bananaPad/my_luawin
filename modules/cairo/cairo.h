#ifndef __INC_CAIRO__
#define __INC_CAIRO__ 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "../../core.h"
#include <cairo.h>

const char* mod_cairo_getname(void);
void mod_cairo_luareg(lua_State* L);
void mod_cairo_new(Phony* p);
void mod_cairo_expose(lua_State* L,char* name);

#endif
