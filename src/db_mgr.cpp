#include "db_mgr.hpp"
#include "const.hpp"
#include "ConsoleColor.hpp"
#include "sqlite3.h"
#include "sql_cmds.hpp"

using namespace std;


db_mgr::db_mgr()
{

}


db_mgr::~db_mgr()
{

}


bool db_mgr::insert_device(const string &DEVICE, const char *DEVICE_DB, vector<tuple<string, string, any>>device_vector_tuple)
{    
    if(!is_file_present(DB_FILE_PATH, DEVICE_DB))
        if(!create_file(DB_FILE_PATH, DEVICE_DB))
            return false;

    //Connect to database
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;
    string ABS_DEVICE_DB = string(DB_FILE_PATH) + string(DEVICE_DB);
    int rc = sqlite3_open(ABS_DEVICE_DB.c_str(), &db);

    if(rc)
    {
        cerr << red << "Cannot open " << DEVICE_DB << " because of: " << sqlite3_errmsg(db) << white << endl;
        return false;
    }
    
    //Check if table exists otherwise it is created
    string sql_cmd = create_table(DEVICE, device_vector_tuple);

    rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    //Insert
    sql_cmd = insert_row(DEVICE, device_vector_tuple);
    rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);
    return true;
}


string select_by_code_return;
bool db_mgr::delete_device(const string &DEVICE, const char *DEVICE_DB)
{
    string code_in;
    int ret;

    cout << endl << blue << "Insert device code" << white << endl << endl;
    cout << white << "in: ";
    cin >> code_in;    

    //Connect to database
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;
    string ABS_DEVICE_DB = string(DB_FILE_PATH) + string(DEVICE_DB);
    int rc = sqlite3_open(ABS_DEVICE_DB.c_str(), &db);

    if(rc)
    {
        cerr << red << "Cannot open " << DEVICE_DB << " because of: " << sqlite3_errmsg(db) << white << endl;
        return false;
    }

    //Check whether the component is present inside related table
    string sql_cmd = select_row_by_code(DEVICE, code_in);
    rc = sqlite3_exec(db, sql_cmd.c_str(), select_by_code, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    }
    if(select_by_code_return == string())
    {
        cout << red << "Not code found!" << white << endl;
        return false;
    }
    
    //Delete
    sql_cmd = delete_row(DEVICE, code_in);
    rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);
    return true;
}