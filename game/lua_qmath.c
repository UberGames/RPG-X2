// qmath lib for lua

#include "g_lua.h"

#ifdef G_LUA
// qmath.abs(float num)
// Returns the integer part of a floating point number
static int Qmath_Abs(lua_State * L)
{
	lua_pushnumber(L, fabs(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.sin(float degree)
// Sinus implementation, takes degree as input not radian
static int Qmath_Sin(lua_State * L)
{
	lua_pushnumber(L, sin(DEG2RAD(luaL_checknumber(L, 1))));
	return 1;
}

// qmath.cos(float degree)
// Cosinus implementation, takes degree as input not radian
static int Qmath_Cos(lua_State * L)
{
	lua_pushnumber(L, cos(DEG2RAD(luaL_checknumber(L, 1))));
	return 1;
}

// qmath.tan(float degree)
// Tan implementation, take degree as input not radian
static int Qmath_Tan(lua_State * L)
{
	lua_pushnumber(L, tan(DEG2RAD(luaL_checknumber(L, 1))));
	return 1;
}

// qmath.asin(float num)
// asin implementation
static int Qmath_Asin(lua_State * L)
{
	lua_pushnumber(L, RAD2DEG(asin(luaL_checknumber(L, 1))));
	return 1;
}

// qmath.acos(float num)
// acos implementation
static int Qmath_Acos(lua_State * L)
{
	lua_pushnumber(L, RAD2DEG(acos(luaL_checknumber(L, 1))));
	return 1;
}

// qmath.atan(float num)
// atan implementation
static int Qmath_Atan(lua_State * L)
{
	lua_pushnumber(L, RAD2DEG(atan(luaL_checknumber(L, 1))));
	return 1;
}

// qmath.atan2
static int Qmath_Atan2(lua_State * L)
{
	lua_pushnumber(L, RAD2DEG(atan2(luaL_checknumber(L, 1), luaL_checknumber(L, 2))));
	return 1;
}

// qmath.ceil(float number)
static int Qmath_Ceil(lua_State * L)
{
	lua_pushnumber(L, ceil(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.floor(float number)
static int Qmath_Floor(lua_State * L)
{
	lua_pushnumber(L, floor(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.fmod(float num, float num2)
static int Qmath_Fmod(lua_State * L)
{
	lua_pushnumber(L, fmod(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
	return 1;
}

// qmath.modf(float num)
static int Qmath_Modf(lua_State * L)
{
	double          ip;
	double          fp = modf(luaL_checknumber(L, 1), &ip);

	lua_pushnumber(L, ip);
	lua_pushnumber(L, fp);
	return 2;
}

// qmath.sqrt(float number)
static int Qmath_Sqrt(lua_State * L)
{
	lua_pushnumber(L, sqrt(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.pow(float num, float num2)
static int Qmath_Pow(lua_State * L)
{
	lua_pushnumber(L, pow(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
	return 1;
}

// qmath.log(float num)
// Returns logarithm of a number
static int Qmath_Log(lua_State * L)
{
	lua_pushnumber(L, log(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.log10(float num)
// Returns logarithm to base 10 of a number
static int Qmath_Log10(lua_State * L)
{
	lua_pushnumber(L, log10(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.exp(float number)
static int Qmath_Exp(lua_State * L)
{
	lua_pushnumber(L, exp(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.deg(float radian)
// Converts radian to degree
static int Qmath_Deg(lua_State * L)
{
	lua_pushnumber(L, RAD2DEG(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.rad(float dgree)
// Converts degree to radian
static int Qmath_Rad(lua_State * L)
{
	lua_pushnumber(L, DEG2RAD(luaL_checknumber(L, 1)));
	return 1;
}

// qmath.frexp(float number)
static int Qmath_Frexp(lua_State * L)
{
	int             e;

	lua_pushnumber(L, frexp(luaL_checknumber(L, 1), &e));
	lua_pushnumber(L, e);
	return 2;
}

// qmath.ldexp(float f, float n)
static int Qmath_Ldexp(lua_State * L)
{
	lua_pushnumber(L, ldexp(luaL_checknumber(L, 1), luaL_checkint(L, 2)));
	return 1;
}


// qmath.min(int array[])
// Returns the minimum of a list of values
static int Qmath_Min(lua_State * L)
{
	int             n = lua_gettop(L);	/* number of arguments */
	lua_Number      dmin = luaL_checknumber(L, 1);
	int             i;

	for(i = 2; i <= n; i++)
	{
		lua_Number      d = luaL_checknumber(L, i);

		if(d < dmin)
			dmin = d;
	}
	lua_pushnumber(L, dmin);
	return 1;
}

// qmath.max(int array[])
// Returns the maximum of a list of numbers
static int Qmath_Max(lua_State * L)
{
	int             n = lua_gettop(L);	/* number of arguments */
	lua_Number      dmax = luaL_checknumber(L, 1);
	int             i;

	for(i = 2; i <= n; i++)
	{
		lua_Number      d = luaL_checknumber(L, i);

		if(d > dmax)
			dmax = d;
	}
	lua_pushnumber(L, dmax);
	return 1;
}

// qmath.rand(void)
// Returns a random integer
static int Qmath_Rand(lua_State * L)
{
	lua_pushinteger(L, rand());
	return 1;
}

// qmath.random(void)
// Returns a random number
static int Qmath_Random(lua_State * L)
{
	lua_pushnumber(L, random());
	return 1;
}

// qmath.crandom(void)
static int Qmath_Crandom(lua_State * L)
{
	lua_pushnumber(L, crandom());
	return 1;
}

static const luaL_Reg lib_qmath[] = {
	{"abs", Qmath_Abs},
	{"sin", Qmath_Sin},
	{"cos", Qmath_Cos},
	{"tan", Qmath_Tan},
	{"asin", Qmath_Asin},
	{"acos", Qmath_Acos},
	{"atan", Qmath_Atan},
	{"atan2", Qmath_Atan2},
	{"ceil", Qmath_Ceil},
	{"floor", Qmath_Floor},
	{"fmod", Qmath_Fmod},
	{"modf", Qmath_Modf},
	{"frexp", Qmath_Frexp},
	{"ldexp", Qmath_Ldexp},
	{"sqrt", Qmath_Sqrt},
	{"min", Qmath_Min},
	{"max", Qmath_Max},
	{"log", Qmath_Log},
	{"log10", Qmath_Log10},
	{"exp", Qmath_Exp},
	{"deg", Qmath_Deg},
	{"pow", Qmath_Pow},
	{"rad", Qmath_Rad},
	{"rand", Qmath_Rand},
	{"random", Qmath_Random},
	{"crandom", Qmath_Crandom},
	{NULL, NULL}
};

int Luaopen_Qmath(lua_State * L)
{
	luaL_register(L, "qmath", lib_qmath);
	lua_pushnumber(L, M_PI);
	lua_setfield(L, -2, "pi");
	lua_pushnumber(L, HUGE_VAL);
	lua_setfield(L, -2, "huge");
	return 1;
}
#endif
