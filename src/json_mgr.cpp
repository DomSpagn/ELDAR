#include "json_mgr.hpp"
#include "const.hpp"
#include "console_color.hpp"
#include "utility.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


using namespace std;
using namespace rapidjson;


json_mgr::json_mgr()
{

}


json_mgr::~json_mgr()
{
    
}


bool json_mgr::get_meta_info_from_json(const string &device, const Value &device_info, map<uint16_t, pair<string, string>> &meta_map)
{
    uint8_t index = 0;
    pair<string, string> aux_pair;
    Value::ConstMemberIterator itr = device_info.MemberBegin();

    if(itr->value.GetString() != device)
        return false;

    for (itr = device_info.MemberBegin(); itr != device_info.MemberEnd(); ++itr)
    {        
        aux_pair = make_pair(itr->name.GetString(), itr->value.GetString());
        meta_map[index] = aux_pair;
        index++;
    }

    return meta_map.empty() ? false : true;
}


bool json_mgr::retrieve_device_metadata(const string &device, map<uint16_t, pair<string, string>> &meta_map)
{       
    ifstream ifs(string(JSON_FILE_PATH) + string(META_DEVICE_FILE));
    if(!ifs.is_open())
    {
        cerr << endl << red << "Cannot open " << META_DEVICE_FILE << " for reading" << white << endl;
        return false;
    }        

    stringstream metadata_json;
    metadata_json << ifs.rdbuf();
    ifs.close();

    Document meta_doc;
    meta_doc.Parse(metadata_json.str().c_str());
    assert(meta_doc.IsArray());
            
    for (SizeType i = 0; i < meta_doc.Size(); i++)
    {
        assert(meta_doc[i].IsObject());

        if(device == RESISTOR)
            if(get_meta_info_from_json(device, meta_doc[i], meta_map))
                return true;
        if(device == CAPACITOR)
            if(get_meta_info_from_json(device, meta_doc[i], meta_map))
                return true;
        if(device == INDUCTOR)
            if(get_meta_info_from_json(device, meta_doc[i], meta_map))
                return true;
    }

    cerr << endl << red << "Cannot retrieve device metadata" << white << endl;
    return false;
}


bool json_mgr::load_device_meta_info(map<uint16_t, pair<string, string>> &meta_map, vector<tuple<string, string, any>> &device_vector_tuple)
{
    string input[meta_map.size()];
    unsigned int i = 0;
    tuple<string, string, any> aux_tuple;
    
    uint8_t u8_value;
    uint16_t u16_value;
    uint32_t u32_value;
    uint64_t u64_value;
    int8_t i8_value;
    int16_t i16_value;
    int32_t i32_value;
    int64_t i64_value;
    float f_value;
    double d_value;    

    cin.ignore();
    cout << endl << blue << "Insert the following data (" << meta_map.size() - 1 << " data):" << white << endl << endl;
    for(auto meta_elem : meta_map)
    {        
        if(meta_elem.second.first == "device")
        {
            aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, string());
            device_vector_tuple.push_back(aux_tuple);            
        }
        else
        {            
            cout << meta_elem.first << ") " << blue << meta_elem.second.first << ": " << white;            
            getline(cin, input[i]);

            if(!check_input_validity(input[i], SIMPLE_ALPHA))
            {
                cerr << endl << red << "Input not allowed..." << white << endl;
                return false; 
            }

            if(meta_elem.second.second == "uint8")
            {
                if(!check_uint8_validity(input[i], u8_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u8_value);
            }
            if(meta_elem.second.second == "uint16")
            {
                if(!check_uint16_validity(input[i], u16_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u16_value);
            }
            if(meta_elem.second.second == "uint32")
            {
                if(!check_uint32_validity(input[i], u32_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u32_value);                    
            }
            if(meta_elem.second.second == "uint64")
            {
                if(!check_uint64_validity(input[i], u64_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u64_value);
            }
            if(meta_elem.second.second == "int8")
            {        
                if(!check_int8_validity(input[i], i8_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i8_value);
            }
            if(meta_elem.second.second == "int16")
            {
                if(!check_int16_validity(input[i], i16_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i16_value);
            }
            if(meta_elem.second.second == "int32")
            {
                if(!check_int32_validity(input[i], i32_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i32_value);
            }
            if(meta_elem.second.second == "int64")
            {
                if(!check_int64_validity(input[i], i64_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i64_value);
            }
            if(meta_elem.second.second == "float")
            {
                if(!check_float_validity(input[i], f_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, f_value);
            }
            if(meta_elem.second.second == "double")
            {
                if(!check_double_validity(input[i], d_value))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, d_value);
            }
            if(meta_elem.second.second == "string")
            {
                if(!check_string_validity(input[i]))
                    return false;
                aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, input[i]);
            }
            
            device_vector_tuple.push_back(aux_tuple);
            i++;
        }        
    }

    return true;
}


bool json_mgr::load_device(map<uint16_t, pair<string, string>> &meta_map, vector<tuple<string, string, any>> &device_vector_tuple)
{
    if(!load_device_meta_info(meta_map, device_vector_tuple))
        return false;

    //Clean meta and device maps
    meta_map.clear();

    return true;
}