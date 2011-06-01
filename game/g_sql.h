
#define MAX_SQL_RESULT	4096
#define SQL_GET_UID(UNAME) va("SELECT `id` FROM `rpgxEF_users` WHERE `username` = \'%s\'", UNAME)
#define SQL_DELETE(TNAME, COND) va("DELETE FROM `%s` WHERE %s", TNAME, COND)
#define SQL_INSERT(TNAME, L1, L2) va("INSERT INTO `%s` (%s) VALUES (%s)", TNAME, L1, L2)
#define SQL_INSERT_DELAYED(TNAME, L1, L2) va("INSERT DELAYED INTO `%s` (%s) VALUES (%s)", TNAME, L1, L2)
#define SQL_UPDATE(TNAME, CHANGE, CONDITION) va("UPDATE `%s` SET %s WHERE %s", TNAME, CHANGE, CONDITION)
#define SQL_CREATEUSERTABLE "CREATE TABLE `rpgxEF_users` ( \
							`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY , \
							`username` LONGTEXT NOT NULL , \
							`password` LONGTEXT NOT NULL, \
							`mail` LONGTEXT \
							) ENGINE = MYISAM"
#define SQL_CREATERIGHTSTABLE "CREATE TABLE `rpgxEF_rights` ( \
									`id` INT NOT NULL, \
									`admin` INT, \
									`give` INT, \
									`god` INT, \
									`noclip` INT, \
									`cloak` INT, \
									`evosuit` INT, \
									`flight` INT, \
									`forcename` INT, \
									`forcekill` INT, \
									`forcekillradius` INT, \
									`targetkick` INT, \
									`drag` INT, \
									`disarm` INT, \
									`forceRank` INT, \
									`revive` INT, \
									`n00b` INT, \
									`msg` INT, \
									`msg2` INT, \
									`forcemodel` INT, \
									`play` INT, \
									`stopmusic` INT, \
									`sound` INT, \
									`useEnt` INT, \
									`beam` INT, \
									`forceplayer` INT, \
									`fx` INT, \
									`flushfx` INT, \
									`spawnchar` INT, \
									`flushchars` INT, \
									`kick2` INT, \
									`clamp` INT, \
									`lock` INT, \
									`unlockall` INT, \
									`ffcolor` INT,  \
									`alert` INT, \
									`lockall` INT, \
									`tess` INT, \
									`forceclass` INT, \
									`forcevote` INT, \
									`shake` INT \
									) ENGINE = MYISAM"
