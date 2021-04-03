#include "device_mgr.hpp"
#include "const.hpp"
#include "ConsoleColor.hpp"
#include "utility.hpp"
#include <string>
#include <sstream>
#include <fstream>


using namespace std;


device_mgr::device_mgr() :
    json_mgr(json_mgr),
    db_mgr(db_mgr)
{    
    
}


device_mgr::~device_mgr()
{

}


bool device_mgr::list_types(bool load_example)
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
    if(load_example)
    {
        while(getline(file, line))
            cout << white << "\t- " << blue << line.substr(0, line.find(" ")) << white << line.substr(line.find(" ")) << white << endl;
    }
    else
    {
        while(getline(file, line))
            cout << white << "\t- " << blue << line.substr(0, line.find(" ")) << white << endl;
    }
    file.close();
    return true;
}


/*******************************************************************************************************/
/*                                          ADD section                                                */ 
/*******************************************************************************************************/
bool device_mgr::add_device(const string &device, map<uint16_t, pair<string, string>> &meta_map)
{
    bool ret = false;        
    vector<tuple<string, string, any>>device_vector_tuple;

    if(device == RESISTOR)
        ret = json_mgr.load_device(RESISTOR_FILE, meta_map, device_vector_tuple);

    if(device == CAPACITOR)
        ret = json_mgr.load_device(CAPACITOR_FILE, meta_map, device_vector_tuple);

    if(device == INDUCTOR)
        ret = json_mgr.load_device(INDUCTOR_FILE, meta_map, device_vector_tuple);

    if(!ret)
        return false;

    print_device_tuple_vector(device_vector_tuple);

    if (is_validated() == NOT_CONFIRMED)
        return false;

    if(device == RESISTOR)
        ret = db_mgr.insert_device(RESISTOR, RESISTOR_DB, device_vector_tuple);

    if(device == CAPACITOR)
        ret = db_mgr.insert_device(CAPACITOR, CAPACITOR_DB, device_vector_tuple);

    if(device == INDUCTOR)
        ret = db_mgr.insert_device(INDUCTOR, INDUCTOR_DB, device_vector_tuple);

    return ret;
}


bool device_mgr::insert_mgr(void)
{
    bool ret = false;
    string device_type_in;
    vector<string> meta_structure;
    map<uint16_t, pair<string, string>> meta_map;

    if(!list_types(true))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(device_type_in == RESISTOR)
        if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
            ret = add_device(RESISTOR, meta_map);

    else if(device_type_in == CAPACITOR)
        if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
            ret = add_device(CAPACITOR, meta_map);

    else if(device_type_in == INDUCTOR)
        if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
            ret = add_device(INDUCTOR, meta_map);

    else
        cerr << red << "wrong choice..." << white << endl;

    return ret;
}


/*******************************************************************************************************/
/*                                          DELETE section                                             */ 
/*******************************************************************************************************/
bool device_mgr::delete_mgr(void)
{
    bool ret = false;
    string device_type_in;

    if(!list_types(false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(device_type_in == RESISTOR)
        ret = db_mgr.delete_device(RESISTOR, RESISTOR_DB);

    else if(device_type_in == CAPACITOR)
        ret = db_mgr.delete_device(CAPACITOR, CAPACITOR_DB);

    else if(device_type_in == INDUCTOR)
        ret = db_mgr.delete_device(INDUCTOR, INDUCTOR_DB);

    else
        cerr << red << "wrong choice..." << white << endl;
  
    return ret;
}


/*******************************************************************************************************/
/*                                          EDIT section                                               */ 
/*******************************************************************************************************/
bool device_mgr::edit_device(const string &device, map<uint16_t, pair<string, string>> &meta_map)
{
    bool ret = false;        
    vector<pair<string, string>>device_change_pairs;

    if(device == RESISTOR)
        ret = json_mgr.load_changes(RESISTOR_FILE, meta_map, device_change_pairs);

    if(device == CAPACITOR)
        ret = json_mgr.load_changes(CAPACITOR_FILE, meta_map, device_change_pairs);

    if(device == INDUCTOR)
        ret = json_mgr.load_changes(INDUCTOR_FILE, meta_map, device_change_pairs);

    if(!ret)
        return false;

    print_device_pair_vector(device_change_pairs);

    if (is_validated() == NOT_CONFIRMED)
        return false;

    if(device == RESISTOR)
        ret = db_mgr.update_device(RESISTOR, RESISTOR_DB, device_change_pairs);

    if(device == CAPACITOR)
        ret = db_mgr.update_device(CAPACITOR, CAPACITOR_DB, device_change_pairs);

    if(device == INDUCTOR)
        ret = db_mgr.update_device(INDUCTOR, INDUCTOR_DB, device_change_pairs);

    return ret;
}


bool device_mgr::edit_mgr(void)
{
    bool ret = false;
    string device_type_in;
    string code;
    vector<string> meta_structure;
    map<uint16_t, pair<string, string>> meta_map;

    if(!list_types(false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;

    if(device_type_in != RESISTOR && device_type_in != CAPACITOR && device_type_in != INDUCTOR)
    {
        cerr << red << "wrong choice..." << white << endl;
        return false;
    }        

    cout << endl << blue << "Insert device code" << white << endl;
    cout << endl << white << "in: " << white;    
    cin >> code;

    if(device_type_in == RESISTOR)
    {
        if(!db_mgr.select_device(code, RESISTOR_DB))
            cerr << red << "device not found..." << white << endl;
        else
        {   
            //db_mgr.show_device(code, RESISTOR_DB);
            if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
                ret = edit_device(RESISTOR, meta_map);
        }
    }
    else if(device_type_in == CAPACITOR)
    {
        if(!db_mgr.select_device(code, CAPACITOR_DB))
            cerr << red << "device not found..." << white << endl;
        else
        {
            //db_mgr.show_device(code, CAPACITOR);
            if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
                ret = edit_device(CAPACITOR, meta_map);
        }
    }    
    else if(device_type_in == INDUCTOR)
    {
        if(!db_mgr.select_device(code, INDUCTOR_DB))
            cerr << red << "device not found..." << white << endl;
        else
        {
            //db_mgr.show_device(code, INDUCTOR_DB);
            if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
                ret = edit_device(INDUCTOR, meta_map);
        }
    }

    return ret;
}
