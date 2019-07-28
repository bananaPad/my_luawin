#include "cairocnt.h"
#include "stdlib.h"

static const char* mod_cairocnt_name="cairocnt";

typedef struct mod_cairocnt_Struct
{
	HDC hdc;
	cairo_surface_t* surface;
	cairo_t* cnt;
} mod_cairocnt_Struct;
//////////////////////////////////////////////////////////////操作类函数/////////////////////////////////////////////////
const char* mod_cairocnt_getname(void){
	return mod_cairocnt_name;
}
void mod_cairocnt_new(Phony* p,HDC hdc){
	mod_cairocnt_Struct *temp;
	p->ID=-2;
	p->name=mod_cairocnt_name;
	temp=(mod_cairocnt_Struct *)malloc(sizeof(mod_cairocnt_Struct));
	p->ptr=temp;
	temp->hdc=hdc;
	temp->surface=cairo_win32_surface_create(hdc);
	temp->cnt=cairo_create ( temp->surface);
}
void mod_cairocnt_newpdf(Phony* p,char* fname,int w,int h){
	mod_cairocnt_Struct *temp;
	p->ID=-2;
	p->name=mod_cairocnt_name;
	temp=(mod_cairocnt_Struct *)malloc(sizeof(mod_cairocnt_Struct));
	p->ptr=temp;
	temp->hdc=-1;
	temp->surface=cairo_pdf_surface_create ( fname,w,h);
	temp->cnt=cairo_create ( temp->surface);
}
void mod_cairocnt_newsvg(Phony* p,char* fname,int w,int h){
	mod_cairocnt_Struct *temp;
	p->ID=-2;
	p->name=mod_cairocnt_name;
	temp=(mod_cairocnt_Struct *)malloc(sizeof(mod_cairocnt_Struct));
	p->ptr=temp;
	temp->hdc=-1;
	temp->surface=cairo_svg_surface_create ( fname,w,h);
	temp->cnt=cairo_create ( temp->surface);
}

void mod_cairocnt_free(Phony* p){
	mod_cairocnt_Struct* temp=(mod_cairocnt_Struct*)p->ptr;
	cairo_destroy (temp->cnt) ;
	cairo_surface_destroy (temp->surface);
	free(temp);
	//free(p);

}
////////////////////////////////////////////////////////暴露给lua的函数//////////////////////////////////////////////
//改变颜色
int exp_cairocnt_setrgb(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=4){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double r=lua_tonumber(L,2);
	double g=lua_tonumber(L,3);
	double b=lua_tonumber(L,4);	
	cairo_set_source_rgb ( own->cnt, r,g,b) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_cairocnt_moveto(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double x=lua_tonumber(L,2);
	double y=lua_tonumber(L,3);
	cairo_move_to(own->cnt,x,y);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
//写文字
int exp_cairocnt_showtext(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	char* s=lua_tostring(L,2);
	cairo_show_text ( own->cnt,s );
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_cairocnt_setfontsize(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double ts=lua_tonumber(L,2);
	cairo_set_font_size (own->cnt, ts) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_cairocnt_setfont(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	char* s=lua_tostring(L,2);
	cairo_select_font_face ( own->cnt, s ,
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_cairocnt_savetopng(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	char* fname=lua_tostring(L,2);
	cairo_surface_write_to_png(own->surface,fname);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;

}
int exp_cairocnt_showpage(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=1){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	cairo_show_page(own->cnt);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
int exp_cairocnt_stroke(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=1){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	cairo_stroke(own->cnt);
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
////////////////////////////////////////////////////////////////////自动生成的函数////////////////////////////
int exp_cairocnt_lineto(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double arg1=lua_tonumber(L,2);
	double arg2=lua_tonumber(L,3);

	cairo_line_to(own->cnt ,arg1,arg2 ) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}

int exp_cairocnt_relineto(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double arg1=lua_tonumber(L,2);
	double arg2=lua_tonumber(L,3);

	cairo_rel_line_to(own->cnt ,arg1,arg2 ) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}

int exp_cairocnt_relmoveto(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=3){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double arg1=lua_tonumber(L,2);
	double arg2=lua_tonumber(L,3);

	cairo_rel_move_to(own->cnt ,arg1,arg2 ) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}

int exp_cairocnt_linewidth(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=2){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_cairocnt_Struct* own=(mod_cairocnt_Struct*)p->ptr;
	double arg1=lua_tonumber(L,2);

	cairo_set_line_width(own->cnt ,arg1 ) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}

///////////////////////////////////////////////注册函数//////////////////////////////////////////////////////////////////////
static luaL_Reg mod_cairocnt_funcs[]={
	{"setrgb",exp_cairocnt_setrgb},
	{"moveto",exp_cairocnt_moveto},
	{"showtext",exp_cairocnt_showtext},
	{"setfont",exp_cairocnt_setfont},
	{"setfontsize",exp_cairocnt_setfontsize},
	{"savetopng",exp_cairocnt_savetopng},
	{"showpage",exp_cairocnt_showpage},
	{"stroke",exp_cairocnt_stroke},
	////////////////////////////////////////自动生成/////////////////////////////
	{"lineto",exp_cairocnt_lineto},
	{"relineto",exp_cairocnt_relineto},
	{"relmoveto",exp_cairocnt_relmoveto},
	{"linewidth",exp_cairocnt_linewidth},
	{NULL,NULL}
};

void mod_cairocnt_luareg(lua_State* L){
	luaL_newmetatable(L,mod_cairocnt_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_cairocnt_funcs,0);
	Util_putTableFunc(L,mod_cairocnt_funcs);
}
