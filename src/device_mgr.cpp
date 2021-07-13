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
    vector<pair<string, string>> input_data;

    if(!json_mgr.load_device(meta_map, input_data))
        return false;

    print_device_info_to_be_confirmed(input_data, INSERT);

    if (is_validated() == NOT_CONFIRMED)
        return false;

    if(device == RESISTOR)
        ret = db_mgr.insert_device(RESISTOR_DB, RESISTOR, input_data);
    if(device == CAPACITOR)
        ret = db_mgr.insert_device(CAPACITOR_DB, CAPACITOR, input_data);
    if(device == INDUCTOR)
        ret = db_mgr.insert_device(INDUCTOR_DB, INDUCTOR, input_data);
    if(device == DIODE)
        ret = db_mgr.insert_device(DIODE_DB, DIODE, input_data);
    if(device == LED)
        ret = db_mgr.insert_device(LED_DB, LED, input_data);
    if(device == BJT)
        ret = db_mgr.insert_device(BJT_DB, BJT, input_data);
    if(device == MOSFET)
        ret = db_mgr.insert_device(MOSFET_DB, MOSFET, input_data);

    return ret;
}


bool device_mgr::insert_mgr(void)
{
    bool ret = false;
    list<string> device_list;
    string device_type_in;
    vector<string> meta_structure;
    map<uint16_t, pair<string, string>> meta_map;

    if(!list_devices(device_list, true))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(!check_range_validity(device_type_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    if(find(begin(device_list), end(device_list), device_type_in) != end(device_list))
    {
        if(json_mgr.retrieve_device_metadata(device_type_in, meta_map))
            ret = add_device(device_type_in, meta_map);
    }
    else
        cerr << endl << red << "Wrong choice..." << white << endl;

    return ret;
}


/*******************************************************************************************************/
/*                                          DELETE section                                             */ 
/*******************************************************************************************************/
bool device_mgr::delete_mgr(void)
{
    bool ret = false;
    list<string> device_list;
    string device_type_in;

    if(!list_devices(device_list, false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(!check_range_validity(device_type_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    if(device_type_in == RESISTOR)
        ret = db_mgr.delete_device(RESISTOR_DB, RESISTOR);
    else if(device_type_in == CAPACITOR)
        ret = db_mgr.delete_device(CAPACITOR_DB, CAPACITOR);
    else if(device_type_in == INDUCTOR)
        ret = db_mgr.delete_device(INDUCTOR_DB, INDUCTOR);
    else if(device_type_in == DIODE)
        ret = db_mgr.delete_device(DIODE_DB, DIODE);
    else if(device_type_in == LED)
        ret = db_mgr.delete_device(LED_DB, LED);
    else if(device_type_in == BJT)
        ret = db_mgr.delete_device(BJT_DB, BJT);
    else if(device_type_in == MOSFET)
        ret = db_mgr.delete_device(MOSFET_DB, MOSFET);
    else
        cerr << endl << red << "Wrong choice..." << white << endl;
  
    return ret;
}


/*******************************************************************************************************/
/*                                          EDIT section                                               */ 
/*******************************************************************************************************/
bool device_mgr::load_changes(vector<pair<string, string>> &current_info, vector<pair<string, string>> &new_data)
{
    cout << endl << blue << "Insert new values (press ENTER to skip):" << white << endl;    
    string input[current_info.size()];
    unsigned int i = 0;

    cin.ignore();
    for(auto data : current_info)
    {
        string key = data.first;
        string validity_type = data.second;        

        if(key == "CATEGORY")
            continue;

        cout << white << key << ": ";
        getline(cin, input[i]);

        if(input[i].length() == 0)
            continue;
        
        std::pair<string, string> aux_pair = make_pair(key, validity_type);
        if(!is_input_data_correct(aux_pair, input[i], new_data))
            return false;

        i++;
    }

    return true;
}


bool device_mgr::edit_device(const string &device, map<uint16_t, pair<string, string>> &meta_map, const string &code)
{
    bool ret = false;        
    vector<pair<string, string>> current_data;
    vector<pair<string, string>> new_data;
    vector<pair<string, string>> current_info;

    if(device == RESISTOR)
        ret = db_mgr.retrieve_current_device_data(RESISTOR_DB, RESISTOR, code, current_data);
    else if(device == CAPACITOR)
        ret = db_mgr.retrieve_current_device_data(CAPACITOR_DB, CAPACITOR, code, current_data);
    else if(device == INDUCTOR)
        ret = db_mgr.retrieve_current_device_data(INDUCTOR_DB, INDUCTOR, code, current_data);
    else if(device == DIODE)
        ret = db_mgr.retrieve_current_device_data(DIODE_DB, DIODE, code, current_data);
    else if(device == LED)
        ret = db_mgr.retrieve_current_device_data(LED_DB, LED, code, current_data);
    else if(device == BJT)
        ret = db_mgr.retrieve_current_device_data(BJT_DB, BJT, code, current_data);
    else if(device == MOSFET)
        ret = db_mgr.retrieve_current_device_data(MOSFET_DB, MOSFET, code, current_data);

    if(!ret)
        return false;

    for(auto meta_pair : meta_map)
        current_info.push_back(make_pair(meta_pair.second.first, meta_pair.second.second));

    if(!load_changes(current_info, new_data))
        return false;
    
    if(new_data.size() == 0)
    {
        cout << endl << yellow << "No data has been modified" << white << endl;
        return true;
    }

    print_device_info_to_be_confirmed(new_data, UPDATE);

    if (is_validated() == NOT_CONFIRMED)
        return false;

    if(device == RESISTOR)
        ret = db_mgr.update_device(RESISTOR_DB, RESISTOR, code, new_data);
    if(device == CAPACITOR)
        ret = db_mgr.update_device(CAPACITOR_DB, CAPACITOR, code, new_data);
    if(device == INDUCTOR)
        ret = db_mgr.update_device(INDUCTOR_DB, INDUCTOR, code, new_data);    
    if(device == DIODE)
        ret = db_mgr.update_device(DIODE_DB, DIODE, code, new_data);    
    if(device == LED)
        ret = db_mgr.update_device(LED_DB, LED, code, new_data);    
    if(device == BJT)
        ret = db_mgr.update_device(BJT_DB, BJT, code, new_data);    
    if(device == MOSFET)
        ret = db_mgr.update_device(MOSFET_DB, MOSFET, code, new_data);    

    return ret;
}


bool device_mgr::edit_mgr(void)
{
    bool ret = false;    
    list<string> device_list;
    string device_type_in;
    string code_in;
    vector<string> meta_structure;
    map<uint16_t, pair<string, string>> meta_map;

    if(!list_devices(device_list, false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;

    if(!check_range_validity(device_type_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    if(device_type_in != RESISTOR && 
       device_type_in != CAPACITOR && 
       device_type_in != INDUCTOR && 
       device_type_in != DIODE &&
       device_type_in != LED &&
       device_type_in != BJT &&
       device_type_in != MOSFET)
    {
        cerr << endl << red << "Wrong choice..." << white << endl;
        return false;
    }        

    cout << endl << blue << "Insert device code to see the current info" << white << endl;
    cout << endl << white << "in: " << white;    
    cin >> code_in;

    if(!check_range_validity(code_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    if(device_type_in == RESISTOR)
    {           
        switch(db_mgr.select_device(RESISTOR_DB, RESISTOR, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(RESISTOR, meta_map))
                    ret = edit_device(RESISTOR, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
        }
    }
    else if(device_type_in == CAPACITOR)
    {        
        switch(db_mgr.select_device(CAPACITOR_DB, CAPACITOR, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(CAPACITOR, meta_map))
                    ret = edit_device(CAPACITOR, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
        }
    }    
    else if(device_type_in == INDUCTOR)
    {        
        switch(db_mgr.select_device(INDUCTOR_DB, INDUCTOR, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(INDUCTOR, meta_map))
                    ret = edit_device(INDUCTOR, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
        }
    }
    else if(device_type_in == DIODE)
    {        
        switch(db_mgr.select_device(DIODE_DB, DIODE, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(DIODE, meta_map))
                    ret = edit_device(DIODE, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
        }
    }
    else if(device_type_in == LED)
    {        
        switch(db_mgr.select_device(LED_DB, LED, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(LED, meta_map))
                    ret = edit_device(LED, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
        }
    }
    else if(device_type_in == BJT)
    {        
        switch(db_mgr.select_device(BJT_DB, BJT, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(BJT, meta_map))
                    ret = edit_device(BJT, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
        }
    }
    else if(device_type_in == MOSFET)
    {        
        switch(db_mgr.select_device(MOSFET_DB, MOSFET, code_in))
        {
            case db_mgr::SEARCH_FOUND:
            {
                if(json_mgr.retrieve_device_metadata(MOSFET, meta_map))
                    ret = edit_device(MOSFET, meta_map, code_in);
            }
            break;

            case db_mgr::SEARCH_NOT_FOUND:
            {
                cerr << endl << yellow << "Not code found!" << white << endl;
            }
            break;
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
    list<string> device_list;
    string device_type_in;
    
    cout << endl << blue << "Choose device type among: " << endl << endl;

    if(!list_devices(device_list, false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(!check_range_validity(device_type_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    if(device_type_in == RESISTOR)
        ret = db_mgr.show_table(RESISTOR_DB, RESISTOR);
    else if(device_type_in == CAPACITOR)
        ret = db_mgr.show_table(CAPACITOR_DB, CAPACITOR);
    else if(device_type_in == INDUCTOR)
        ret = db_mgr.show_table(INDUCTOR_DB, INDUCTOR);
    else if(device_type_in == DIODE)
        ret = db_mgr.show_table(DIODE_DB, DIODE);
    else if(device_type_in == LED)
        ret = db_mgr.show_table(LED_DB, LED);
    else if(device_type_in == BJT)
        ret = db_mgr.show_table(BJT_DB, BJT);
    else if(device_type_in == MOSFET)
        ret = db_mgr.show_table(MOSFET_DB, MOSFET);
    else
        cerr << endl << red << "Wrong choice..." << white << endl;

    return ret;
}


bool device_mgr::search_by_code(void)
{
    bool ret = false;
    unsigned int counter = 0;
    string code_in;
    map<string, string> db_and_table_names; 

    if(!db_mgr.build_db_table_map(db_and_table_names))
        return ret;
    
    cout << endl << blue << "Digit code" << endl;
    cout << endl << white << "in: " << white;
    cin >> code_in;

    if(!check_range_validity(code_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    for(auto db_table_pair : db_and_table_names)
        ret = db_mgr.show_device_by_parameter(db_table_pair.first.c_str(), db_table_pair.second, CODE_PARAMETER, code_in, false, counter);

    if(counter == 0)
        cout << endl << endl << yellow << "Code not found!" << white << endl;

    return ret;
}


bool device_mgr::search_by_manufacturer(void)
{
    bool ret = false;
    unsigned int counter = 0;
    string manufacturer_in;
    map<string, string> db_and_table_names; 

    if(!db_mgr.build_db_table_map(db_and_table_names))
        return ret;

    
    cout << endl << blue << "Digit manufacturer" << endl;
    cout << endl << white << "in: " << white;
    cin >> manufacturer_in;    

    if(!check_range_validity(manufacturer_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    for(auto db_table_pair : db_and_table_names)
        ret = db_mgr.show_device_by_parameter(db_table_pair.first.c_str(), db_table_pair.second, MANUFACTURER_PARAMETER, manufacturer_in, false, counter);

    if(counter == 0)
        cout << endl << endl << yellow << "Manufacturer not found!" << white << endl;

    return ret;
}


bool device_mgr::search_by_mounting_type(void)
{
    bool ret = false;
    unsigned int counter = 0;
    string mounting_type_in;
    map<string, string> db_and_table_names; 

    if(!db_mgr.build_db_table_map(db_and_table_names))
        return ret;
    
    cout << endl << blue << "Digit mounting type" << endl;
    cout << endl << white << "in: " << white;
    cin >> mounting_type_in;    

    if(!check_range_validity(mounting_type_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    for(auto db_table_pair : db_and_table_names)
        ret = db_mgr.show_device_by_parameter(db_table_pair.first.c_str(), db_table_pair.second, MOUNTING_TYPE_PARAMETER, mounting_type_in, false, counter);

    if(counter == 0)
        cout << endl << endl << yellow << "Mounting type not found!" << white << endl;

    return ret;
}


bool device_mgr::search_by_category(void)
{
    bool ret = false;
    unsigned int counter = 0;
    string category_in;
    map<string, string> db_and_table_names; 

    if(!db_mgr.build_db_table_map(db_and_table_names))
        return ret;
    
    cout << endl << blue << "Digit category" << endl;
    cout << endl << white << "in: " << white;
    cin >> category_in;    

    if(!check_range_validity(category_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    for(auto db_table_pair : db_and_table_names)
        ret = db_mgr.show_device_by_parameter(db_table_pair.first.c_str(), db_table_pair.second, CATEGORY_PARAMETER, category_in, false, counter);

    if(counter == 0)
        cout << endl << endl << yellow << "Category not found!" << white << endl;

    return ret;
}


bool device_mgr::search_by_description(void)
{
    bool ret = false;
    unsigned int counter = 0;
    string description_in;
    map<string, string> db_and_table_names; 

    if(!db_mgr.build_db_table_map(db_and_table_names))
        return ret;
    
    cout << endl << blue << "Digit mounting type" << endl;
    cout << endl << white << "in: " << white;
    cin >> description_in;    

    if(!check_range_validity(description_in, ALPHANUMERIC))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    for(auto db_table_pair : db_and_table_names)
        ret = db_mgr.show_device_by_parameter(db_table_pair.first.c_str(), db_table_pair.second, DESCRIPTION_PARAMETER, description_in, false, counter);

    if(counter == 0)
        cout << endl << endl << yellow << "Description not found!" << white << endl;

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
    cout << blue << "\t5) " << white << "category" << endl;
    cout << blue << "\t6) " << white << "description" << endl;    

    cout << endl << white << "in: ";
    cin >> selection;
    
    if(selection.size() > 1)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
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

            case 5:
                ret = search_by_category();
            break;

            case 6:
                ret = search_by_description();
            break;

            default:
                cerr << endl << red << "Wrong input..." << white << endl;
                ret = false;
        }
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        ret = false;
    }

    return ret;
}


bool device_mgr::transfer_mgr(void)
{
    return db_mgr.copy_db_to_cloud();
}


/*******************************************************************************************************/
/*                                          GLOSSARY section                                           */ 
/*******************************************************************************************************/
bool device_mgr::glossary_mgr(void)
{
    bool ret = false;
    string selection;

    cout << endl;
    cout << blue << "Digit correct number to select device glossary: " << endl << endl;
    cout << blue << "\t1) " << white << "resistor" << endl;
    cout << blue << "\t2) " << white << "capacitor" << endl;
    cout << blue << "\t3) " << white << "inductor" << endl;
    cout << blue << "\t4) " << white << "diode" << endl;
    cout << blue << "\t5) " << white << "LED" << endl;
    cout << blue << "\t6) " << white << "BJT" << endl;    
    cout << blue << "\t7) " << white << "MOSFET" << endl;    

    cout << endl << white << "in: ";
    cin >> selection;
    

    cout << endl;
    
    try
    {
        switch(stoul(selection, nullptr, 10))
        {
            case 1:
                ret = print_glossary(RESISTOR_GLOSSARY);
            break;

            case 2:
                ret = print_glossary(CAPACITOR_GLOSSARY);
            break;

            case 3:
                ret = print_glossary(INDUCTOR_GLOSSARY);
            break;

            case 4:
                ret = print_glossary(DIODE_GLOSSARY);
            break;

            case 5:
                ret = print_glossary(LED_GLOSSARY);
            break;

            case 6:
                ret = print_glossary(BJT_GLOSSARY);
            break;

            case 7:
                ret = print_glossary(MOSFET_GLOSSARY);
            break;

            default:
                cerr << endl << red << "Wrong input..." << white << endl;
                ret = false;
        }
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        ret = false;
    }

    return ret;
}