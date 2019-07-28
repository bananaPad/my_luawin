#ifndef __INC_CAIROCNT__
#define __INC_CAIROCNT__ 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include "../../core.h"
#include <cairo.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>

const char* mod_cairocnt_getname(void);
void mod_cairocnt_luareg(lua_State* L);
void mod_cairocnt_new(Phony* p,HDC hdc);
void mod_cairocnt_free(Phony* p);
void mod_cairocnt_newpdf(Phony* p,char* fname,int w,int h);
void mod_cairocnt_newsvg(Phony* p,char* fname,int w,int h);

#endif
