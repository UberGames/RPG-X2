#include "cg_lua.h"

#ifdef CG_LUA
#include "cg_lua.h"

#ifdef CG_LUA
static int Refent_GC(lua_State * L)
{

	return 0;
}

static int Refent_ToString(lua_State * L)
{
	rent_t		 *rent;
	refEntity_t  *ent;
	char            buf[MAX_STRING_CHARS];

	rent = Lua_GetRent(L, 1);
	ent = rent->r;
	Com_sprintf(buf, sizeof(buf), "centity: pointer=%p\n", ent);
	lua_pushstring(L, buf);

	return 1;
}

static const luaL_Reg Refentity_ctor[] = {
	{NULL,						NULL}
};

static const luaL_Reg Refentity_meta[] = {
	{"__gc",						Refent_GC},
	{"__tostring",					Refent_ToString},

	{NULL, NULL}
};

int Luaopen_Rent(lua_State * L)
{
	luaL_newmetatable(L, "cgame.refentity");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, Refentity_meta);
	luaL_register(L, "refentity", Refentity_ctor);

	return 1;
}

void Lua_PushRent(lua_State * L, refEntity_t * rent)
{
	rent_t     *refent;

	if(!rent)
		lua_pushnil(L);
	else {
		refent = (rent_t *)lua_newuserdata(L, sizeof(cent_t));
		luaL_getmetatable(L, "cgame.refentity");
		lua_setmetatable(L, -2);
		refent->r = rent;
	}
}

rent_t *Lua_GetRent(lua_State * L, int argNum)
{
	void           *ud;

	ud = luaL_checkudata(L, argNum, "cgame.refentity");
	luaL_argcheck(L, ud != NULL, argNum, "\'refentity\' expected");
	return (rent_t *) ud;
}
#endif

#endif
