#include "db_mgr.hpp"
#include "const.hpp"
#include "ConsoleColor.hpp"
#include "sql_cmds.hpp"

using namespace std;


db_mgr::db_mgr()
{

}


db_mgr::~db_mgr()
{

}


sqlite3 *db_mgr::database_connection(const char *device_db)
{
    sqlite3 *db = nullptr;
    string abs_device_db = string(DB_FILE_PATH) + string(device_db);
    int rc = sqlite3_open(abs_device_db.c_str(), &db);

    if(rc)
    {
        cerr << red << "Cannot open " << device_db << " because of: " << sqlite3_errmsg(db) << white << endl;
        db = nullptr;
    }

    return db;
}


/*******************************************************************************************************/
/*                                          INSERT wrapper                                             */ 
/*******************************************************************************************************/
bool db_mgr::insert_device(const string &device, const char *device_db, vector<tuple<string, string, any>>device_vector_tuple)
{    
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    if(!is_file_present(DB_FILE_PATH, device_db))
        if(!create_file(DB_FILE_PATH, device_db))
            return false;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
        return false;
  
    //Check if table exists otherwise it is created
    string sql_cmd = create_table(device, device_vector_tuple);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    //Insert
    sql_cmd = insert_row(device, device_vector_tuple);
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


/*******************************************************************************************************/
/*                                          SELECT wrapper                                             */ 
/*******************************************************************************************************/
bool db_mgr::select_device(const string &device, const char *device_db)
{
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    string code_in;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
        return false;

    //Check whether the component is present inside related table
    string sql_cmd = select_row_by_code(device, code_in);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), select_by_code, NULL, &zErrMsg);

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

    sqlite3_close(db);
    return true;
}


/*******************************************************************************************************/
/*                                          DELETE wrapper                                             */ 
/*******************************************************************************************************/
string select_by_code_return;
bool db_mgr::delete_device(const string &device, const char *device_db)
{
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;
    string code_in;
    int ret;

    cout << endl << blue << "Insert device code" << white << endl << endl;
    cout << white << "in: ";
    cin >> code_in;    

    if(!select_device(device, device_db))
        return false;
    
    //Delete
    string sql_cmd = delete_row(device, code_in);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);
    return true;
}


/*******************************************************************************************************/
/*                                          UPDATE wrapper                                             */ 
/*******************************************************************************************************/
bool db_mgr::update_device(const string &device, const char *device_db, vector<pair<string, string>>device_change_pair)
{   
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    if(!is_file_present(DB_FILE_PATH, device_db))
        if(!create_file(DB_FILE_PATH, device_db))
            return false;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
        return false;

    //Update
    string sql_cmd = update_row(device, device_change_pair);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);
    return true;
}


/*******************************************************************************************************/
/*                                     SQLite command wrappers                                         */ 
/*******************************************************************************************************/
void show_device(const string &device, const char *device_db)
{
    //system("sqlite3" + SPACE + device_db + DOUBLE_QUOTE +  + DOUBLE_QUOTE);
}