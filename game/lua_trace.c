// lua library for trace_t

#include "g_lua.h"

#ifdef G_LUA
static int Trace_GC(lua_State * L)
{

	return 0;
}

static int Trace_ToString(lua_State * L)
{
	ltrace_t     *ltrace;
	trace_t      *trace;
	char         buf[MAX_STRING_CHARS];

	ltrace = Lua_GetTrace(L, 1);
	trace = ltrace->tr;
	Com_sprintf(buf, sizeof(buf), "trace: entity=%i fraction=%f allsolid=%i contents=%i endpos=\"%s\" startsolid=%i surfaceFlags=%i pointer=%p\n", 
											trace->entityNum,
											trace->fraction,
											trace->allsolid,
											trace->contents,
											vtos(trace->endpos),
											trace->startsolid,
											trace->surfaceFlags,
											trace);
	lua_pushstring(L, buf);

	return 1;
}

static const luaL_Reg lib_trace[] = {
	//{"DoTrace", Trace_DoTrace},
	{NULL, NULL}
};

static const luaL_Reg Trace_meta[] = {
	{"__gc", Trace_GC},
	{"__tostring", Trace_ToString},
	//{"GetAllsolid", Trace_GetAllsolid},
	//{"GetStartsolid", Trace_GetStartsolid},
	//{"GetFraction", Trace_GetFraction},
	//{"GetEndpos", Trace_GetEndpos},
	//{"GetSurfaceFlags", Trace_GetSurfaceFlags},
	//{"GetContents", Trace_GetContents},
	//{"GetEntityNum", Trace_GetEntityNum},
	{NULL, NULL}
};

int Luaopen_Trace(lua_State *L) {
	luaL_newmetatable(L, "game.trace");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, Trace_meta);
	luaL_register(L, "trace", lib_trace);

	return 1;
}

void Lua_PushTrace(lua_State * L, trace_t * tr)
{
	ltrace_t     *trace;

	trace = (ltrace_t *)lua_newuserdata(L, sizeof(ltrace_t));

	luaL_getmetatable(L, "game.trace");
	lua_setmetatable(L, -2);

	trace->tr = tr;
}

ltrace_t *Lua_GetTrace(lua_State * L, int argNum)
{
	void           *ud;

	ud = luaL_checkudata(L, argNum, "game.trace");
	luaL_argcheck(L, ud != NULL, argNum, "\'trace\' expected");
	return (ltrace_t *) ud;
}
#endif
