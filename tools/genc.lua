Template={}
Template["expItem"]=[[{"$funcname",exp_$modname_$funcname},]]
Template["expose"]=[[
static luaL_Reg mod_$modname_funcs[]={
$strings	{NULL,NULL}
};
void mod_$modname_luareg(lua_State* L){
	luaL_newmetatable(L,mod_$modname_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index");
	luaL_setfuncs(L,mod_$modname_funcs,0);
	Util_putTableFunc(L,mod_$modname_funcs);
}
]]

Template["voidfunc"]=[[
int exp_$modname_$funcname(lua_State* L){
	Phony* p=lua_touserdata(L,1);
	int n=lua_gettop(L);
	if(p==NULL || n!=$n){
		lua_pushnumber(L,1);
		lua_pushstring(L,"arg error");
		return 2;
	}
	mod_$modname_Struct* own=(mod_$modname_Struct*)p->ptr;
$argdef
	$userfunc(own->$struc $args ) ;
	lua_pushnumber(L,0);
	lua_pushstring(L,"ok");
	return 2;
}
]]
------------------------------------------------------------------------------------------------------------------------------------------------------

allfuncstrings={}
allfuncnames={}

-------------------------------------------------------------------------------------------------------------------------------------------------------
function getexpose(modname, t )
	local s = ""
	local out = ""
	for v,i in ipairs(t) do
		s=s.."\t"..Template["expItem"]:gsub("%$(%w+)",function (n) if n=="modname" then return modname end  if n=="funcname" then return i end  end).."\n"
	end
	out=Template["expose"]:gsub("%$(%w+)",function (n) if n=="modname" then return modname end  if n=="strings" then return s end  end)
	print(out)
end
function genargdef(s)
	local k = ""
	local nn=0
	local args=""
	local argdef = ""
	for k in string.gmatch(s, "(%w+)") do
		nn=nn+1
		args=args..",arg"..nn
		if k=="int" then
			argdef=argdef.."\t".."int arg"..nn.."=(int)lua_tonumber(L,"..(nn+1)..");\n"
		elseif k=="double" then
			argdef=argdef.."\t".."double arg"..nn.."=lua_tonumber(L,"..(nn+1)..");\n"
		elseif k=="char*" then
			argdef=argdef.."\t".."char* arg"..nn.."=lua_tostring(L,"..(nn+1)..");\n"
		else
			argdef=argdef.."\t".."Phony* arg"..nn.."=(Phony*)lua_touserdata(L,"..(nn+1)..");\n"
		end
	end
	return nn,args,argdef
end
function genvoidfunc(t)
	local nn,args,argdef =genargdef(t["args"])

	local temps = {modname=modname,funcname=t["funcname"],n=nn+1,argdef=argdef,args=args,userfunc=t["userfunc"],struc=t["struc"]}
	local out = ""
	out=Template["voidfunc"]:gsub("%$(%w+)",temps)
	table.insert(allfuncnames,t["funcname"])
	table.insert(allfuncstrings,out)
end

input=assert(arg[1],"need an input")
dofile(input)
for i,v in ipairs(allfuncstrings) do
	print(v)
end
getexpose(modname,allfuncnames)
