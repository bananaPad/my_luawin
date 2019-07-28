#include "window.h"
#include "stdlib.h"
#include "../button/button.h"
#include "../textbox/textbox.h"
#include "../cairo/cairo.h"
#include "../cairo/cairocnt.h"
#include "../listbox/listbox.h"
#include "../com/com.h"

int mod_window_myprint(lua_State* L);

int exp_window_msgbox(lua_State* L);
int exp_window_settitle(lua_State* L);
int exp_window_setposition(lua_State* L);
int exp_window_setwidthheight(lua_State* L);
int exp_window_setexit(lua_State* L);
int exp_window_newbutton(lua_State* L);
int exp_window_newtextbox(lua_State* L);
int exp_window_setredraw(lua_State* L);
int exp_window_newlistbox(lua_State* L);
int exp_window_choosecolor(lua_State* L);
int exp_window_choosefile(lua_State* L);
int exp_window_setkeydown(lua_State* L);
int exp_window_setkeyup(lua_State* L);
int exp_window_setmousemove(lua_State* L);
int exp_window_setlbup(lua_State* L);
int exp_window_setlbdown(lua_State* L);
int exp_window_redraw(lua_State* L);
int exp_window_opencom(lua_State* L);

//全局变量定义
static const char* mod_window_name ="window";
static HWND mod_window_hwnd=0;
static HINSTANCE mod_window_hInstance=0;
static luaL_Reg mod_window_funcs[]={
	{"msgbox",exp_window_msgbox},
	{"settitle",exp_window_settitle},
	{"setposition",exp_window_setposition},
	{"setwidthheight",exp_window_setwidthheight},
	{"setexit",exp_window_setexit},
	{"newbutton",exp_window_newbutton},
	{"newtextbox",exp_window_newtextbox},
	{"setredraw",exp_window_setredraw},
	{"newlistbox",exp_window_newlistbox},
	{"choosecolor",exp_window_choosecolor},
	{"choosefile",exp_window_choosefile},
	{"setkeydown",exp_window_setkeydown},
	{"setkeyup",exp_window_setkeyup},
	{"setlbup",exp_window_setlbup},
	{"setlbdown",exp_window_setlbdown},
	{"setmousemove",exp_window_setmousemove},
	{"redraw",exp_window_redraw},
	{"opencom",exp_window_opencom},
	{NULL,NULL}
};
///////////////////////////////////////////////////操作类函数//////////////////////////////////////////////////////////////////////////////////////////////
void mod_window_new(Phony* p){
	p->ID=-1;
	p->name=mod_window_name;
	p->ptr=&mod_window_hwnd;
}
//将此模块暴露到lua中名为name的变量中，对lua栈无影响
void mod_window_expose(lua_State* L,char* name){
	Phony* p= (Phony*)lua_newuserdata(L,sizeof(Phony));

	mod_window_new(p);

	mod_window_luareg(L);

	lua_setmetatable(L,-2);
	lua_setglobal(L,name);
	lua_pushcfunction(L,mod_window_myprint);
	lua_setglobal(L,"print");
	//注册button模块
	mod_button_luareg(L);
	lua_pop(L,1);
	//注册textbox模块
	mod_textbox_luareg(L);
	lua_pop(L,1);
	//注册cairo模块
	mod_cairocnt_luareg(L);
	lua_pop(L,1);
	mod_cairo_expose(L,"cairo");
	//注册listbox模块
	mod_listbox_luareg(L);
	lua_pop(L,1);
}
const char* mod_window_getname(void){
	return mod_window_name;
}
void mod_window_sethwnd(HWND hwnd){
	mod_window_hwnd=hwnd;
}
void mod_window_setins(HINSTANCE hInstance){
	mod_window_hInstance=hInstance;
}
//向lua注册当前模块，只注册元表。将metatable放入最顶栈。
void mod_window_luareg(lua_State* L){
	luaL_newmetatable(L,mod_window_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_window_funcs,0);
	Util_putTableFunc(L,mod_window_funcs);
	//lua_pop(L,1);
}
int mod_window_myprint(lua_State* L){
	int n=lua_gettop(L),i=0;
	char** allparam=(char**)malloc(n*sizeof(char*));
	for(i=1; i <= n; i++)
	{
		allparam[i-1]=lua_tostring(L,i);
	}
	MessageBox(mod_window_hwnd,allparam[0],"print",MB_OK);
	return 0;
}
int mod_window_setcallbacks(lua_State* L,char* name){
	int n=lua_gettop(L);
	if(lua_isfunction(L,1) && n==1){
		int callback=luaL_ref(L,LUA_REGISTRYINDEX);
		Util_ids_setcallback(-1,name,callback);	
		lua_pop(L,n);
		lua_pushnumber(L,0);
		lua_pushstring(L,"ok");
		return 2;
	}else{
		lua_pop(L,n);
		lua_pushnumber(L,1);
		lua_pushstring(L,"not a callback");
		return 2;
	}
}
///////////////////////////////////////////////暴露到lua中的函数////////////////////////////////////////////////////////////////////////////////////////
int exp_window_msgbox(lua_State* L){
	if (lua_gettop(L)!=2)
	{
		return 0;
	}
	char* msg=lua_tostring(L,1);
	char* title=lua_tostring(L,2);
	MessageBox(mod_window_hwnd, msg,title, MB_OK);
	return 0;
}
int exp_window_settitle(lua_State* L){
	if (lua_gettop(L)!=1)
	{
		return 0;
	}
	char* title=lua_tostring(L,1);
	SendMessage(mod_window_hwnd,WM_SETTEXT,0,(LPARAM)title);
	return 0;
}
int exp_window_setposition(lua_State* L){
	if (lua_gettop(L)!=2)
	{
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int x=(int)lua_tonumber(L,1);
	int y=(int)lua_tonumber(L,2);
	RECT rc;
	GetWindowRect(mod_window_hwnd,&rc);
	int width=rc.right-rc.left;
	int height=rc.bottom-rc.top;
	MoveWindow(mod_window_hwnd,x,y,width,height,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_window_setwidthheight(lua_State* L){
	if (lua_gettop(L)!=2)
	{
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	int w=(int)lua_tonumber(L,1);
	int h=(int)lua_tonumber(L,2);
	RECT rc;
	GetWindowRect(mod_window_hwnd,&rc);
	MoveWindow(mod_window_hwnd,rc.left,rc.top,w,h,TRUE);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;

}
int exp_window_setexit(lua_State* L){
	return mod_window_setcallbacks(L,"exit");
}
//新建一个按钮
int exp_window_newbutton(lua_State* L){
	int n=lua_gettop(L);
	if(n!=5)return 0;
	char* caption=lua_tostring(L,1);
	int x=(int)lua_tonumber(L,2);
	int y=(int)lua_tonumber(L,3);
	int w=(int)lua_tonumber(L,4);
	int h=(int)lua_tonumber(L,5);
	lua_pop(L,n);
	Phony* p= (Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_button_new(p,mod_window_hwnd,mod_window_hInstance,caption,x,y,w,h);
	luaL_newmetatable(L,p->name);
	lua_setmetatable(L,-2);
	return 1;
}
//新建一个文本框
int exp_window_newtextbox(lua_State* L){
	int n=lua_gettop(L);
	if(n!=4)return 0;
	int x=(int)lua_tonumber(L,1);
	int y=(int)lua_tonumber(L,2);
	int w=(int)lua_tonumber(L,3);
	int h=(int)lua_tonumber(L,4);
	lua_pop(L,n);
	Phony* p= (Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_textbox_new(p,mod_window_hwnd,mod_window_hInstance,x,y,w,h);
	luaL_newmetatable(L,p->name);
	lua_setmetatable(L,-2);
	return 1;
}
//新建一个列表框
int exp_window_newlistbox(lua_State* L){
	int n=lua_gettop(L);
	if(n!=4)return 0;
	int x=(int)lua_tonumber(L,1);
	int y=(int)lua_tonumber(L,2);
	int w=(int)lua_tonumber(L,3);
	int h=(int)lua_tonumber(L,4);
	lua_pop(L,n);
	Phony* p= (Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_listbox_new(p,mod_window_hwnd,mod_window_hInstance,x,y,w,h);
	luaL_newmetatable(L,p->name);
	lua_setmetatable(L,-2);
	return 1;
}
//打开串口
int exp_window_opencom(lua_State* L){
	int n=lua_gettop(L);
	if(n!=2)return 0;
	int fname=(int)lua_tostring(L,1);
	int baud=(int)lua_tonumber(L,2);
	lua_pop(L,n);
	Phony* p= (Phony*)lua_newuserdata(L,sizeof(Phony));
	mod_com_new(p,fname,baud);
	luaL_newmetatable(L,p->name);
	lua_setmetatable(L,-2);
	return 1;
}
//强行刷新
int exp_window_redraw(lua_State* L){
	InvalidateRect(mod_window_hwnd,NULL,TRUE);
	return 0;
}

//设置重绘函数
int exp_window_setredraw(lua_State* L){
	return mod_window_setcallbacks(L,"redraw");
}
//设置键盘函数
int exp_window_setkeydown(lua_State* L){
	return mod_window_setcallbacks(L,"keydown");
}
int exp_window_setkeyup(lua_State* L){
	return mod_window_setcallbacks(L,"keyup");
}
//设置鼠标响应函数
int exp_window_setlbdown(lua_State* L){
	return mod_window_setcallbacks(L,"lbdown");
}
int exp_window_setlbup(lua_State* L){
	return mod_window_setcallbacks(L,"lbup");
}
int exp_window_setmousemove(lua_State* L){
	return mod_window_setcallbacks(L,"mousemove");
}
//颜色选择对话框
int exp_window_choosecolor(lua_State* L){
	CHOOSECOLOR cc;
	COLORREF acr[16];
	unsigned char r,g,b;
	ZeroMemory(&cc,sizeof(cc));
	cc.lStructSize=sizeof(cc);
	cc.hwndOwner=mod_window_hwnd;
	cc.lpCustColors=(LPDWORD)acr;
	cc.Flags=CC_FULLOPEN ;
	if (ChooseColor(&cc)==TRUE)
	{
		r=(unsigned char)(cc.rgbResult & 0xff);
		g=(unsigned char)((cc.rgbResult>>8)&0xff);
		b=(unsigned char)((cc.rgbResult>>16)&0xff);
		lua_pushnumber(L,r);
		lua_pushnumber(L,g);
		lua_pushnumber(L,b);
		return 3;	
	}
	lua_pushnumber(L,0);
	lua_pushnumber(L,0);
	lua_pushnumber(L,0);
	return 3;	
}
//文件选择对话框
int exp_window_choosefile(lua_State* L){
	OPENFILENAME ofn;
	char* szFile=(char*)malloc(260);
	//szFile[260]="\0";
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=mod_window_hwnd;
	ofn.lpstrFile=szFile;
	ofn.nMaxFile=260;
	ofn.lpstrFilter="All\0*.*\0Text\0*.TXT\0\0";
	ofn.nFilterIndex=0;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;
	ofn.Flags=OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	if (GetOpenFileName(&ofn)==TRUE)
	{
		lua_pushstring(L,ofn.lpstrFile);
		free(szFile);
		return 1;
	}
	free(szFile);
	lua_pushstring(L,"");
	return 1;
}
