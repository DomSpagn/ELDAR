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
        if(device == DIODE)
            if(get_meta_info_from_json(device, meta_doc[i], meta_map))
                return true;
        if(device == LED)
            if(get_meta_info_from_json(device, meta_doc[i], meta_map))
                return true;
        if(device == BJT)
            if(get_meta_info_from_json(device, meta_doc[i], meta_map))
                return true;
    }

    cerr << endl << red << "Cannot retrieve device metadata" << white << endl;
    return false;
}


bool json_mgr::load_device_meta_info(map<uint16_t, pair<string, string>> &meta_map, vector<tuple<string, string, any>> &device_vector_tuple, const string &category)
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
    cout << endl << blue << "Insert the following data (press ENTER to leave an empty field):" << white << endl << endl;
    for(auto meta_elem : meta_map)
    {        
        if(meta_elem.second.first == "device")
        {
            aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, string());
            device_vector_tuple.push_back(aux_tuple);            
        }
        else if(meta_elem.second.first == "category")
        {
            aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, category);
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
                switch(check_uint8_validity(input[i], u8_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u8_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "uint16")
            {
                switch(check_uint16_validity(input[i], u16_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u16_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "uint32")
            {
                switch(check_uint32_validity(input[i], u32_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u32_value);                    
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "uint64")
            {
                switch(check_uint64_validity(input[i], u64_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, u64_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "int8")
            {        
                switch(check_int8_validity(input[i], i8_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i8_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "int16")
            {
                switch(check_int16_validity(input[i], i16_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i16_value);
                    break;

                    case SKIPPED:                        
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "int32")
            {
                switch(check_int32_validity(input[i], i32_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i32_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "int64")
            {
                switch(check_int64_validity(input[i], i64_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, i64_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }                
            }
            if(meta_elem.second.second == "float")
            {
                switch(check_float_validity(input[i], f_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, f_value);
                    break;

                    case SKIPPED:                        
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }
            }
            if(meta_elem.second.second == "double")
            {
                switch(check_double_validity(input[i], d_value))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, d_value);
                    break;

                    case SKIPPED:
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }                
            }
            if(meta_elem.second.second == "string")
            {
                switch(check_string_validity(input[i]))
                {
                    case NOT_VALID:
                        return false;
                    break;

                    case VALID:
                        aux_tuple = make_tuple(meta_elem.second.first, meta_elem.second.second, input[i]);
                    break;

                    case SKIPPED:
                        if(meta_elem.second.first == "code")
                        {
                            cerr << endl << red << "This field cannot be empty..." << white << endl;
                            return false;
                        }
                        aux_tuple = make_tuple(meta_elem.second.first, "string", string("-"));
                    break;
                }                
            }
            
            device_vector_tuple.push_back(aux_tuple);
            i++;
        }        
    }

    return true;
}


bool json_mgr::load_device(map<uint16_t, pair<string, string>> &meta_map, vector<tuple<string, string, any>> &device_vector_tuple, const string &category)
{
    if(!load_device_meta_info(meta_map, device_vector_tuple, category))
        return false;

    //Clean meta and device maps
    meta_map.clear();

    return true;
}