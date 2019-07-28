#include "textbox.h"
#include "stdlib.h"

int exp_textbox_settext(lua_State* L);
int mod_textbox_gettext(lua_State* L);

int mod_textbox_setposition(lua_State* L);
int mod_textbox_setwidthheight(lua_State* L);

static const char* mod_textbox_name="textbox";
static luaL_Reg mod_textbox_funcs[]={
	{"settext",exp_textbox_settext},
	{"setposition",mod_textbox_setposition},
	{"setwidthheight",mod_textbox_setwidthheight},
	{"gettext",mod_textbox_gettext},
	{NULL,NULL}
};

typedef struct mod_textbox_Struct
{
	HWND hparent;
	HWND hme;
} mod_textbox_Struct;
//////////////////////////////////////////////////////////////操作类函数/////////////////////////////////////////////////
const char* mod_textbox_getname(void){
	return mod_textbox_name;
}
void mod_textbox_new(Phony* p,HWND hparent,HINSTANCE hInstance,int x,int y,int w,int h){
	int id=Util_ids_newID();
	mod_textbox_Struct *temp;
	p->ID=id;
	p->name=mod_textbox_name;
	temp=(mod_textbox_Struct *)malloc(sizeof(mod_textbox_Struct));
	p->ptr=temp;
	temp->hparent=hparent;
	HWND hme=CreateWindowEx(0, "edit", "", WS_CHILD|WS_VISIBLE|ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_BORDER,
		x, y, w, h, hparent, (HMENU)id, hInstance, NULL);
	temp->hme=hme;
}
void mod_textbox_luareg(lua_State* L){
	luaL_newmetatable(L,mod_textbox_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_textbox_funcs,0);
	Util_putTableFunc(L,mod_textbox_funcs);
}
////////////////////////////////////////////////////////暴露给lua的函数//////////////////////////////////////////////
//改变文本框的显示文字
int exp_textbox_settext(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	char* cap=lua_tostring(L,2);

	mod_textbox_Struct* own=(mod_textbox_Struct*)p->ptr;
	SendMessage(own->hme,WM_SETTEXT,0,(LPARAM)cap);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}

//设置文本框位置
int mod_textbox_setposition(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int x=(int)lua_tonumber(L,2);
	int y=(int)lua_tonumber(L,3);
	mod_textbox_Struct* me=(mod_textbox_Struct*)p->ptr;
	RECT rc;
	GetWindowRect(me->hme,&rc);
	int width=rc.right-rc.left;
	int height=rc.bottom-rc.top;
	MoveWindow(me->hme,x,y,width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//改变文本框的大小
int mod_textbox_setwidthheight(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int width=(int)lua_tonumber(L,2);
	int height=(int)lua_tonumber(L,3);
	mod_textbox_Struct* me=(mod_textbox_Struct*)p->ptr;
	RECT rc;
	GetWindowRect(me->hme,&rc);
	MoveWindow(me->hme,ScreenToClient(me->hme,rc.left),ScreenToClient(me->hme,rc.top),width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//获取文本框内容，最大500字符
int mod_textbox_gettext(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=1){
		lua_pushstring(L,"");
		return 1;
	}
	mod_textbox_Struct* me=(mod_textbox_Struct*)p->ptr;
	char s[500];
	GetWindowText(me->hme,s,500);
	lua_pushstring(L,s);
	return 1;
}