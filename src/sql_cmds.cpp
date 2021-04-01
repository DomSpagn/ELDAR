#include "sql_cmds.hpp"
#include <algorithm>
#include <iostream>


using namespace std;


string create_table(const string &device_name, vector<tuple<string, string, any>> &device_vector_tuple)
{
    unsigned int index = 0;
    string COLUMN_NAME;
    string TYPE;
    string BODY;    
    
    string ROOT = "CREATE TABLE IF NOT EXISTS " + device_name + "(ID INTEGER PRIMARY KEY AUTOINCREMENT, ";
    
    //Each BODY in this loop is filled with its name, type and constraints
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
            COLUMN_NAME = meta_name;
        }

        if (meta_type == "uint8" || meta_type == "uint16" || meta_type == "uint32" || meta_type == "uint64" || meta_type == "int8" || meta_type == "int16" || meta_type == "int32" || meta_type == "int64")
            TYPE = "INTEGER";
        else if(meta_type == "float" || meta_type == "double")
            TYPE = "REAL";
        else if(meta_type == "string")
            TYPE = "TEXT";
        
        if(index != device_vector_tuple.size())
        {
            if(COLUMN_NAME == "CODE")
                BODY += COLUMN_NAME + SPACE + TYPE + SPACE + NOT_NULL_CONSTRAINT + SPACE + UNIQUE_CONSTRAINT + COMMA + SPACE;
            else
                BODY += COLUMN_NAME + SPACE + TYPE + SPACE + NOT_NULL_CONSTRAINT + COMMA + SPACE;
        }
        else
        {
            if(COLUMN_NAME == "CODE")
                BODY += COLUMN_NAME + SPACE + TYPE + SPACE + NOT_NULL_CONSTRAINT + SPACE + UNIQUE_CONSTRAINT;
            else
                BODY += COLUMN_NAME + SPACE + TYPE + SPACE + NOT_NULL_CONSTRAINT;
        }
    }
    
    return ROOT + BODY + END;
}


string insert_row(const string &device_name, vector<tuple<string, string, any>> &device_vector_tuple)
{
    unsigned int index = 0;
    string BODY;
    string VALUE;
    vector<string>value_array;

    string ROOT = "INSERT INTO " + device_name + SPACE + "VALUES (NULL" + COMMA + SPACE;

    for (const auto& tuple_elem : device_vector_tuple)
    {
        ++index;
        string meta_name = get<0>(tuple_elem);
        string meta_type = get<1>(tuple_elem);
        any value = get<2>(tuple_elem);

        if(meta_name == "device")
            continue;

        if (meta_type == "uint8")
            VALUE = to_string(any_cast<uint8_t>(value));
        else if (meta_type == "uint16")
            VALUE = to_string(any_cast<uint16_t>(value));
        else if (meta_type == "uint32")
            VALUE = to_string(any_cast<uint32_t>(value));
        else if (meta_type == "uint64")
            VALUE = to_string(any_cast<uint64_t>(value));
        else if (meta_type == "int8")
            VALUE = to_string(any_cast<int8_t>(value));
        else if (meta_type == "int16")
            VALUE = to_string(any_cast<int16_t>(value));
        else if (meta_type == "int32")
            VALUE = to_string(any_cast<int32_t>(value));
        else if (meta_type == "int64")
            VALUE = to_string(any_cast<int64_t>(value));
        else if (meta_type == "float")
            VALUE = to_string(any_cast<float>(value));
        else if (meta_type == "double")
            VALUE = to_string(any_cast<double>(value));
        else
            VALUE = SINGLE_QUOTE + any_cast<string>(value) + SINGLE_QUOTE;

        value_array.push_back(VALUE);
    }
    
    //Build the BODY
    for (auto &val : value_array)    
    {
        if(&val == &value_array.back())
            BODY += val;
        else
            BODY += val + COMMA + SPACE;
    }
    
    return ROOT + BODY + END;
}