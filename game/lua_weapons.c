// lua library for weapons

#include "g_lua.h"

#ifdef G_LUA

static const luaL_Reg lib_weapons[] = {
	{NULL, NULL}
};

int Luaopen_Weapons(lua_State *L) {
	luaL_register(L, "weapons", lib_weapons);

	return 1;
}
#endif
