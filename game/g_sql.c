
#ifdef SQL

#if (defined(_MSC_VER) | defined(__WIN32__))
#include "windows.h"
#endif

#include "sqlite3.h"
//#include "q_shared.h"
#include "g_sql.h"
#include "g_local.h"

extern vmCvar_t sql_dbName;
extern vmCvar_t sql_use;
extern vmCvar_t sql_server;
extern vmCvar_t sql_user;
extern vmCvar_t sql_password;
extern vmCvar_t sql_port;

extern void QDECL G_Printf( const char *fmt, ... );
extern void QDECL G_PrintfClient( gentity_t *ent, const char *fmt, ...);

sqlite3	*user_db;

/*
===============
G_SqlInit
===============
*/
qboolean G_Sql_Init(void) {
	int res;

	if(!sql_use.integer) return qtrue;

	res = sqlite3_open("db/users.db", &user_db);
	if(res) {
		G_Printf(S_COLOR_RED "SQL ERROR: Could not open user database\n");
		return qfalse;
	}

	return qtrue;
}

/*
===============
G_Sql_Shutdown
===============
*/
void G_Sql_Shutdown(void) {

	if(!sql_use.integer) return;

	sqlite3_close(user_db);
}

/*
===============
G_Sql_Query

Does a SQL Query and returns it's result (max 4096 chars)
===============
*/

// I'm worng check me
void Do_Sql_Query(const char *query, const char *dbName, char *res) {
	char			*query2;
	sqlite3_stmt	*stmt;
	sqlite3			*handle;
	int				res2, cols, i;

	query2 = (char *)malloc(MAX_STRING_CHARS * sizeof(char));
	if(!query2) {
		G_Printf(S_COLOR_RED "ERROR: Was unable to allocate %i byte.\n", MAX_STRING_CHARS * sizeof(char) );
		return;
	}

	if(!dbName) {
		res2 = sqlite3_open("/db/rpgx.sqlite3", &handle);
	} else {
		res2 = sqlite3_open(va("/db/%s.sqlite3", dbName), &handle);
	}

	if(res2) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Database connection failed\n");
		return;
	}

	res2 =  sqlite3_prepare_v2(handle, query, -1, &stmt, 0);
	if(res2) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Query failed\n");
		return;
	}

	cols = sqlite3_column_count(stmt);
	while(1) {
		res2 = sqlite3_step(stmt);
		
		if(res2 == SQLITE_ROW) {
			const char *val;
			for(i = 0; i < cols; i++) {
				val = (const char*)sqlite3_column_text(stmt, i);
				G_Printf("%s = %s\t", sqlite3_column_name(stmt, i), val);
			}
			G_Printf("\n");
		} else if(res2 == SQLITE_DONE) {
			G_Printf("Thats all...\n");
			break;
		} else {
			G_Printf(S_COLOR_RED "SQLITE ERROR: An error occured while printing the result\n");
			break;
		}
	}

	sqlite3_close(handle);
	return;
}

/*
===============
G_Sql_CreateTables
===============
*/
qboolean Do_Sql_CreateTables(const char *dbName) {
	sqlite3 *handle;
	sqlite3_stmt *stmt;
	int res;

	if(!dbName) {
		res = sqlite3_open("/db/rpgx.sqlite3", &handle);
	} else {
		res = sqlite3_open(va("/deb/%s.sqlite3", dbName), &handle);
	}

	if(res) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Database connection failed\n");
		return qfalse;
	}

	res = sqlite3_prepare_v2(handle, SQL_CREATEUSERTABLE, -1, &stmt, 0);  
	if(res) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Was unable to create user table\n");
		return qfalse;
	}
	res = sqlite3_prepare_v2(handle, SQL_CREATERIGHTSTABLE, -1, &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Was uanable to create rights table\n");
		return qfalse;
	}

	sqlite3_close(handle);

	return qfalse;
}

/*
===============
G_Sql_UserDel
===============
*/
qboolean Do_Sql_UserDel(const char *dbName, const char *uName) {
	
	return qtrue;
}

/*
===============
G_Sql_UserMod
===============
*/
qboolean  Do_Sql_UserMod(const char *dbName, const char *uName, const char *right, int value) {
	return qfalse;
}

/*
===============
G_Sql_UserAdd
===============
*/
qboolean Do_Sql_UserAdd(const char *dbName, const char *uName, const char *password) {
	return qfalse;
}

/*
===============
G_Sql_Userlogin
===============
*/
qboolean Do_Sql_UserLogin(const char *dbName, const char *uName, const char *pwd, int clientnum) {
	return qfalse;
}

/*
===============
G_Sql_UserCheckRight
===============
*/
qboolean Do_Sql_UserCheckRight(const char *dbName, int uid, const char *right) {
	return qfalse;
}

#endif //SQL
