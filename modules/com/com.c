#include "com.h"
#include "stdlib.h"
#include <string.h>

int exp_com_writestr(lua_State* L);
int exp_com_isvalid(lua_State* L);

static const char* mod_com_name="comfile";
static luaL_Reg mod_com_funcs[]={
	{"writestr",exp_com_writestr},
	{"isvalid",exp_com_isvalid},
	{NULL,NULL}
};

typedef struct mod_com_Struct
{
	HANDLE hCom;
} mod_com_Struct;
//////////////////////////////////////////////////////////////操作类函数/////////////////////////////////////////////////
const char* mod_com_getname(void){
	return mod_com_name;
}
void mod_com_new(Phony* p,char* filename,int baud){
	mod_com_Struct *temp;
	HANDLE hCom;
	p->ID=-2;
	p->name=mod_com_name;
	temp=(mod_com_Struct *)malloc(sizeof(mod_com_Struct));
	p->ptr=temp;
	hCom=CreateFile(filename,
		GENERIC_READ|GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if(hCom==(HANDLE)-1)
	{
		temp->hCom=hCom;
		return;
	}
	SetupComm(hCom,1,1); //输入缓冲区和输出缓冲区的大小都是 1
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(hCom,&TimeOuts); //设置超时
	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate=baud; //波特率为 4800
	dcb.ByteSize=8; //每个字节有8位
	dcb.Parity=NOPARITY; //无奇偶校验位
	dcb.StopBits=TWOSTOPBITS; //两个停止位
	SetCommState(hCom,&dcb);
	temp->hCom=hCom;
}
void mod_com_luareg(lua_State* L){
	luaL_newmetatable(L,mod_com_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_com_funcs,0);
	Util_putTableFunc(L,mod_com_funcs);
}
////////////////////////////////////////////////////////暴露给lua的函数//////////////////////////////////////////////
//向串口写入字符串数据
int exp_com_writestr(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwBytesWrite=0;
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	char* str=lua_tostring(L,2);
	mod_com_Struct* own=(mod_com_Struct*)p->ptr;
	dwBytesWrite=strlen(str);
	if(own->hCom==(HANDLE)-1)
	{
		lua_pushnumber(L,2);
		lua_pushstring(L,"invalid handle");
		return 2;
	}
	ClearCommError(own->hCom,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(own->hCom,str,dwBytesWrite,&dwBytesWrite,NULL);
	if (bWriteStat==TRUE)
	{
		lua_pushnumber(L,0);
		lua_pushstring(L,"ok");
	}else{
		lua_pushnumber(L,3);
		lua_pushstring(L,"writefile failed");
	}
	return 2;
}
//串口句柄是否有效
int exp_com_isvalid(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_com_Struct* own=(mod_com_Struct*)p->ptr;
	if(own->hCom==(HANDLE)-1)
	{
		lua_pushnumber(L,2);
		lua_pushstring(L,"invalid handle");
		return 2;
	}else{

		lua_pushnumber(L,0);
		lua_pushstring(L,"yes");
		return 2;
	}
}
