#include "db_mgr.hpp"
#include "const.hpp"
#include "table_mgr.hpp"
#include "console_color.hpp"
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
/*                                          INSERT section                                             */ 
/*******************************************************************************************************/
bool db_mgr::insert_device(const char *device_db, const string &table, vector<tuple<string, string, any>>device_vector_tuple)
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
    string sql_cmd = create_table(table, device_vector_tuple);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cout << red << "SQL error: " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    //Insert
    sql_cmd = insert_row(table, device_vector_tuple);
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
/*                                          SELECT section                                             */ 
/*******************************************************************************************************/
string select_by_code_return;
bool db_mgr::select_device(const char *device_db, const string &table, const string& code)
{
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
        return false;

    //Check whether the component is present inside related table
    string sql_cmd = select_row_by_code(table, code);
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
/*                                          DELETE section                                             */ 
/*******************************************************************************************************/
bool db_mgr::delete_device(const char *device_db, const string &table)
{
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
        return false;

    string code_in;
    cout << endl << blue << "Insert device code" << white << endl << endl;
    cout << white << "in: ";
    cin >> code_in;    

    if(!select_device(device_db, table, code_in))
        return false;
    
    //Delete
    string sql_cmd = delete_row(table, code_in);    
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
/*                                          UPDATE section                                             */ 
/*******************************************************************************************************/
bool db_mgr::retrieve_current_device_data(const char *device_db, const std::string &table, const std::string& code, vector<tuple<string, string, any>> &current_data)
{
    vector<string> headers;
    tuple<string, string, any> aux_tuple;
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
        return false;

    //Extract info column by column
    string sql_cmd = select_row_by_code(table, code);
    sqlite3_prepare_v2(db, sql_cmd.c_str(), -1, &stmt, NULL);
    int step = sqlite3_step(stmt);
    int num_cols = sqlite3_column_count(stmt);    
    
    //Print current info of stored device    
    for(int i = 1; i < num_cols; i++)
        headers.push_back(sqlite3_column_name(stmt, i));

    TableMgr device_info(headers);
    device_info.setPadding(TABLE_PADDIND);
    device_info.setStyle(TABLE_STYLE);

    vector<string> string_values;
    int i;
    vector<string>::iterator h;
    long long temp_int_value;
    double temp_real_value;
    string temp_string_value;
    for(h = headers.begin(), i = 1; h != headers.end() && i < num_cols; h++, i++)
    {
        sqlite3_value* value = sqlite3_column_value(stmt, i);        
        switch (sqlite3_value_type(value)) 
        {
            case SQLITE_INTEGER:
                temp_int_value = sqlite3_value_int64(value);
                string_values.push_back(to_string(temp_int_value));
                aux_tuple = make_tuple(*h, "integer", temp_int_value);
                break;
            case SQLITE_FLOAT:            
                temp_real_value = sqlite3_value_double(value);
                string_values.push_back(to_string(temp_real_value));
                aux_tuple = make_tuple(*h, "real", temp_real_value);
                break;
            case SQLITE_TEXT:
                temp_string_value = string(reinterpret_cast<const char *>(sqlite3_value_text(value)));
                string_values.push_back(temp_string_value);
                aux_tuple = make_tuple(*h, "text", temp_string_value);
                break;
        }
        current_data.push_back(aux_tuple);
    }
    device_info += {string_values};
    cout << endl << device_info << endl;
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);    
    return true;
}

//NEED Table name, code and pairs made of <changed columns name, type value, changed values>
bool db_mgr::update_device(const char *device_db, const string& table, const string &code, vector<tuple<string, string, any>> &new_data)
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
    string sql_cmd = update_row(table, code, new_data);
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