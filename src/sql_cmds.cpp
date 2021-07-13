#include "sql_cmds.hpp"
#include <algorithm>
#include <iostream>
#include "console_color.hpp"
#include "const.hpp"

using namespace std;


/*******************************************************************************************************/
/*                                          CREATE query                                               */ 
/*******************************************************************************************************/
string create_table(const string &table, vector<pair<string, string>> &device_info)
{
    unsigned int index = 0;
    string column_name;
    string type;
    string body;    
    
    string root = "CREATE TABLE IF NOT EXISTS " + table + "(ID INTEGER PRIMARY KEY AUTOINCREMENT, ";
    
    //Each body in this loop is filled with its name, type and constraints
    for (const auto& elem_pair : device_info)
    {
        ++index;
        string meta_name = elem_pair.first;        

        transform(meta_name.begin(), meta_name.end(), meta_name.begin(), ::toupper);            
        column_name = meta_name;
       
        if(index != device_info.size())
        {
            if(column_name == CODE_PARAMETER)
                body += column_name + SPACE + TEXT + SPACE + NOT_NULL_CONSTRAINT + SPACE + UNIQUE_CONSTRAINT + COMMA + SPACE;
            else
                body += column_name + SPACE + TEXT + SPACE + NOT_NULL_CONSTRAINT + COMMA + SPACE;
        }
        else
        {
            if(column_name == CODE_PARAMETER)
                body += column_name + SPACE + TEXT + SPACE + NOT_NULL_CONSTRAINT + SPACE + UNIQUE_CONSTRAINT;
            else
                body += column_name + SPACE + TEXT + SPACE + NOT_NULL_CONSTRAINT;
        }
    }
    
    return root + body + END;
}


/*******************************************************************************************************/
/*                                          INSERT query                                               */ 
/*******************************************************************************************************/
string insert_row(const string &table, vector<pair<string, string>> &device_info)
{
    unsigned int index = 0;
    string body;
    string value;
    vector<string>value_array;

    string root = "INSERT INTO " + table + SPACE + "VALUES (NULL" + COMMA + SPACE;

    for (const auto &elem_pair : device_info)
    {
        ++index;
        string meta_name = elem_pair.first;
        string value_in = elem_pair.second;

        value = SINGLE_QUOTE + value_in + SINGLE_QUOTE;
        value_array.push_back(value);
    }
    
    //Build the body
    for (auto &val : value_array)    
    {
        if(&val == &value_array.back())
            body += val;
        else
            body += val + COMMA + SPACE;
    }
    
    return root + body + END;
}


/*******************************************************************************************************/
/*                                          SELECT query                                               */ 
/*******************************************************************************************************/
string select_row_by_code(const string &table, const string &code)
{    
    string root = "SELECT * FROM " + table + SPACE + "WHERE CODE" + EQUAL + SPACE;
    string body = SINGLE_QUOTE + code + SINGLE_QUOTE;

    return root + body;
}


bool show_sql_table(const char *device_db, const string &table)
{
    string std_out_line;
    array<char, 128> buffer;   

    string cmd = "sqlite3" + SPACE + DB_FILE_PATH + device_db + SPACE + "-cmd" + SPACE + "\".header on\"" + SPACE + "\".mode column\"" + SPACE + "\"SELECT * FROM" + SPACE + table + DOUBLE_QUOTE;

    FILE *fp = popen(cmd.c_str(), "r");
    if(fp == NULL)
    {
        cerr << endl << red << "Cannot read" + SPACE + table << white << endl;
        return false;
    }

    while (fgets(buffer.data(), 128, fp) != NULL) 
        std_out_line += buffer.data();

    if(pclose(fp) < 0)    
    {
        cerr << endl << red << "popen error has been detected" << white << endl;
        return false;
    }    

    string aux_table = table;
    transform(aux_table.begin(), aux_table.end(), aux_table.begin(), ::toupper);
    cout << endl << endl << blue << aux_table << endl << green << std_out_line << endl;
    return true;
}


bool show_sql_device(const char *device_db, const string &table, const std::string &parameter, const string &parameter_value, bool exact, unsigned int &counter)
{
    string std_out_line;
    array<char, 128> buffer;
    string cmd;
    
    if(exact)
        cmd = "sqlite3" + SPACE + DB_FILE_PATH + device_db + SPACE + "-cmd" + SPACE + "\".header on\"" + SPACE + "\".mode column\"" + SPACE + "\"SELECT * FROM" + SPACE + table + SPACE + "WHERE" + SPACE + parameter + SPACE + EQUAL + SPACE + SINGLE_QUOTE + parameter_value + SINGLE_QUOTE;
    else
        cmd = "sqlite3" + SPACE + DB_FILE_PATH + device_db + SPACE + "-cmd" + SPACE + "\".header on\"" + SPACE + "\".mode column\"" + SPACE + "\"SELECT * FROM" + SPACE + table + SPACE + "WHERE" + SPACE + parameter + SPACE + LIKE + SPACE + SINGLE_QUOTE + PERCENTAGE + parameter_value + PERCENTAGE + SINGLE_QUOTE;

    FILE *fp = popen(cmd.c_str(), "r");
    if(fp == NULL)
    {
        cerr << endl << red << "Cannot read" + SPACE + table << white << endl;
        return false;
    }

    while (fgets(buffer.data(), 128, fp) != NULL) 
        std_out_line += buffer.data();

    if(pclose(fp) < 0)    
    {
        cerr << endl << red << "popen error has been detected" << white << endl;
        return false;
    }

    if(!std_out_line.empty())
    {
        counter++;
        string aux_table = table;
        transform(aux_table.begin(), aux_table.end(), aux_table.begin(), ::toupper);
        cout << endl << endl << green << blue << aux_table << endl << green << std_out_line << endl;
    }
    return true;
}


string table_size(const char *device_db, const string &table)
{
    string root = "SELECT COUNT(*) FROM " + table;    
    return root;
}


bool get_table_from_db(const char *db, string &table_name)
{
    array<char, 128> buffer;

    string cmd = "sqlite3" + SPACE + DB_FILE_PATH + db + SPACE + "\".tables\"";
    FILE *fp = popen(cmd.c_str(), "r");
    if(fp == NULL)
    {
        cerr << endl << red << "Cannot read" + SPACE + db << white << endl;
        return false;
    }

    if(fgets(buffer.data(), 128, fp) != NULL) 
        table_name += buffer.data();

    table_name.erase(remove(table_name.begin(), table_name.end(), '\n'), table_name.end());

    if(pclose(fp) < 0)    
    {
        cerr << endl << red << "popen error has been detected" << white << endl;
        return false;
    }
    return true;    
}


/*******************************************************************************************************/
/*                                          DELETE query                                               */ 
/*******************************************************************************************************/
string delete_row(const string &table, const string &code)
{    
    string root = "DELETE FROM " + table + SPACE + "WHERE CODE" + EQUAL;
    string body = SINGLE_QUOTE + code + SINGLE_QUOTE;

    return root + body;
}


/*******************************************************************************************************/
/*                                          UPDATE query                                               */ 
/*******************************************************************************************************/
string update_row(const string &table, const string& code, vector<pair<string, string>> &new_data)
{    
    string root = "UPDATE" + SPACE + table + SPACE + "SET" + SPACE;
    string body;
    string value;

    for(auto updated_value = new_data.begin(); updated_value != new_data.end(); ++updated_value)
    {
        string column_name = updated_value->first;        
        string data_value = updated_value->second;

        value = SINGLE_QUOTE + data_value + SINGLE_QUOTE;

        if(next(updated_value) == new_data.end())        
            body += column_name + EQUAL + value + SPACE;
        else            
            body += column_name + EQUAL + value + COMMA + SPACE;  
    }
    string condition = "WHERE" + SPACE + CODE_PARAMETER + EQUAL + SINGLE_QUOTE + code + SINGLE_QUOTE;

    return root + body + condition;
}