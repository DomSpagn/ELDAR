#include "device_mgr.hpp"
#include "const.hpp"
#include "console_color.hpp"
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


/*******************************************************************************************************/
/*                                          ADD section                                                */ 
/*******************************************************************************************************/
bool device_mgr::add_device(const string &device, map<uint16_t, pair<string, string>> &meta_map)
{
    bool ret = false;        
    vector<tuple<string, string, any>>device_vector_tuple;

    if(device == RESISTOR)
        ret = json_mgr.load_device(meta_map, device_vector_tuple);

    if(device == CAPACITOR)
        ret = json_mgr.load_device(meta_map, device_vector_tuple);

    if(device == INDUCTOR)
        ret = json_mgr.load_device(meta_map, device_vector_tuple);

    if(!ret)
        return false;

    print_device_tuple_vector(device_vector_tuple, INSERT);

    if (is_validated() == NOT_CONFIRMED)
        return false;

    if(device == RESISTOR)
        ret = db_mgr.insert_device(RESISTOR_DB, RESISTOR, device_vector_tuple);

    if(device == CAPACITOR)
        ret = db_mgr.insert_device(CAPACITOR_DB, CAPACITOR, device_vector_tuple);

    if(device == INDUCTOR)
        ret = db_mgr.insert_device(INDUCTOR_DB, INDUCTOR, device_vector_tuple);

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
    {
        if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
            ret = add_device(RESISTOR, meta_map);
    }
    else if(device_type_in == CAPACITOR)
    {
        if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
            ret = add_device(CAPACITOR, meta_map);
    }
    else if(device_type_in == INDUCTOR)
    {
        if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
            ret = add_device(INDUCTOR, meta_map);
    }
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
        ret = db_mgr.delete_device(RESISTOR_DB, RESISTOR);

    else if(device_type_in == CAPACITOR)
        ret = db_mgr.delete_device(CAPACITOR_DB, CAPACITOR);

    else if(device_type_in == INDUCTOR)
        ret = db_mgr.delete_device(INDUCTOR_DB, INDUCTOR);

    else
        cerr << red << "wrong choice..." << white << endl;
  
    return ret;
}


/*******************************************************************************************************/
/*                                          EDIT section                                               */ 
/*******************************************************************************************************/
bool device_mgr::load_changes(vector<tuple<string, string, any>> &current_data, vector<tuple<string, string, any>> &new_data)
{
    cout << blue << "Insert new values or press ENTER to skip:" << white << endl;    
    string input[current_data.size()];
    unsigned int i = 0;
    tuple<string, string, any> aux_tuple;

    int64_t int_value;
    double real_value;    

    cin.ignore();
    for(auto data : current_data)
    {
        string key = get<0>(data);
        string type = get<1>(data);
        any value = get<2>(data);

        cout << white << key << " (" << yellow << type << white << "): "; 
        getline(cin, input[i]);        

        if(input[i].length() == 0)
            continue;
        else if(type == "integer" && check_int64_validity(input[i], int_value))
            aux_tuple = make_tuple(key, type, int_value);
        else if(type == "real" && check_double_validity(input[i], real_value))
            aux_tuple = make_tuple(key, type, real_value);
        else if(type == "text" && check_string_validity(input[i]))
            aux_tuple = make_tuple(key, type, input[i]);
        else
            return false;

        new_data.push_back(aux_tuple);
        i++;
    }

    return true;
}


bool device_mgr::edit_device(const string &device, const string &code)
{
    bool ret = false;        
    vector<tuple<string, string, any>> current_data;
    vector<tuple<string, string, any>> new_data;

    if(device == RESISTOR)
        ret = db_mgr.retrieve_current_device_data(RESISTOR_DB, RESISTOR, code, current_data);

    else if(device == CAPACITOR)
        ret = db_mgr.retrieve_current_device_data(CAPACITOR_DB, CAPACITOR, code, current_data);

    else if(device == INDUCTOR)
        ret = db_mgr.retrieve_current_device_data(INDUCTOR_DB, INDUCTOR, code, current_data);

    if(!ret)
        return false;

    if(!load_changes(current_data, new_data))
        return false;
    
    if(new_data.size() == 0)
    {
        cout << endl << yellow << "No data has been modified" << white << endl;
        return true;
    }

    print_device_tuple_vector(new_data, UPDATE);

    if (is_validated() == NOT_CONFIRMED)
        return false;

    if(device == RESISTOR)
        ret = db_mgr.update_device(RESISTOR_DB, RESISTOR, code, new_data);

    if(device == CAPACITOR)
        ret = db_mgr.update_device(CAPACITOR_DB, CAPACITOR, code, new_data);

    if(device == INDUCTOR)
        ret = db_mgr.update_device(INDUCTOR_DB, INDUCTOR, code, new_data);    

    return ret;
}


bool device_mgr::edit_mgr(void)
{
    bool ret = false;
    string device_type_in;
    string code_in;
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

    cout << endl << blue << "Insert device code to see the current info" << white << endl;
    cout << endl << white << "in: " << white;    
    cin >> code_in;

    if(device_type_in == RESISTOR)
    {        
        if(db_mgr.select_device(RESISTOR_DB, RESISTOR, code_in))
        {   
            if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
                ret = edit_device(RESISTOR, code_in);
        }
    }
    else if(device_type_in == CAPACITOR)
    {
        if(db_mgr.select_device(CAPACITOR_DB, CAPACITOR, code_in))
        {
            if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
                ret = edit_device(CAPACITOR, code_in);
        }
    }    
    else if(device_type_in == INDUCTOR)
    {
        if(db_mgr.select_device(INDUCTOR_DB, INDUCTOR, code_in))
        {
            if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
                ret = edit_device(INDUCTOR, code_in);
        }
    }

    return ret;
}


/*******************************************************************************************************/
/*                                          SEARCH section                                             */ 
/*******************************************************************************************************/
bool device_mgr::search_by_type(void)
{
    bool ret = false;
    string device_type_in;
    cout << endl;
    cout << blue << "Choose device type among: " << endl << endl;

    if(!list_types(false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(device_type_in == RESISTOR)
        ret = db_mgr.show_table(RESISTOR_DB, RESISTOR);
    else if(device_type_in == CAPACITOR)
        ret = db_mgr.show_table(CAPACITOR_DB, CAPACITOR);
    else if(device_type_in == INDUCTOR)
        ret = db_mgr.show_table(INDUCTOR_DB, INDUCTOR);
    else
        cerr << red << "wrong choice..." << white << endl;

    return ret;
}


bool device_mgr::search_by_code(void)
{
    bool ret = false;
    string code_in;
    cout << endl;
    cout << blue << "Digit code: " << endl << endl;

    cout << endl << white << "in: " << white;
    cin >> code_in;    

    return ret;
}


bool device_mgr::search_by_manufacturer(void)
{
    bool ret = false;
    string manufacturer_in;
    cout << endl;
    cout << blue << "Digit manufacturer: " << endl << endl;

    cout << endl << white << "in: " << white;
    cin >> manufacturer_in;    

    return ret;
}


bool device_mgr::search_by_mounting_type(void)
{
    bool ret = false;
    string mounting_type_in;
    cout << endl;
    cout << blue << "Digit mounting type: " << endl << endl;

    cout << endl << white << "in: " << white;
    cin >> mounting_type_in;    

    return ret;
}

bool device_mgr::search_mgr(void)
{
    bool ret = false;
    string selection;

    cout << endl;
    cout << blue << "Digit related number to search by: " << endl << endl;
    cout << blue << "\t1) " << white << "type" << endl;
    cout << blue << "\t2) " << white << "code" << endl;
    cout << blue << "\t3) " << white << "manufacturer" << endl;
    cout << blue << "\t4) " << white << "mounting type" << endl;    

    cout << endl << white << "in: ";
    cin >> selection;
    if(selection.size() > 1)
    {
        cerr << red << "Wrong input..." << white << endl;
        return false;
    }        
    cout << endl;
    
    try
    {
        switch(stoul(selection, nullptr, 10))
        {
            case 1:
                ret = search_by_type();
            break;

            case 2:
                ret = search_by_code();
            break;

            case 3:                
                ret = search_by_manufacturer();
            break;

            case 4:                
                ret = search_by_mounting_type();
            break;

            default:
                cerr << red << "Wrong input..." << white << endl;
                ret = false;
        }
    }
    catch(...)
    {
        cerr << red << "Wrong input..." << white << endl;
        ret = false;
    }

    return ret;
}