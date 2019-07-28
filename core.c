#include "core.h"
#include <stdio.h>
#include "stdlib.h"
#include <windows.h>
static IDs* Core_idsfirst;
static IDs* Core_idslast;
///////////////////////////////////////////////////////////回调函数及控件ID管理相关函数/////////////////////////////////////////////////////////
void Util_ids_init(void){
	Core_idsfirst=(IDs* )malloc(sizeof(IDs));
	Core_idsfirst->ID=-1;
	Core_idsfirst->firstc=(Callbacks*)malloc(sizeof(Callbacks));
	Core_idsfirst->firstc->next=NULL;
	Core_idsfirst->lastc=Core_idsfirst->firstc;
	Core_idsfirst->next=(IDs* )malloc(sizeof(IDs));
	Core_idslast=Core_idsfirst->next;
	Core_idslast->ID=119;
	Core_idslast->firstc=(Callbacks*)malloc(sizeof(Callbacks));
	Core_idslast->lastc=Core_idslast->firstc;
	Core_idslast->firstc->next=NULL;
	Core_idslast->next=NULL;
}
int Util_ids_newID(void){
	int newID=Core_idslast->ID+1;
	IDs* temp=(IDs* )malloc(sizeof(IDs));
	temp->ID=newID;
	temp->firstc=(Callbacks*)malloc(sizeof(Callbacks));
	temp->lastc=temp->firstc;
	temp->firstc->next=NULL;
	temp->next=NULL;
	Core_idslast->next=temp;
	Core_idslast=temp;
	return newID;
}
void Util_ids_setcallback(int id,char* name,int callback){
	IDs* tempid=Core_idsfirst;
	Callbacks* tempcal;
	Callbacks* newcal;
	//寻找ID
	while(1){
		if(tempid->ID==id){
			break;
		}
		tempid=tempid->next;
		if(tempid==NULL)return;
	}
	//寻找name
	tempcal=tempid->firstc;
	while(1){
		if(Util_strcmp(name,tempcal->name,10)==0){
			//找到了
			tempcal->callback=callback;
			return;
		}
		if (tempcal->next==NULL)
		{
			break;
		}
		tempcal=tempcal->next;
	}
	//没有找到就要新建
	newcal=(Callbacks*)malloc(sizeof(Callbacks));
	Util_strcopy(name,newcal->name,10);
	newcal->callback=callback;
	newcal->next=NULL;
	tempcal->next=newcal;
	tempid->lastc=newcal;
}
int Util_ids_getcallback(int id,char* name){
	IDs* tempid=Core_idsfirst;
	Callbacks* tempcal;
	Callbacks* newcal;
	//寻找ID
	while(1){
		if(tempid->ID==id){
			break;
		}
		tempid=tempid->next;
		//没有找到
		if(tempid==NULL)return -1;
	}
	//寻找name
	tempcal=tempid->firstc;
	while(1){
		if(Util_strcmp(name,tempcal->name,10)==0){
			//找到了
			return tempcal->callback;
		}
		if (tempcal->next==NULL)
		{
			return -1;
		}
		tempcal=tempcal->next;
	}
}
/////////////////////////////////////////////////////////////字符串处理相关函数/////////////////////////////////////////////////////////
void Util_strcopy(char* from,char* to ,int len){
	int i=0;
	for (i = 0; i < len; ++i)
	{
		to[i]=from[i];
		if(from[i]=='\0')break;
	}
}
int Util_strcmp(char* s1,char* s2,int len){
	int i=0;
	for (i = 0; i < len; ++i)
	{
		if(s1[i]!=s2[i])return 1;
		if(s1[i]=='\0')break;
	}
	return 0;
}

///////////////////////////////////////////////////////////lua相关函数////////////////////////////////////////////////////////////////////////
int Util_putTableFunc(lua_State* L,luaL_Reg* funcs){
	luaL_Reg temp;
	int i=0;

	if (!lua_istable(L,-1))
	{
		return 1;
	}
	while(1){
		temp=funcs[i];
		if(!temp.name)break;
		lua_pushstring(L,temp.name);
		lua_pushcfunction(L,temp.func);
		lua_settable(L,-3);
		
		i++;
	}
	return 0;
}
int Util_putTable(lua_State* L,TableDef* t){
	TableDef temp;
	int i=0;
	if (!lua_istable(L,-1))
	{
		return 1;
	}
	while(1){
		temp=t[i];
		if(!temp.name)break;
		lua_pushstring(L,temp.name);
		switch(temp.type){
			case NUMBER:
				lua_pushnumber(L,temp.number);
				break;
			case FUNC:
				lua_pushcfunction(L,temp.func);
				break;
			case STRING:
				lua_pushstring(L,temp.str);
				break;
			default:
				lua_pushnumber(L,0);
				break;
		}
		lua_settable(L,-3);
		i++;
	}
	return 0;
}
