
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
G_Sql_Check_StepReturn
===============
*/
static qboolean G_Sql_Check_StepReturn(int r) {
	switch(r) {
		case SQLITE_ROW:
		case SQLITE_DONE:
			return qfalse;
		default:
			G_Printf(S_COLOR_RED "SQL ERROR: %s\n", sqlite3_errmsg(user_db));
			break;
	}
	return qtrue;
}

/*
===============
G_Sql_Check_PrepareReturn
===============
*/
static qboolean G_Sql_Check_PrepareReturn(int r) {
	switch(r) {
		case SQLITE_OK:
			return qfalse;
		default:
			G_Printf(S_COLOR_RED "SQL ERROR: %s\n", sqlite3_errmsg(user_db));
			break;
	}
	return qtrue;
}

/*
===============
G_Sql_Check_BindReturn
===============
*/
static qboolean G_Sql_Check_BindReturn(int r) {
	switch(r) {
		case SQLITE_OK:
			return qfalse;
		default:
			G_Printf(S_COLOR_RED "SQL ERROR: %s\n", sqlite3_errmsg(user_db));
			break;
	}
	return qtrue;
}

/*
===============
G_Sql_Md5
===============
*/
static char *G_Sql_Md5(const char *s) {
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
	if(res != SQLITE_OK) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", sqlite3_errmsg(user_db));
		return qfalse;
	}

	res = sqlite3_prepare_v2(user_db, SQL_ENABLE_FOREIGN_KEY_CONSTRAINTS, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}
	sqlite3_finalize(stmt);

	res = sqlite3_prepare_v2(user_db, SQL_USER_CREATEUSERTABLE, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}
	sqlite3_finalize(stmt);

	res = sqlite3_prepare_v2(user_db, SQL_USER_CREATERIGHTSTABLE, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		sqlite3_close(user_db);
		sql_ready = qfalse;
		return qfalse;
	}
	sqlite3_finalize(stmt);

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
G_Sql_UserDB_Del
===============
*/
qboolean G_Sql_UserDB_Del(const char *uName) {
	sqlite3_stmt *stmt;
	int res, id = -1;

	res = sqlite3_prepare_v2(user_db, "SELECT id FROM rpgx_users WHERE username = :UNAME", -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_text(stmt, 1, uName, sizeof(uName), SQLITE_STATIC);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}

	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}
	if(res == SQLITE_ROW) {
		id = sqlite3_column_int(stmt, 0);
	} else if(res == SQLITE_DONE) {
		G_Printf("SQL: User \'%s\' not found\n", uName);
		return qfalse;
	} 
	sqlite3_finalize(stmt);

	if(id == -1) {
		G_Printf(S_COLOR_RED "SQL ERROR: no user %s found\n", uName);
		return qfalse;
	}

	res = sqlite3_prepare_v2(user_db, SQL_USER_DELTE_RIGHTS, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_int(stmt, 1, id);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}
	sqlite3_finalize(stmt);

	res = sqlite3_prepare_v2(user_db, SQL_USER_DELETE, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_text(stmt, 1, uName, sizeof(uName), SQLITE_STATIC);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}
	sqlite3_finalize(stmt);

	return qtrue;
}

/*
===============
G_Sql_UserAdd
===============
*/
qboolean Do_Sql_UserAdd(const char *uName, const char *password) {
	sqlite3_stmt *stmt;
	int res, id;
	char *hashedpw;

	res = sqlite3_prepare_v2(user_db, SQL_USER_GET_UID, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_text(stmt, 1, uName, sizeof(uName), SQLITE_STATIC);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}

	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}
	if(res == SQLITE_ROW) {
		G_Printf("SQL: There already exists a user with username \'%s\'\n", uName);
		G_Printf("SQL: If you lost your password please contact an admin with access to the database.\n");
		sqlite3_finalize(stmt);
		return qfalse;
	}
	sqlite3_finalize(stmt);

	hashedpw = G_Sql_Md5(password);
	res = sqlite3_prepare_v2(user_db, SQL_USER_ADD, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_text(stmt, 1, uName, sizeof(uName), SQLITE_STATIC);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_text(stmt, 2, hashedpw, sizeof(hashedpw), free);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}

	res = sqlite3_prepare_v2(user_db, SQL_USER_GET_UID, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_text(stmt, 1, uName, sizeof(uName), SQLITE_STATIC);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}

	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}
	if(res == SQLITE_ROW) {
		id = sqlite3_column_int(stmt, 0);
	} else {
 		G_Printf("SQL: There already exists a user with username \'%s\'\n", uName);
		sqlite3_finalize(stmt);
		return qfalse;
	}
	sqlite3_finalize(stmt);

	res = sqlite3_prepare_v2(user_db, SQL_USER_ADD_RIGHTS, -1, &stmt, 0);
	if(G_Sql_Check_PrepareReturn(res)) {
		return qfalse;
	}
	res = sqlite3_bind_int(stmt, 1, id);
	if(G_Sql_Check_BindReturn(res)) {
		return qfalse;
	}
	res = sqlite3_step(stmt);
	if(G_Sql_Check_StepReturn(res)) {
		return qfalse;
	}
	sqlite3_finalize(stmt);

	return qtrue;
}

/*
===============
G_Sql_UserDB_login
===============
*/
qboolean G_Sql_UserDB_login(const char *uName, const char *pwd, int clientnum) {

	return qfalse;
}

/*
===============
G_Sql_UserDB_CheckRight
===============
*/
qboolean G_Sql_UserDB_CheckRight(int uid, int right) {
	return qfalse;
}

/*
===============
G_Sql_UserDB_AddRight
===============
*/
qboolean G_Sql_UserDB_AddRight(int uid, int right) {

}

/*
===============
G_Sql_UserDB_RemoveRight
===============
*/
qboolean G_Sql_UserDB_RemoveRight(int uid, int right) {

}

#endif //SQL
