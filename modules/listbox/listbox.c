#include "listbox.h"
#include "stdlib.h"

int mod_listbox_setposition(lua_State* L);
int mod_listbox_setwidthheight(lua_State* L);
int exp_listbox_addnew(lua_State* L);
int exp_listbox_getbyind(lua_State* L);
int exp_listbox_gettotalnum(lua_State* L);
int exp_listbox_getselind(lua_State* L);
int exp_listbox_deleteind(lua_State* L);

static const char* mod_listbox_name="listbox";
static luaL_Reg mod_listbox_funcs[]={
	{"setposition",mod_listbox_setposition},
	{"setwidthheight",mod_listbox_setwidthheight},
	{"addnew",exp_listbox_addnew},
	{"getbyind",exp_listbox_getbyind},
	{"gettotalnum",exp_listbox_gettotalnum},
	{"getselind",exp_listbox_getselind},
	{"deleteind",exp_listbox_deleteind},
	{NULL,NULL}
};

typedef struct mod_listbox_Struct
{
	HWND hparent;
	HWND hme;
} mod_listbox_Struct;
//////////////////////////////////////////////////////////////操作类函数/////////////////////////////////////////////////
const char* mod_listbox_getname(void){
	return mod_listbox_name;
}
void mod_listbox_new(Phony* p,HWND hparent,HINSTANCE hInstance,int x,int y,int w,int h){
	int id=Util_ids_newID();
	mod_listbox_Struct *temp;
	p->ID=id;
	p->name=mod_listbox_name;
	temp=(mod_listbox_Struct *)malloc(sizeof(mod_listbox_Struct));
	p->ptr=temp;
	temp->hparent=hparent;
	HWND hme=CreateWindowEx(0, "listbox", NULL, WS_CHILD|WS_VISIBLE|LBS_NOTIFY|LBS_STANDARD,
		x, y, w, h, hparent, (HMENU)id, hInstance, NULL);
	temp->hme=hme;
}
void mod_listbox_luareg(lua_State* L){
	luaL_newmetatable(L,mod_listbox_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_listbox_funcs,0);
	Util_putTableFunc(L,mod_listbox_funcs);
}
////////////////////////////////////////////////////////暴露给lua的函数//////////////////////////////////////////////

//设置l列表框位置
int mod_listbox_setposition(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int x=(int)lua_tonumber(L,2);
	int y=(int)lua_tonumber(L,3);
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	RECT rc;
	GetWindowRect(me->hme,&rc);
	int width=rc.right-rc.left;
	int height=rc.bottom-rc.top;
	MoveWindow(me->hme,x,y,width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//改变列表框的大小
int mod_listbox_setwidthheight(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int width=(int)lua_tonumber(L,2);
	int height=(int)lua_tonumber(L,3);
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	RECT rc;
	GetWindowRect(me->hme,&rc);
	MoveWindow(me->hme,ScreenToClient(me->hme,rc.left),ScreenToClient(me->hme,rc.top),width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_listbox_addnew(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	char* text=lua_tostring(L,2);
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	SendMessage(me->hme,LB_ADDSTRING,0,(LPARAM)text);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_listbox_getbyind(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushstring(L,"");
		return 1;
	}
	int text=(int)lua_tonumber(L,2);
	char buff[500];
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	if(SendMessage(me->hme,LB_GETTEXT,(WPARAM)text,(LPARAM)buff)==LB_ERR){
		lua_pushstring(L,"");
		return 1;
	}
	lua_pushstring(L,buff);
	return 1;
}
int exp_listbox_gettotalnum(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=1){
		lua_pushnumber(L,0);
		return 1;
	}
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	int num=SendMessage(me->hme,LB_GETCOUNT,0,NULL);
	lua_pushnumber(L,num);
	return 1;
}
int exp_listbox_getselind(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=1){
		lua_pushnumber(L,-1);
		return 1;
	}
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	int num=SendMessage(me->hme,LB_GETCURSEL,0,0);
	lua_pushnumber(L,num);
	return 1;
}
int exp_listbox_deleteind(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int ind=(int)lua_tonumber(L,2);
	mod_listbox_Struct* me=(mod_listbox_Struct*)p->ptr;
	SendMessage(me->hme,LB_DELETESTRING,ind,0);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
