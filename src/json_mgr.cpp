#include "json_mgr.hpp"
#include "const.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <dirent.h>

using namespace std;
using namespace rapidjson;


json_mgr::json_mgr()
{

}


json_mgr::~json_mgr()
{
    
}


bool json_mgr::get_device_info(const Value& resistor_info, map<string, string>& meta_map)
{
    for (Value::ConstMemberIterator  itr = resistor_info.MemberBegin(); itr != resistor_info.MemberEnd(); ++itr)
        meta_map[itr->name.GetString()] = itr->value.GetString();
    return meta_map.empty() ? false : true;
}


bool json_mgr::retrieve_device_metadata(const char *device, map<string, string>& meta_map)
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


bool json_mgr::is_file_present(const char *path, string filename)
{
    bool ret = false;
    DIR *dir; struct dirent *diread;
    vector<string> files;

    if ((dir = opendir(path)) != nullptr) 
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            if(diread->d_name[0] != '.' && diread->d_name[1] != '.')
                files.push_back(diread->d_name);
        }
        closedir (dir);
    } 
    else 
        return ret;

    for (auto file : files) 
    {        
        if(file == filename)
        {
            ret = true;
            break;
        }
    }
    return ret;
}   


bool json_mgr::delete_json(string filename)
{   
    bool ret = false; 
    string abs_filename = ROOT_FILE_PATH + filename;

    if(!is_file_present(ROOT_FILE_PATH, filename))
    {
        cerr << "Cannot delete " << filename << "because it does not exist" << endl;
        return ret;
    }

    if(remove(abs_filename.c_str()) == 0)
        ret = true;
    else
        cerr << "Cannot delete " << filename << endl;

    return ret;
}


bool json_mgr::create_new_json(string filename)
{         
    fstream file;
    file.open(ROOT_FILE_PATH + filename, ios::out); 

    if(!file)
    {
        cerr << "File " << filename << " has not been created..." << endl;
        return false;
    }

    file.close();
    return true;
}


bool json_mgr::create_resistor(map<string, string>& meta_map)
{
    if(!is_file_present(ROOT_FILE_PATH, RESISTOR_FILE))
    {
        if(!create_new_json(RESISTOR_FILE))
            return false;
    }

    
    return false;
}


bool json_mgr::create_capacitor(map<string, string>& meta_map)
{
    if(!is_file_present(ROOT_FILE_PATH, CAPACITOR_FILE))
    {
        if(!create_new_json(CAPACITOR_FILE))
            return false;
    }

    return false;
}


bool json_mgr::create_inductor(map<string, string>& meta_map)
{
    if(!is_file_present(ROOT_FILE_PATH, INDUCTOR_FILE))
    {
        if(!create_new_json(INDUCTOR_FILE))
            return false;
    }

    return false;
}


bool json_mgr::create_device(const char *device, map<string, string>& meta_map)
{
    if(string(device) == RESISTOR)
        return create_resistor(meta_map);
    if(string(device) == CAPACITOR)
        return create_capacitor(meta_map);
    if(string(device) == INDUCTOR)
        return create_inductor(meta_map);

    return false;
}