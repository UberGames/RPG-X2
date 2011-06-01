
#ifdef SQL

#if (defined(_MSC_VER) | defined(__WIN32__))
#include "windows.h"
#endif

#include "../mysql/mysql.h"
#include "../sqlite/sqlite3.h"
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

/*
===============
Do_Mysql_Hash
===============
*/
static const char *Do_Mysql_Hash(const char *str) {
	switch(sql_hash.integer) {
		default:
		case 0:
			return va("MD5(\'%s\')", str);
		case 1:
			return va("SHA1(\'%s)\'", str);
		case 2:
			return va("SHA512(\'%s\')", str);
	}
}

/*
===============
G_Sql_Query

Does a SQL Query and returns it's result (max 4096 chars)
===============
*/
static void Do_Mysql_Query(const char *query, const char *dbName, char *res) {
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int i;
	char *r;

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, sql_port.integer, NULL, 0)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return;
	}

	if(mysql_query(conn, query)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return;
	}

	r = malloc(MAX_SQL_RESULT * sizeof(char));

	result = mysql_use_result(conn);

	while((row = mysql_fetch_row(result)) != NULL) {
		for(i = 0; i < mysql_num_rows(result); i++) {
			if(!r[0])
				Com_sprintf(r, sizeof(r), "%s ", row[i]);
			else
				Com_sprintf(r, sizeof(r), "%s%s ", r, row[i]);
		}
		Com_sprintf(r, sizeof(r), "%s\n", r);
	}
	
	if(r)
		res = r;
	else {
		free(r);
		res = NULL;
	}

	mysql_free_result(result);
	mysql_close(conn);
}

// I'm worng check me
static void Do_Sqlite_Query(const char *query, const char *dbName, char *res) {
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

void G_Sql_Query(const char *query, const char *dbName, char *res) {
	if(sql_use.integer == 1)
		Do_Mysql_Query(query, dbName, res);
	else if(sql_use.integer == 2)
		Do_Sqlite_Query(query, dbName, res);
}

/*
===============
G_Sql_CreateTables
===============
*/
static qboolean Do_Mysql_CreateTables(const char *dbName) {
	MYSQL *conn;

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, 0, NULL, 0)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	if(mysql_query(conn, SQL_CREATEUSERTABLE)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	mysql_store_result(conn);

	if(mysql_query(conn, SQL_CREATERIGHTSTABLE)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	mysql_store_result(conn);

	mysql_close(conn);

	return qtrue;
}

static qboolean Do_Sqlite_CreateTables(const char *dbName) {
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
	res = sqlite3_prepare_v2(handle, SQL_CREATERIGHTSTABLE, -1 &stmt, 0);
	if(res) {
		G_Printf(S_COLOR_RED "SQLITE ERROR: Was uanable to create rights table\n");
		return qfalse;
	}

	sqlite3_close(handle);

	return qfalse;
}

qboolean G_Sql_CreateTables(const char *dbName) {
	if(sql_use.integer == 1)
		return Do_Mysql_CreateTables(dbName);
	else if(sql_use.integer == 2)
		return Do_Sqlite_CreateTables(dbName);
}


/*
===============
G_Sql_UserDel
===============
*/
static qboolean Do_Mysql_UserDel(const char *dbName, const char *uName) {
	MYSQL *conn;
	char *query = SQL_DELETE("rpgxEF_rights", va("`id` = (%s) LIMIT 1", SQL_GET_UID(uName)));

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, 0, NULL, 0)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	if(mysql_query(conn, query)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	query = SQL_DELETE("rpgxEF_users", va("`username` = \'%s\' LIMIT 1", uName));

	if(mysql_query(conn, query)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	return qtrue;
}

static qboolean Do_Sqlite_UserDel(const char *dbName, const char *uName) {
	
	return qtrue;
}

qboolean G_Sql_UserDel(const char *dbName, const char *uName) {
	if(sql_use.integer == 1)
		return Do_Mysql_UserDel(dbName, uName);
	else if(sql_use.integer == 2)
		return Do_Sqlite_UserDel(dbName, uName);
}

/*
===============
G_Sql_UserMod
===============
*/
static qboolean  Do_Mysql_UserMod(const char *dbName, const char *uName, const char *right, int value) {
	MYSQL *conn;
	char *query = va("UPDATE `rpgxEF_rights` SET `%s` = %i WHERE (%s)", right, value, SQL_GET_UID(uName));

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, 0, NULL, 0)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	if(mysql_query(conn, query)) {
		G_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	if(mysql_affected_rows(conn) == 0) {
		mysql_close(conn);
		return qfalse; // no such user
	}

	mysql_close(conn);
		
	return qtrue;
}

static qboolean  Do_Sqlite_UserMod(const char *dbName, const char *uName, const char *right, int value) {
	return qfalse;
}

qboolean G_Sql_UserMod(const char *dbName, const char *uName, const char *right, int value) {
	if(sql_use.integer == 1)
		return Do_Mysql_UserMod(dbName, uName, right, value);
	else if(sql_use.integer == 2)
		return Do_Sqlite_UserMod(dbName, uName, right, value);
}

/*
===============
G_Sql_UserAdd
===============
*/
static qboolean Do_Mysql_UserAdd(const char *dbName, const char *uName, const char *password) {
	MYSQL *conn;
	char *query = va("INSERT INTO `rpgxEF_users` VALUES (NULL, \'%s\', %s, NULL)", uName, Do_Mysql_Hash(password));
	char *query2 = va("INSERT DELAYED INTO `rpgxEF_rights` (\
					`id` , `admin` , `give` , `god` , `noclip` , `cloak` , `evosuit` , `flight` ,\
					`forcename` , `forcekill` , `forcekillradius` , `targetkick` , `drag` ,\
					`disarm` , `forceRank` , `revive` , `n00b` , `msg` , `msg2` , `forcemodel` ,\
					`play` , `stopmusic` , `sound` , `useEnt` , `beam` , `forceplayer` , `fx` ,\
					`flushfx` , `spawnchar` , `flushchars` , `kick2` , `clamp` , `lock` ,\
					`unlockall` , `ffcolor` , `alert` , `lockall` , `tess` , `forceclass` ,\
					`forcevote`, `shake` )\
					VALUES ( LAST_INSERT_ID(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
					)");

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, 0, NULL, 0)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	if(mysql_query(conn, query)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	if(mysql_query(conn, query2)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	mysql_close(conn);

	return qtrue;
}

static qboolean Do_Sqlite_UserAdd(const char *dbName, const char *uName, const char *password) {
	return qfalse;
}

qboolean G_Sql_UserAdd(const char *dbName, const char *uName, const char *password) {
	if(sql_use.integer == 1)
		return Do_Mysql_UserAdd(dbName, uName, password);
	else if(sql_use.integer == 2)
		return Do_Sqlite_UserAdd(dbName, uName, password);
}

/*
===============
G_Sql_Userlogin
===============
*/
static qboolean Do_Mysql_UserLogin(const char *dbName, const char *uName, const char *pwd, int clientnum) {
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char *query = va("SELECT `id` FROM `rpgxEF_users` WHERE `username` = \'%s\' AND `password` = %s LIMIT 1", uName, Do_Mysql_Hash(pwd));
	int i;
	gentity_t *client = &g_entities[clientnum];

	if(!client || !client->client)
		return qfalse;

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, 0, NULL, 0)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		G_PrintfClient(client, "Login Failed: Mysql Connection Error\n");
		return qfalse;
	}

	if(mysql_query(conn, query)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		G_PrintfClient(client, "Login Failed: Mysql Query Error\n");
		return qfalse;
	}

	if(mysql_affected_rows(conn) == 0) return 0;

	result = mysql_use_result(conn);

	row = mysql_fetch_row(result);

	if(row != NULL)
		i = atoi(row[0]);
	else
		i = 0;

	mysql_free_result(result);
	mysql_close(conn);

	if(!i) {
		G_PrintfClient(client, "Login Failed: Uknown user or wrong password\n");
		return qfalse;
	} else {
		client->client->uid = i;
		G_PrintfClient(client, "Login Successful\n");
		return qtrue;
	}
}

static qboolean Do_Sqlite_UserLogin(const char *dbName, const char *uName, const char *pwd, int clientnum) {
	return qfalse;
}

qboolean G_Sql_UserLogin(const char *dbName, const char *uName, const char *pwd, int clientnum) {
	if(sql_use.integer == 1)
		return Do_Mysql_UserLogin(dbName, uName, pwd, clientnum);
	else if(sql_use.integer = 2)
		return Do_Sqlite_UserLogin(dbName, uName, pwd, clientnum);
}

/*
===============
G_Sql_UserCheckRight
===============
*/
static qboolean Do_Mysql_UserCheckRight(const char *dbName, int uid, const char *right) {
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char *query;
	int i;

	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, sql_server.string, sql_user.string, sql_password.string, dbName, 0, NULL, 0)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	query = va("SELECT `%s` FROM `rpgxEF_rights` WHERE `id` = %i", right, uid);

	if(mysql_query(conn, query)) {
		Com_Printf(S_COLOR_RED "SQL ERROR: %s\n", mysql_error(conn));
		mysql_close(conn);
		return qfalse;
	}

	result = mysql_use_result(conn);

	row = mysql_fetch_row(result);

	if(row == NULL) {
		mysql_free_result(result);
		mysql_close(conn);
		return qfalse;
	}

	i = atoi(row[0]);

	mysql_free_result(result);
	mysql_close(conn);

	if(i) return qtrue;
	else return qfalse;
}

static qboolean Do_Sqlite_UserCheckRight(const char *dbName, int uid, const char *right) {
	return qfalse;
}

qboolean G_Sql_UserCheckRight(const char *dbName, int uid, const char *right) {
	if(sql_use.integer == 1)
		return Do_Mysql_UserCheckRight(dbName, uid, right);
	else if(sql_use.integer == 2)
		return Do_Sqlite_UserCheckRight(dbName, uid, right);
}

#endif //SQL
