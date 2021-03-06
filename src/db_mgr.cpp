#include "db_mgr.hpp"
#include "const.hpp"
#include "console_color.hpp"
#include "sql_cmds.hpp"
#include <dirent.h>
#include <list>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>

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
        cerr << endl << red << "Cannot open " << device_db << " because of: " << sqlite3_errmsg(db) << white << endl;
        db = nullptr;
    }

    return db;
}


/*******************************************************************************************************/
/*                                          INSERT section                                             */ 
/*******************************************************************************************************/
bool db_mgr::insert_device(const char *device_db, const string &table, vector<pair<string, string>> &device_info)
{    
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    if(!is_file_present(DB_FILE_PATH, device_db))
        if(!create_file(DB_FILE_PATH, device_db))
            return false;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
    {
        cerr << endl << red << "Cannot connect to " << device_db << white << endl;
        return false;
    }
  
    //Check if table exists otherwise it is created
    string sql_cmd = create_table(table, device_info);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cerr << endl << red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    //Insert
    sql_cmd = insert_row(table, device_info);
    rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cerr << endl << red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);
    return true;
}


/*******************************************************************************************************/
/*                                          SELECT section                                             */ 
/*******************************************************************************************************/
string db_mgr::select_by_code_return = string();
db_mgr::SEARCH_RESULT db_mgr::select_device(const char *device_db, const string &table, const string& code)
{
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
    {
        cerr << endl << red << "Cannot connect to " << device_db << white << endl;
        return SEARCH_ERROR;
    }

    //Check whether the component is present inside related table
    string sql_cmd = select_row_by_code(table, code);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), select_by_code, &select_by_code_return, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cerr << endl << red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return SEARCH_ERROR;
    }
    if(select_by_code_return == string())
        return SEARCH_NOT_FOUND;

    sqlite3_close(db);
    return SEARCH_FOUND;
}


bool db_mgr::show_table(const char *device_db, const string &table)
{
    if(!is_file_present(DB_FILE_PATH, device_db))
    {
        cout << endl << red << "The selected DB is not present" << white << endl;
        return false;
    }

    return show_sql_table(device_db, table);
}


bool db_mgr::show_device_by_parameter(const char *device_db, const string &table, const string &parameter, const string &parameter_value, bool exact, unsigned int &counter)
{
    return show_sql_device(device_db, table, parameter, parameter_value, exact, counter);
}


bool db_mgr::has_suffix(const string &s, const string &suffix)
{
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());    
}


string db_mgr::get_table(const char *db_name)
{
    string table_name;
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(db_name);
    if(db == nullptr)
    {
        cerr << endl << red << "Cannot connect to " << db_name << white << endl;
        return string();
    }

    //Retrieve table name from related DB
    if(!get_table_from_db(db_name, table_name))
        return string(); 

    return table_name;       
}


bool db_mgr::build_db_table_map(map<string, string> &db_table_names)
{
    bool ret = false;    
    DIR *dir = opendir(DB_FILE_PATH);
    if(!dir)
    {
        cerr << endl << red << "Cannot open db file path..." << white << endl;        
        return ret;
    }
    else
    {
        dirent *entry;
        while((entry = readdir(dir)) != NULL) 
        {
            if(has_suffix(entry->d_name, ".db"))
            {
                string table_name = get_table(entry->d_name);
                if(!table_name.empty())
                    db_table_names.insert(pair<string, string>(entry->d_name, table_name));
                else
                {
                    closedir(dir);
                    return ret;
                }
            }
        }
        closedir(dir);
    }

    if(db_table_names.size() > 0)        
        ret = true;
    else
    {
        cout << endl << yellow << "No DB is present!" << white << endl;
        ret = false;
    }

    return ret;
}


db_mgr::SEARCH_RESULT db_mgr::search_code_among_dbs(map<string, string> &db_and_table_names, const string &code)
{    
    SEARCH_RESULT search_res;

    for(auto db_table_pair : db_and_table_names)
    {
        search_res = select_device(db_table_pair.first.c_str(), db_table_pair.second, code);
        if(search_res == SEARCH_ERROR)
            break;
        else if(search_res == SEARCH_FOUND)
        {
            search_res = SEARCH_FOUND;
            break;
        }
        else
            search_res = SEARCH_NOT_FOUND;
    }
    return search_res;
}

/*******************************************************************************************************/
/*                                          DELETE section                                             */ 
/*******************************************************************************************************/
int db_mgr::table_count;
bool db_mgr::delete_device(const char *device_db, const string &table)
{
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
    {
        cerr << endl << red << "Cannot connect to " << device_db << white << endl;
        return false;
    }

    string code_in;
    cout << endl << blue << "Insert device code" << white << endl;
    cout << endl << white << "in: ";
    cin >> code_in;    

    if(!check_range_validity(code_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    switch(select_device(device_db, table, code_in))
    {
        case db_mgr::SEARCH_FOUND:
        break;

        case db_mgr::SEARCH_NOT_FOUND:
        {
            cerr << endl << yellow << "Not code found!" << white << endl;
            return false;
        }
        break;

        case db_mgr::SEARCH_ERROR:
        {
            return false;
        }
        break;
    }
  
    //Delete
    string sql_cmd = delete_row(table, code_in);    
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cerr << endl << red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    }

    //Delete db file whether the selected device was the last one
    sql_cmd = table_size(device_db, table);
    rc = sqlite3_exec(db, sql_cmd.c_str(), select_to_count, &table_count, &zErrMsg);
    if (rc != SQLITE_OK) 
    {
        cerr << endl << red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);   
    return true;
}


/*******************************************************************************************************/
/*                                          UPDATE section                                             */ 
/*******************************************************************************************************/
bool db_mgr::retrieve_current_device_data(const char *device_db, const string &table, const string& code, vector<pair<string, string>> &current_data)
{
    vector<string> headers;
    pair<string, string> aux_pair;
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
    {
        cerr << endl << red << "Cannot connect to " << device_db << white << endl;
        return false;
    }
    
    //Extract info column by column
    string sql_cmd = select_row_by_code(table, code);
    int rc = sqlite3_prepare_v2(db, sql_cmd.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        cerr << endl <<  red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    }

    int step = sqlite3_step(stmt);
    int num_cols = sqlite3_column_count(stmt);    

    //Retrieve current data to be modified    
    for(int i = 1; i < num_cols; i++)
        headers.push_back(sqlite3_column_name(stmt, i));

    vector<string> string_values;
    int i;
    vector<string>::iterator h;
    string temp_string_value;
    for(h = headers.begin(), i = 1; h != headers.end() && i < num_cols; h++, i++)
    {
        sqlite3_value* value = sqlite3_column_value(stmt, i);        
        switch (sqlite3_value_type(value)) 
        {
            case SQLITE_TEXT:
                temp_string_value = string(reinterpret_cast<const char *>(sqlite3_value_text(value)));
                string_values.push_back(temp_string_value);
                aux_pair = make_pair(*h, temp_string_value);
                break;
        }
        current_data.push_back(aux_pair);
    }

    //Print current info of stored device
    unsigned int counter = 0;
    if(!show_device_by_parameter(device_db, table, CODE_PARAMETER, code, true, counter))
        return false;        
   
    sqlite3_finalize(stmt);
    sqlite3_close(db);    
    return true;
}


bool db_mgr::update_device(const char *device_db, const string& table, const string &code, vector<pair<string, string>> &new_data)
{   
    sqlite3 *db = nullptr;
    char *zErrMsg = 0;

    if(!is_file_present(DB_FILE_PATH, device_db))
        if(!create_file(DB_FILE_PATH, device_db))
            return false;

    //Connect to database
    db = database_connection(device_db);
    if(db == nullptr)
    {
        cerr << endl << red << "Cannot connect to " << device_db << white << endl;
        return false;
    }

    //Update
    string sql_cmd = update_row(table, code, new_data);
    int rc = sqlite3_exec(db, sql_cmd.c_str(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK)
    {        
        cerr << endl << red << "SQL error - " << zErrMsg << white << endl;
        sqlite3_free(zErrMsg);
        return false;
    } 

    sqlite3_close(db);
    return true;
}


/*******************************************************************************************************/
/*                                          TRANSFER section                                           */ 
/*******************************************************************************************************/
bool db_mgr::copy_db_to_cloud(void)
{
    bool ret = false;
    list<string> db_list;
    DIR *dir = opendir(DB_FILE_PATH);
    if(!dir)
    {
        cerr << endl << red << "Cannot open db file path..." << white << endl;        
        return ret;
    }
    else
    {
        dirent *entry;
        while((entry = readdir(dir)) != NULL) 
        {
            if(has_suffix(entry->d_name, ".db"))
                db_list.push_back(entry->d_name);
        }
        closedir(dir);
    }

    if(db_list.empty())
    {
        cout << endl << yellow << "No DB files to be transfered!" << white << endl;
        ret = true;
    }
    else
    {
        time_t t = time(NULL);
        tm *tPtr = localtime(&t);
        clock_t start, end;
        start = clock();

        string src_file;        
        string dest_file;
        string day = to_string(tPtr->tm_mday);
        string month = to_string((tPtr->tm_mon) + 1);
        string year = to_string((tPtr->tm_year) + 1900);            
        string hour = to_string(tPtr->tm_hour);            
        string minutes = to_string(tPtr->tm_min);
        string seconds = to_string(tPtr->tm_sec);
        string dest_folder = CLOUD_DB_PATH + day + "_" + month + "_" + year + "-" + hour + "_" + minutes + "_" + seconds + "/";

        //Create folder
        if (mkdir(dest_folder.c_str()) == -1)
        {
            cerr << endl << red << "Cannot create folder on cloud: " << strerror(errno) << white << endl;
            ret = false;
        }
        else
        {
            for(auto db_name : db_list)
            {
                src_file = DB_FILE_PATH + db_name;

                //Create files
                dest_file = dest_folder + db_name;

                ifstream src(src_file.c_str(), ios::binary);
                ofstream dest(dest_file.c_str(), ios::binary);

                dest << src.rdbuf();

                src.close();
                dest.close();
            }
        }
        end = clock();
        cout << endl << green << "Transfer completed in " << white << static_cast<double>(end - start) << green << " msec" << white << endl;
        ret = true;
    }



    return ret;
}
