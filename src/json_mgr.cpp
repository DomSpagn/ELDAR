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


bool json_mgr::get_meta_info_from_json(const Value &device_info, map<uint16_t, pair<string, string>> &meta_map)
{
    uint8_t index = 0;
    pair<string, string> aux_pair;

    for (Value::ConstMemberIterator itr = device_info.MemberBegin(); itr != device_info.MemberEnd(); ++itr)
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
        if(meta_doc[i].HasMember(device.c_str()))
        {            
            if(get_meta_info_from_json(meta_doc[i][device.c_str()], meta_map))
                return true;
        }
    }

    cerr << endl << red << "Cannot retrieve device metadata" << white << endl;
    return false;
}


bool json_mgr::load_device_meta_info(map<uint16_t, pair<string, string>> &meta_map, vector<pair<string, string>> &input_data)
{
    string input[meta_map.size()];
    unsigned int i = 0;

    cin.ignore();
    cout << endl << blue << "Insert the following data (press ENTER to skip):" << white << endl << endl;
    for(auto meta_elem : meta_map)
    {        
        if(meta_elem.second.first == "category")
            input_data.push_back(make_pair(meta_elem.second.first, meta_elem.second.second));            
        else
        {            
            cout << meta_elem.first << ") " << blue << meta_elem.second.first << ": " << white;            
            getline(cin, input[i]);
            std::pair<string, string> aux_pair = make_pair(meta_elem.second.first, meta_elem.second.second);
            if(!is_input_data_correct(aux_pair, input[i], input_data))
                return false;
            i++;
        }        
    }
    return true;
}


bool json_mgr::load_device(map<uint16_t, pair<string, string>> &meta_map, vector<pair<string, string>> &input_data)
{
    if(!load_device_meta_info(meta_map, input_data))
        return false;

    //Clean meta and device maps
    meta_map.clear();

    return true;
}