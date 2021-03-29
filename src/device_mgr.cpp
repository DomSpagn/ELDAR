#include "device_mgr.hpp"
#include "const.hpp"
#include "ConsoleColor.hpp"
#include "utility.hpp"
#include <string>
#include <sstream>
#include <fstream>


using namespace std;


device_mgr::device_mgr() :
    json_mgr(json_mgr)
{    
    
}


device_mgr::~device_mgr()
{

}


bool device_mgr::list_types(void)
{
    if(!is_file_present(TXT_FILE_PATH, DEVICE_TYPE_FILE))
        return false;

    cout << blue << "Select component type among: " << white << endl << endl;
    fstream file;
    file.open(string(TXT_FILE_PATH) + string(DEVICE_TYPE_FILE), ios::in); 

    if(!file)
    {
        cerr << red << "File " << DEVICE_TYPE_FILE << " cannot be opened!" << white << endl;
        return false;
    }

    //Read line by line
    string line;
    while(getline(file, line))
        cout << white << "\t- " << blue << line.substr(0, line.find(" ")) << white << line.substr(line.find(" ")) << white << endl;

    file.close();
    return true;
}


bool device_mgr::insert_mgr(void)
{
    bool ret = false;
    string device_type_in;
    vector<string> meta_structure;
    map<uint16_t, pair<string, string>> meta_map;

    if(!list_types())
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(device_type_in == RESISTOR)
    {
        if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
        {
            ret = json_mgr.add_device(RESISTOR, meta_map);
        }
    }
    else if(device_type_in == CAPACITOR)
    {
        if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
        {
            ret = json_mgr.add_device(CAPACITOR, meta_map);
        }
    }    
    else if(device_type_in == INDUCTOR)
    {
        if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
        {
            ret = json_mgr.add_device(INDUCTOR, meta_map);
        }
    }
    else
        cerr << red << "wrong choice..." << white << endl;

    return ret;
}