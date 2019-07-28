#include <windows.h>
#include "modules/window/window.h"
#include "modules/cairo/cairocnt.h"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
lua_State* L;
int IsRedraw=0;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szClassName[] = "MainWClass";	
	WNDCLASSEX wndclass;
	L=luaL_newstate();
	
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;	
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.cbClsExtra = 0;	
	wndclass.cbWndExtra = 0;	
	wndclass.hInstance = hInstance;		
	wndclass.hIcon = LoadIcon(NULL, 
			IDI_APPLICATION);	
	wndclass.hCursor = LoadCursor(NULL,
			IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)
		GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;	
	wndclass.lpszClassName = szClassName ;	
	wndclass.hIconSm = NULL;	
	
	RegisterClassEx(&wndclass); 
	
	HWND hwnd = CreateWindowEx( 
		0,				
		szClassName,		
		"winlua",	
		WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT,			
		CW_USEDEFAULT,	
		CW_USEDEFAULT,			
		CW_USEDEFAULT,	
		NULL,				
		NULL,		
		hInstance,				
		NULL) ;				

	if(hwnd == NULL)
	{
		MessageBox(NULL, "createwindow error!","error", MB_OK);
		return -1;
	}
	luaL_openlibs(L);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	////////////////////////////////////////准备lua//////////////////////////////////////////////////////////////////////////////////////////////////////
	Util_ids_init();
	mod_window_sethwnd(hwnd);
	mod_window_setins(hInstance);
	mod_window_expose(L,"window");
	IsRedraw=1;

	luaL_loadfile(L,"./main.lua");
	int i=lua_pcall(L, 0, LUA_MULTRET, 0);
	if(i!=0){
		MessageBox(NULL, luaL_checkstring(L, -1),"error", MB_OK);
		return -1;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MSG msg;	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szText[] = "hello winlua";
	int callback;
	char s[10];
	switch (message)
	{	 	
	case WM_PAINT: 
		{

			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint (hwnd, &ps) ;
			if(IsRedraw==0)return 0;
			callback=Util_ids_getcallback(-1,"redraw");
			if(callback!=-1){
				lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
				Phony* p=(Phony*)lua_newuserdata(L,sizeof(Phony));
				mod_cairocnt_new(p,hdc);
				mod_cairocnt_luareg(L);
				lua_setmetatable(L,-2);
				int ii=lua_pcall(L,1,0, 0);
				if(ii!=0){
					MessageBox(hwnd,lua_tostring(L,-1),"error",MB_OK);
				}
				//回收内存
				mod_cairocnt_free(p);

			}
			EndPaint(hwnd, &ps);
			return 0;
		}	
	case WM_COMMAND:
	{
		int id=(int)LOWORD(wParam);
		callback=Util_ids_getcallback(id,"click");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_pcall(L,0,0, 0);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		int callback=Util_ids_getcallback(-1,"keyup");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_pushnumber(L,(int)wParam);
			lua_pcall(L,1,0, 0);
		}
		return 0;
	}
	case WM_KEYDOWN:
	{
		int callback=Util_ids_getcallback(-1,"keydown");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_pushnumber(L,(int)wParam);
			lua_pcall(L,1,0, 0);
		}
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int callback=Util_ids_getcallback(-1,"mousemove");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_newtable(L);
			lua_pushstring(L,"x");
			lua_pushnumber(L,(int)LOWORD(lParam));
			lua_settable(L,-3);
			lua_pushstring(L,"y");
			lua_pushnumber(L,(int)HIWORD(lParam));
			lua_settable(L,-3);
			lua_pcall(L,1,0, 0);
		}
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int callback=Util_ids_getcallback(-1,"lbdown");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_newtable(L);
			lua_pushstring(L,"x");
			lua_pushnumber(L,(int)LOWORD(lParam));
			lua_settable(L,-3);
			lua_pushstring(L,"y");
			lua_pushnumber(L,(int)HIWORD(lParam));
			lua_settable(L,-3);
			lua_pcall(L,1,0, 0);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int callback=Util_ids_getcallback(-1,"lbup");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_newtable(L);
			lua_pushstring(L,"x");
			lua_pushnumber(L,(int)LOWORD(lParam));
			lua_settable(L,-3);
			lua_pushstring(L,"y");
			lua_pushnumber(L,(int)HIWORD(lParam));
			lua_settable(L,-3);
			lua_pcall(L,1,0, 0);
		}
		return 0;
	}
	case WM_DESTROY: 
		callback=Util_ids_getcallback(-1,"exit");
		if(callback!=-1){
			lua_rawgeti(L,LUA_REGISTRYINDEX,callback);
			lua_pcall(L,0,0, 0);
		}

		PostQuitMessage(0) ;
		return 0 ;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
