// lua library for weapons

#include "g_lua.h"

#ifdef G_LUA
static int Weapons_DoTrace(lua_State *L) {
	/* 
	things that should be returned/set by this:
	*	tr.entityNum
	*	tr.fraction
	*/ 
	/*trace_t tr;
	vec_t *start, *mins, *maxs, *end;
	int passEntNum, contentMask;

	start = Lua_GetVector(L, 1);
	mins = Lua_GetVector(L, 2);
	maxs = Lua_GetVector(L, 3);
	end = Lua_GetVector(L, 4);
	passEntNum = luaL_checknumber(L, 5);

	trap_Trace(&tr, start, mins, maxs, end, passEntNum, contentMask);*/

	return 1;
}

static const luaL_Reg lib_weapons[] = {
	{"DoTrace", Weapons_DoTrace},
	{NULL, NULL}
};

int Luaopen_Weapons(lua_State *L) {
	luaL_register(L, "weapons", lib_weapons);

	return 1;
}
#endif
