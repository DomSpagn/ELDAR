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


bool db_mgr::add_resistor(vector<tuple<string, string, any>>resistor_vector_tuple)
{
    if(!is_file_present(DB_FILE_PATH, RESISTOR_DB))
        if(!create_file(DB_FILE_PATH, RESISTOR_DB))
            return false;

    //Connect to database
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;
    string ABS_RESISTOR_DB = string(DB_FILE_PATH) + string(RESISTOR_DB);
    int rc = sqlite3_open(ABS_RESISTOR_DB.c_str(), &db);

    if(rc)
    {
        cerr << red << "Cannot open " << RESISTOR_DB << " because of: " << sqlite3_errmsg(db) << white << endl;
        return false;
    }
    
    //Check if table exists otherwise it is created
    string sql_cmd = create_table("RESISTOR", resistor_vector_tuple);

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


bool db_mgr::add_capacitor(vector<tuple<string, string, any>>capacitor_vector_tuple)
{
    if(!is_file_present(DB_FILE_PATH, CAPACITOR_DB))
        if(!create_file(DB_FILE_PATH, CAPACITOR_DB))
            return false;

    //Connect to database
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;
    string ABS_CAPACITOR_DB = string(DB_FILE_PATH) + string(CAPACITOR_DB);
    int rc = sqlite3_open(ABS_CAPACITOR_DB.c_str(), &db);

    if(rc)
    {
        cerr << red << "Cannot open " << CAPACITOR_DB << " because of: " << sqlite3_errmsg(db) << white << endl;
        return false;
    }
    
    //Check if table exists otherwise it is created
    string sql_cmd = create_table("CAPACITOR", capacitor_vector_tuple);

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


bool db_mgr::add_inductor(vector<tuple<string, string, any>>inductor_vector_tuple)
{
    if(!is_file_present(DB_FILE_PATH, INDUCTOR_DB))
        if(!create_file(DB_FILE_PATH, INDUCTOR_DB))
            return false;

    //Connect to database
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;
    string ABS_INDUCTOR_DB = string(DB_FILE_PATH) + string(INDUCTOR_DB);
    int rc = sqlite3_open(ABS_INDUCTOR_DB.c_str(), &db);

    if(rc)
    {
        cerr << red << "Cannot open " << INDUCTOR_DB << " because of: " << sqlite3_errmsg(db) << white << endl;
        return false;
    }
    
    //Check if table exists otherwise it is created
    string sql_cmd = create_table("INDUCTOR", inductor_vector_tuple);

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