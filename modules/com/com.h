#ifndef __INC_COM__
#define __INC_COM__ 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>
#include <winbase.h>
#include "../../core.h"

const char* mod_com_getname(void);
void mod_com_luareg(lua_State* L);
void mod_com_new(Phony* p,char* filename,int baud);

#endif
