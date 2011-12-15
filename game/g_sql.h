
#define MAX_SQL_RESULT	4096
#define SQL_ENABLE_FOREIGN_KEY_CONTRAINTS "PRAGMA foreign_keys = ON;"
#define SQL_GET_UID(UNAME) va("SELECT id FROM rpgx_users WHERE username = %s", UNAME)
#define SQL_CREATEUSERTABLE "CREATE TABLE rpgx_users ( \
							id INTEGER PRIMARY KEY AUTOINCREMENT, \
							username TEXT NOT NULL , \
							password TEXT NOT NULL, \
							mail TEXT \
							)"
#define SQL_CREATERIGHTSTABLE "CREATE TABLE rpgx_userRights ( \
									id INT NOT NULL PRIMARY KEY, \
									admin BIT NOT NULL, \
									rights LONG NOT NULL, \
									FOREIGN KEY(id) REFERENCES rpgx_users(id) \
									)"

typedef enum {
	SQLF_GIVE			= 1,
	SQLF_GOD			= 2,
	SQLF_NOCLIP			= 4,
	SQLF_CLOAK			= 8,
	SQLF_EVOSUIT		= 16,
	SQLF_FLIGHT			= 32,
	SQLF_FORCENAME		= 64,
	SQLF_FORCEKILL		= 128,
	SQLF_KICK			= 256,
	SQLF_DRAG			= 512,
	SQLF_DISARM			= 1024,
	SQLF_FORCERANK		= 2048,
	SQLF_REVIVE			= 4096,
	SQLF_NOOB			= 8096,
	SQLF_MESSAGE		= 16384,
	SQLF_FORCEMODEL		= 32768,
	SQLF_MUSIC			= 65536,
	SQLF_SOUND			= 131072,
	SQLF_USEENT			= 262144,
	SQLF_BEAM			= 524288,
	SQLF_FORCEPLAYER	= 1048576,
	SQLF_FX				= 2097152,
	SQLF_CHARS			= 4194304,
	SQLF_CLAMP			= 8388608,
	SQLF_LOCK			= 16777216,
	SQLF_FFCOLOR		= 33554432,
	SQLF_ALERT			= 67108864,
	SQLF_TESS			= 134217728,
	SQLF_FORCECLASS		= 268435456,
	SQLF_FORCEVOTE		= 536870912,
	SQLF_SHAKE			= 1073741824
} sql_userflags;

qboolean G_SqlInit(void);
