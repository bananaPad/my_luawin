#include "button.h"
#include "stdlib.h"

int exp_button_settext(lua_State* L);
int exp_button_setcallback(lua_State* L);
int mod_button_setposition(lua_State* L);
int mod_button_setwidthheight(lua_State* L);

static const char* mod_button_name="button";
static luaL_Reg mod_button_funcs[]={
	{"settext",exp_button_settext},
	{"setcallback",exp_button_setcallback},
	{"setposition",mod_button_setposition},
	{"setwidthheight",mod_button_setwidthheight},
	{NULL,NULL}
};

typedef struct mod_button_Struct
{
	HWND hparent;
	HWND hme;
} mod_button_Struct;
//////////////////////////////////////////////////////////////操作类函数/////////////////////////////////////////////////
const char* mod_button_getname(void){
	return mod_button_name;
}
void mod_button_new(Phony* p,HWND hparent,HINSTANCE hInstance,char* caption,int x,int y,int w,int h){
	int id=Util_ids_newID();
	mod_button_Struct *temp;
	p->ID=id;
	p->name=mod_button_name;
	temp=(mod_button_Struct *)malloc(sizeof(mod_button_Struct));
	p->ptr=temp;
	temp->hparent=hparent;
	HWND hme=CreateWindowEx(0, "button", caption, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		x, y, w, h, hparent, (HMENU)id, hInstance, NULL);
	temp->hme=hme;
}
void mod_button_luareg(lua_State* L){
	luaL_newmetatable(L,mod_button_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_button_funcs,0);
	Util_putTableFunc(L,mod_button_funcs);
}
////////////////////////////////////////////////////////暴露给lua的函数//////////////////////////////////////////////
//改变按钮的显示文字
int exp_button_settext(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	char* cap=lua_tostring(L,2);
	mod_button_Struct* own=(mod_button_Struct*)p->ptr;
	SendMessage(own->hme,WM_SETTEXT,0,(LPARAM)cap);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//为按钮设置点击回调函数
int exp_button_setcallback(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2 || (!lua_isfunction(L,2))){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int callback=luaL_ref(L,LUA_REGISTRYINDEX);
	Util_ids_setcallback(p->ID,"click",callback);	
	lua_pop(L,n);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//设置按钮位置
int mod_button_setposition(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int x=(int)lua_tonumber(L,2);
	int y=(int)lua_tonumber(L,3);
	mod_button_Struct* me=(mod_button_Struct*)p->ptr;
	RECT rc;
	GetWindowRect(me->hme,&rc);
	int width=rc.right-rc.left;
	int height=rc.bottom-rc.top;
	MoveWindow(me->hme,x,y,width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//改变按钮的大小
int mod_button_setwidthheight(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int width=(int)lua_tonumber(L,2);
	int height=(int)lua_tonumber(L,3);
	mod_button_Struct* me=(mod_button_Struct*)p->ptr;
	RECT rc;
	GetWindowRect(me->hme,&rc);
	MoveWindow(me->hme,ScreenToClient(me->hme,rc.left),ScreenToClient(me->hme,rc.top),width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
