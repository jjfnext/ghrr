#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

int callback(void *NotUsed, int argc, char **argv, char **azColName) 
{    
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");   
    return 0;
}

int main()
{
	const char* db_name = "../test/car.db";
	sqlite3 *db;
	char *err_msg = 0;
	int rc;

	cout << "test sqlite" << endl;

	cout << "version: " << 	sqlite3_libversion() << endl;
	rc = sqlite3_open(db_name, &db);
	if (rc != SQLITE_OK) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return 1;
	}

	const char* sql = "SELECT * from Cars;";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {       
        cerr << "SQL error: " << err_msg << endl;
        sqlite3_free(err_msg);        
        sqlite3_close(db);        
        return 1;
    } 
    
    sqlite3_close(db);    
    return 0;	
}
