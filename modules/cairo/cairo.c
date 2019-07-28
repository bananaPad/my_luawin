#include "cairo.h"
#include "cairocnt.h"
#include "stdlib.h"

int exp_cairo_newpdf(lua_State* L);
int exp_cairo_newsvg(lua_State* L);
int exp_cairo_destroy(lua_State* L);

static const char* mod_cairo_name="cairo";
static luaL_Reg mod_cairo_funcs[]={
	{"newpdf",exp_cairo_newpdf},
	{"newsvg",exp_cairo_newsvg},
	{"destroy",exp_cairo_destroy},
	{NULL,NULL}
};

//////////////////////////////////////////////////////////////操作类函数/////////////////////////////////////////////////
const char* mod_cairo_getname(void){
	return mod_cairo_name;
}
void mod_cairo_expose(lua_State* L,char* name){
	Phony* p= (Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_cairo_new(p);
	mod_cairo_luareg(L);
	lua_setmetatable(L,-2);
	lua_setglobal(L,name);
}
void mod_cairo_new(Phony* p){
	p->ID=-1;
	p->name=mod_cairo_name;
	p->ptr=NULL;
}
void mod_cairo_luareg(lua_State* L){
	luaL_newmetatable(L,mod_cairo_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_cairo_funcs,0);
	Util_putTableFunc(L,mod_cairo_funcs);
}
////////////////////////////////////////////////////////暴露给lua的函数//////////////////////////////////////////////
int exp_cairo_newpdf(lua_State* L){
	int n=lua_gettop(L);
	if(n!=3){
		lua_pushnil(L);
		return 1;
	}
	char* fname=lua_tostring(L,1);
	int width=(int)lua_tonumber(L,2);
	int height=(int)lua_tonumber(L,3);
	Phony* p=(Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_cairocnt_newpdf(p,fname,width,height);
	mod_cairocnt_luareg(L);
	lua_setmetatable(L,-2);
	return 1;
}
int exp_cairo_destroy(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=1 || p->name==NULL){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	if(Util_strcmp(mod_cairocnt_getname(),p->name,10)!=0){
		lua_pushnumber(L,2);
		lua_pushstring(L,"must be a cairocnt");
		return 2;
	}
	mod_cairocnt_free(p);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_cairo_newsvg(lua_State* L){
	int n=lua_gettop(L);
	if(n!=3){
		lua_pushnil(L);
		return 1;
	}
	char* fname=lua_tostring(L,1);
	int width=(int)lua_tonumber(L,2);
	int height=(int)lua_tonumber(L,3);
	Phony* p=(Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_cairocnt_newsvg(p,fname,width,height);
	mod_cairocnt_luareg(L);
	lua_setmetatable(L,-2);
	return 1;
}
