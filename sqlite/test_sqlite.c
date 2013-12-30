/**
 *	@filename :
 * 		test_sqlite.c
 * 
 * 	@note :
 * 		查看tvchannels所有内容
 * 		./bin/test_sqlite sqlite/livetv.db 'select * from tvchannels'
 * 		
 * 		查看数据库里所有表的结构
 * 		./bin/test_sqlite sqlite/livetv.db 'select * from sqlite_master where type="table";'
 * 		
 * 		./bin/test_sqlite sqlite/livetv.db 'select * from sqlite_master where type="table" and name="tvchannels";'
 *  
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	
	return 0;
}

int main(int argc, char **argv)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	if( argc!=3 ) {
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return(1);
	}
	
	rc = sqlite3_open(argv[1], &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	return 0;
}
