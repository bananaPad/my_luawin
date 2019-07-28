#ifndef __INC_CORE__
#define __INC_CORE__

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

//输出结构体定义
typedef enum {
	NUMBER=1,
	FUNC,
	STRING
} ValueType;

typedef struct TableDef{
	ValueType type;
	char* name;
	char* str;
	lua_CFunction func;
	double number;
} TableDef;
typedef struct Phony
{
	int ID;
	const char* name;
	void* ptr;
} Phony;
typedef struct Callbacks
{
	char name[10] ;
	int callback;
	struct  Callbacks* next;
}Callbacks;
typedef struct IDs
{
	int ID;
	Callbacks* firstc;
	Callbacks* lastc;
	struct IDs* next;
}IDs;
//输出函数定义
void Util_ids_init(void);
int Util_ids_newID(void);
void Util_ids_setcallback(int id,char* name,int callback);
int Util_ids_getcallback(int id,char* name);

int Util_strcmp(char* s1,char* s2,int len);
void Util_strcopy(char* from,char* to ,int len);

int Util_putTable(lua_State* L,TableDef* t);
int Util_putTableFunc(lua_State* L,luaL_Reg* funcs);

#endif
