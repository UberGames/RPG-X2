
#ifdef SQL

#if (defined(_MSC_VER) | defined(__WIN32__))
#include "windows.h"
#endif

#include "sqlite3.h"
//#include "q_shared.h"
#include "g_sql.h"
#include "g_local.h"
#include "md5.h"

extern vmCvar_t sql_dbName;
extern vmCvar_t sql_use;
extern vmCvar_t sql_server;
extern vmCvar_t sql_user;
extern vmCvar_t sql_password;
extern vmCvar_t sql_port;

extern void QDECL G_Printf( const char *fmt, ... );
extern void QDECL G_PrintfClient( gentity_t *ent, const char *fmt, ...);

sqlite3	*user_db;
qboolean sql_ready = qfalse;

/*
===============
G_Sql_Md5
===============
*/
char *G_Sql_Md5(char *s) {
	char *res;
	unsigned char sig[16];
	struct MD5Context md5c;

	MD5Init(&md5c);
	MD5Update(&md5c, s, strlen(s));
	MD5Final(sig, &md5c);

	res = (char *)malloc(sizeof(char)*MAX_QPATH);
	if(!res) {
		G_Printf(S_COLOR_RED "SQL ERROR: was unable to allocate %u byte\n", sizeof(char)*(strlen(s)+1));
		return NULL;
	}

	sprintf(res, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
					sig[0], sig[1], sig[2], sig[3], sig[4], sig[5], sig[6], sig[7],
					sig[8], sig[9], sig[10], sig[11], sig[12], sig[13], sig[14], sig[15] );
	
	return res;
}

/*
===============
G_SqlInit
===============
*/
qboolean G_Sql_Init(void) {
	int res;
	sqlite3_stmt *stmt;

	if(!sql_use.integer) return qtrue;

	res = sqlite3_open("db/users.db", &user_db);
	if(res) {
		G_Printf(S_COLOR_RED "SQL ERROR: Could not open user database\n");
		return qfalse;
	}

	res = sqlite3_prepare_v2(user_db, SQL_ENABLE_FOREIGN_KEY_CONSTRAINTS, -1, &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQL ERROR: Enabling foreign key constraints failed\n");
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}

	sql_ready = qtrue;
	return qtrue;
}

/*
===============
G_Sql_Shutdown
===============
*/
void G_Sql_Shutdown(void) {

	if(!sql_use.integer) return;

	sql_ready = qfalse;
	sqlite3_close(user_db);
}

/*
===============
G_Sql_Query

Does a SQL Query and returns it's result (max 4096 chars)
===============
*/
/*
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
}*/

/*
===============
G_Sql_UserDB_CreateTables
===============
*/
qboolean G_Sql_UserDB_CreateTables(const char *dbName) {
	sqlite3_stmt *stmt;
	int res;

	if(!sql_use.integer || !sql_ready) return qtrue;

	res = sqlite3_prepare_v2(user_db, SQL_CREATEUSERTABLE, -1, &stmt, 0);  
	if(res) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Was unable to create user table\n");
		return qfalse;
	}
	res = sqlite3_prepare_v2(user_db, SQL_CREATERIGHTSTABLE, -1, &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Was uanable to create rights table\n");
		return qfalse;
	}

	sqlite3_free(stmt);
	return qtrue;
}

/*
===============
G_Sql_UserDB_Del
===============
*/
qboolean G_Sql_UserDB_Del(const char *dbName, const char *uName) {
	sqlite3_stmt *stmt;
	int res, cols, i, id = -1;

	res = sqlite3_prepare_v2(user_db, SQL_GET_UID(uName), -1, &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQL ERROR: Query failed\n");
		return qfalse;
	}

	cols = sqlite3_column_count(stmt);
	for(i = 0; i < cols; i++) {
		res = sqlite3_step(stmt);

		if(res == SQLITE_ROW) {
			id = sqlite3_column_int(stmt, 0);
		} else if(res == SQLITE_DONE) {
			break;
		} else {
			G_Printf(S_COLOR_RED "SQL ERROR: An error occured getting results from a row\n");
			return qfalse;
		}
	}
	sqlite3_free(stmt);

	if(id == -1) {
		G_Printf(S_COLOR_RED "SQL ERROR: no user %s found\n", uName);
		return qfalse;
	}

	res = sqlite3_prepare_v2(user_db, SQL_USER_DELTE_RIGHTS(id), -1, &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQL ERROR: Query failed\n");
		return qfalse;
	}
	sqlite3_free(stmt);

	res = sqlite3_prepare_v2(user_db, SQL_USER_DELETE(uName), -1, &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQL ERROR: Query failed\n");
		return qfalse;
	}
	sqlite3_free(stmt);

	return qtrue;
}

/*
===============
G_Sql_UserDB_Mod
===============
*/
qboolean  G_Sql_UserDB_Mod(const char *dbName, const char *uName, const char *right, int value) {
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
G_Sql_UserDB_login
===============
*/
qboolean G_Sql_UserDB_login(const char *dbName, const char *uName, const char *pwd, int clientnum) {
	return qfalse;
}

/*
===============
G_Sql_UserDB_CheckRight
===============
*/
qboolean G_Sql_UserDB_CheckRight(const char *dbName, int uid, int right) {
	return qfalse;
}

#endif //SQL
