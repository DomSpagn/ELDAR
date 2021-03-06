#include "device_mgr.hpp"
#include "const.hpp"
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
    if(!json_mgr.is_file_present(ROOT_FILE_PATH, DEVICE_TYPE_FILE))
    {
        cerr << DEVICE_TYPE_FILE << " is not present!" << endl;    
        return false;
    }

    cout << "Select type among: " << endl << endl;
    fstream file;
    file.open(string(ROOT_FILE_PATH) + string(DEVICE_TYPE_FILE), ios::in); 

    if(!file)
    {
        cerr << "File " << DEVICE_TYPE_FILE << " cannot be opened!" << endl;
        return false;
    }

    //Read line by line
    string line;
    while(getline(file, line))
        cout << "   - " << line << endl;

    return true;
}


bool device_mgr::insert_device(void)
{
    string device_type_in;
    vector<string> meta_structure;
    map<string, string> meta_map;

    if(!list_types())
        return false;

    cout << endl << "type: ";
    cin >> device_type_in;    

    if(device_type_in == RESISTOR)
    {
        if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
        {
            json_mgr.create_device(RESISTOR, meta_map);
        }
    }
    else if(device_type_in == CAPACITOR)
    {
        if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
        {
            json_mgr.create_device(RESISTOR, meta_map);
        }
    }    
    else if(device_type_in == INDUCTOR)
    {
        if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
        {
            json_mgr.create_device(RESISTOR, meta_map);
        }
    }
    else
        cerr << "wrong choice..." << endl;

    return false;
}