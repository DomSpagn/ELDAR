#include "sql_cmds.hpp"
#include <algorithm>
#include <iostream>


using namespace std;


/*******************************************************************************************************/
/*                                          CREATE query                                               */ 
/*******************************************************************************************************/
string create_table(const string &table, vector<tuple<string, string, any>> &device_vector_tuple)
{
    unsigned int index = 0;
    string column_name;
    string type;
    string body;    
    
    string root = "CREATE TABLE IF NOT EXISTS " + table + "(ID INTEGER PRIMARY KEY AUTOINCREMENT, ";
    
    //Each body in this loop is filled with its name, type and constraints
    for (const auto& tuple_elem : device_vector_tuple)
    {
        ++index;
        string meta_name = get<0>(tuple_elem);
        string meta_type = get<1>(tuple_elem);

        if(meta_name == "device")
            continue;
        else
        {
            transform(meta_name.begin(), meta_name.end(), meta_name.begin(), ::toupper);            
            column_name = meta_name;
        }

        if (meta_type == "uint8" || meta_type == "uint16" || meta_type == "uint32" || meta_type == "uint64" || meta_type == "int8" || meta_type == "int16" || meta_type == "int32" || meta_type == "int64")
            type = "INTEGER";
        else if(meta_type == "float" || meta_type == "double")
            type = "REAL";
        else if(meta_type == "string")
            type = "TEXT";
        
        if(index != device_vector_tuple.size())
        {
            if(column_name == "CODE")
                body += column_name + SPACE + type + SPACE + NOT_NULL_CONSTRAINT + SPACE + UNIQUE_CONSTRAINT + COMMA + SPACE;
            else
                body += column_name + SPACE + type + SPACE + NOT_NULL_CONSTRAINT + COMMA + SPACE;
        }
        else
        {
            if(column_name == "CODE")
                body += column_name + SPACE + type + SPACE + NOT_NULL_CONSTRAINT + SPACE + UNIQUE_CONSTRAINT;
            else
                body += column_name + SPACE + type + SPACE + NOT_NULL_CONSTRAINT;
        }
    }
    
    return root + body + END;
}


/*******************************************************************************************************/
/*                                          INSERT query                                               */ 
/*******************************************************************************************************/
string insert_row(const string &table, vector<tuple<string, string, any>> &device_vector_tuple)
{
    unsigned int index = 0;
    string body;
    string value;
    vector<string>value_array;

    string root = "INSERT INTO " + table + SPACE + "VALUES (NULL" + COMMA + SPACE;

    for (const auto& tuple_elem : device_vector_tuple)
    {
        ++index;
        string meta_name = get<0>(tuple_elem);
        string meta_type = get<1>(tuple_elem);
        any value_in = get<2>(tuple_elem);

        if(meta_name == "device")
            continue;

        if (meta_type == "uint8")
            value = to_string(any_cast<uint8_t>(value_in));
        else if (meta_type == "uint16")
            value = to_string(any_cast<uint16_t>(value_in));
        else if (meta_type == "uint32")
            value = to_string(any_cast<uint32_t>(value_in));
        else if (meta_type == "uint64")
            value = to_string(any_cast<uint64_t>(value_in));
        else if (meta_type == "int8")
            value = to_string(any_cast<int8_t>(value_in));
        else if (meta_type == "int16")
            value = to_string(any_cast<int16_t>(value_in));
        else if (meta_type == "int32")
            value = to_string(any_cast<int32_t>(value_in));
        else if (meta_type == "int64")
            value = to_string(any_cast<int64_t>(value_in));
        else if (meta_type == "float")
            value = to_string(any_cast<float>(value_in));
        else if (meta_type == "double")
            value = to_string(any_cast<double>(value_in));
        else
            value = SINGLE_QUOTE + any_cast<string>(value_in) + SINGLE_QUOTE;

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
std::string select_row_by_code(const string &table, const string &code)
{    
    string root = "SELECT * FROM " + table + SPACE + "WHERE CODE LIKE" + SPACE;
    string body = SINGLE_QUOTE + code + SINGLE_QUOTE;

    return root + body;
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
string update_row(const string &table, const string& code, vector<tuple<string, string, any>> &new_data)
{    
    string root = "UPDATE" + SPACE + table + SPACE + "SET" + SPACE;
    string body;
    string value;

    for(auto &updated_value : new_data)
    {
        string column_name = get<0>(updated_value);
        string data_type = get<1>(updated_value);
        any data_value = get<2>(updated_value);

        if (data_type == "integer")
            value = to_string(any_cast<int64_t>(data_value));
        else if (data_type == "real")
            value = to_string(any_cast<double>(data_value));
        else
            value = SINGLE_QUOTE + any_cast<string>(data_value) + SINGLE_QUOTE;

        if(&updated_value == &new_data.back())        
            body += column_name + EQUAL + value + SPACE;
        else            
            body += column_name + EQUAL + value + COMMA + SPACE;  
    }
    string condition = "WHERE" + SPACE + "CODE" + EQUAL + SINGLE_QUOTE + code + SINGLE_QUOTE;

    return root + body + condition;
}
