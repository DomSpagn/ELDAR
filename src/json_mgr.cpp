#include "json_mgr.hpp"
#include "const.hpp"
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


bool json_mgr::get_device_info(const Value &resistor_info, map<string, string> &meta_map)
{
    for (Value::ConstMemberIterator  itr = resistor_info.MemberBegin(); itr != resistor_info.MemberEnd(); ++itr)
        meta_map[itr->name.GetString()] = itr->value.GetString();
    return meta_map.empty() ? false : true;
}


bool json_mgr::retrieve_device_metadata(const char *device, map<string, string> &meta_map)
{   
    ifstream ifs(string(ROOT_FILE_PATH) + string(META_DEVICE_FILE));
    if(!ifs.is_open())
    {
        cerr << "Cannot open " << META_DEVICE_FILE << " for reading" << endl;
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

        if(string(device) == RESISTOR)
            return get_device_info(meta_doc[i], meta_map);
        if(string(device) == CAPACITOR)
            return get_device_info(meta_doc[i], meta_map);
        if(string(device) == INDUCTOR)
            return get_device_info(meta_doc[i], meta_map);
    }

    cerr << "Cannot retrieve device metadata" << endl;

    return false;
}


bool json_mgr::load_device_info(map<string, string> &meta_map)
{
    string input;
    for(auto info : meta_map)
    {
        cout << endl << info.first << ": ";        
        cin >> input;
        if(info.second == "uint8")
            if(!check_uint8_validity(input))
                return false;
        if(info.second == "uint16")
            if(!check_uint16_validity(input))
                return false;
        if(info.second == "uint32")
            if(!check_uint32_validity(input))
                return false;
        if(info.second == "uint64")
            if(!check_int64_validity(input))
                return false;
        if(info.second == "int8")
            if(!check_int8_validity(input))
                return false;
        if(info.second == "int16")
            if(!check_int16_validity(input))
                return false;
        if(info.second == "int32")
            if(!check_int32_validity(input))
                return false;
        if(info.second == "int64")
            if(!check_int64_validity(input))
                return false;
        if(info.second == "float")
            if(!check_float_validity(input))
                return false;
        if(info.second == "double")
            if(!check_float_validity(input))
                return false;
        if(info.second == "string")
            if(!check_string_validity(input))
                return false;
    }
    return false;
}


bool json_mgr::create_resistor(map<string, string> &meta_map)
{
    if(!is_file_present(ROOT_FILE_PATH, RESISTOR_FILE))
        if(!create_file(RESISTOR_FILE))
            return false;

    if(!load_device_info(meta_map))
    {
        cerr << "one of more inputs are unacceptable" << endl;
        return false;
    }
    
    //Bisogna aggiungere in append se il file già esiste

    return false;
}


bool json_mgr::create_capacitor(map<string, string> &meta_map)
{
    if(!is_file_present(ROOT_FILE_PATH, CAPACITOR_FILE))
    {
        if(!create_file(CAPACITOR_FILE))
            return false;
    }

    return false;
}


bool json_mgr::create_inductor(map<string, string> &meta_map)
{
    if(!is_file_present(ROOT_FILE_PATH, INDUCTOR_FILE))
    {
        if(!create_file(INDUCTOR_FILE))
            return false;
    }

    return false;
}


bool json_mgr::create_device(const char *device, map<string, string> &meta_map)
{
    if(string(device) == RESISTOR)
        return create_resistor(meta_map);
    if(string(device) == CAPACITOR)
        return create_capacitor(meta_map);
    if(string(device) == INDUCTOR)
        return create_inductor(meta_map);

    return false;
}