#include "sql_cmds.hpp"
#include <algorithm>


using namespace std;

string create_table(const string &device_name, vector<tuple<string, string, any>> &device_vector_tuple)
{
    unsigned int index = 0;
    string START;
    string COLUMN_NAME;
    string TYPE;
    string BODY;
    const string CONSTRAINT = "NOT NULL";
    const string SPACE = " ";
    const string COMMA = ", ";
    string END = ");";
    
    string ROOT = "CREATE TABLE IF NOT EXISTS " + device_name + "(ID INTEGER PRIMARY KEY AUTOINCREMENT, ";
    

    for (const auto& tuple_elem : device_vector_tuple)
    {
        ++index;
        string meta_name = get<0>(tuple_elem);
        string meta_type = get<1>(tuple_elem);
        any value = get<2>(tuple_elem);

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
            BODY += COLUMN_NAME + SPACE + TYPE + SPACE + CONSTRAINT + COMMA;
        else
            BODY += COLUMN_NAME + SPACE + TYPE + SPACE + CONSTRAINT;
    }
    
    return ROOT + BODY + END;
}