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
        ret = json_mgr.load_device(meta_map, device_vector_tuple, RESISTOR_CATEGORY);
    if(device == CAPACITOR)
        ret = json_mgr.load_device(meta_map, device_vector_tuple, CAPACITOR_CATEGORY);
    if(device == INDUCTOR)
        ret = json_mgr.load_device(meta_map, device_vector_tuple, INDUCTOR_CATEGORY);
    if(device == DIODE)
        ret = json_mgr.load_device(meta_map, device_vector_tuple, DIODE_CATEGORY);
    if(device == LED)
        ret = json_mgr.load_device(meta_map, device_vector_tuple, LED_CATEGORY);
    if(device == BJT)
        ret = json_mgr.load_device(meta_map, device_vector_tuple, BJT_CATEGORY);

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
    if(device == DIODE)
        ret = db_mgr.insert_device(DIODE_DB, DIODE, device_vector_tuple);
    if(device == LED)
        ret = db_mgr.insert_device(LED_DB, LED, device_vector_tuple);
    if(device == BJT)
        ret = db_mgr.insert_device(BJT_DB, BJT, device_vector_tuple);

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

    if(!check_input_validity(device_type_in, SIMPLE_ALPHA))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

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
    else if(device_type_in == DIODE)
    {
        if(json_mgr.retrieve_device_metadata(DIODE, meta_map))
            ret = add_device(DIODE, meta_map);
    }
    else if(device_type_in == LED)
    {
        if(json_mgr.retrieve_device_metadata(LED, meta_map))
            ret = add_device(LED, meta_map);
    }
    else if(device_type_in == BJT)
    {
        if(json_mgr.retrieve_device_metadata(BJT, meta_map))
            ret = add_device(BJT, meta_map);
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
    string device_type_in;

    if(!list_types(false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(!check_input_validity(device_type_in, SIMPLE_ALPHA))
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
    else
        cerr << endl << red << "Wrong choice..." << white << endl;
  
    return ret;
}


/*******************************************************************************************************/
/*                                          EDIT section                                               */ 
/*******************************************************************************************************/
bool device_mgr::load_changes(vector<tuple<string, string, any>> &current_data, vector<tuple<string, string, any>> &new_data)
{
    cout << endl << blue << "Insert new values or press ENTER to skip:" << white << endl;    
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

        if(key == "CATEGORY")
            continue;

        cout << white << key << " (" << yellow << type << white << "): "; 
        getline(cin, input[i]);

        if(!check_input_validity(input[i], SIMPLE_ALPHA))
        {
            cerr << endl << red << "Input not allowed..." << white << endl;
            return false; 
        }

        if(input[i].length() == 0)
            continue;
        else if(type == "integer" && check_int64_validity(input[i], int_value) == VALID)
            aux_tuple = make_tuple(key, type, int_value);
        else if(type == "real" && check_double_validity(input[i], real_value) == VALID)
            aux_tuple = make_tuple(key, type, real_value);
        else if(type == "text" && check_string_validity(input[i]) == VALID)
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
    else if(device == DIODE)
        ret = db_mgr.retrieve_current_device_data(DIODE_DB, DIODE, code, current_data);
    else if(device == LED)
        ret = db_mgr.retrieve_current_device_data(LED_DB, LED, code, current_data);
    else if(device == BJT)
        ret = db_mgr.retrieve_current_device_data(BJT_DB, BJT, code, current_data);

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
    if(device == DIODE)
        ret = db_mgr.update_device(DIODE_DB, DIODE, code, new_data);    
    if(device == LED)
        ret = db_mgr.update_device(LED_DB, LED, code, new_data);    
    if(device == BJT)
        ret = db_mgr.update_device(BJT_DB, BJT, code, new_data);    

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

    if(!check_input_validity(device_type_in, SIMPLE_ALPHA))
    {
        cerr << endl << red << "Input not allowed..." << white << endl;
        return false; 
    }

    if(device_type_in != RESISTOR && 
       device_type_in != CAPACITOR && 
       device_type_in != INDUCTOR && 
       device_type_in != DIODE &&
       device_type_in != LED &&
       device_type_in != BJT)
    {
        cerr << endl << red << "Wrong choice..." << white << endl;
        return false;
    }        

    cout << endl << blue << "Insert device code to see the current info" << white << endl;
    cout << endl << white << "in: " << white;    
    cin >> code_in;

    if(!check_input_validity(code_in, SIMPLE_ALPHA))
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
                    ret = edit_device(RESISTOR, code_in);
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
                    ret = edit_device(CAPACITOR, code_in);
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
                    ret = edit_device(INDUCTOR, code_in);
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
                    ret = edit_device(DIODE, code_in);
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
                    ret = edit_device(LED, code_in);
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
                    ret = edit_device(BJT, code_in);
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
    string device_type_in;
    
    cout << endl << blue << "Choose device type among: " << endl << endl;

    if(!list_types(false))
        return false;

    cout << endl << white << "in: " << white;
    cin >> device_type_in;    

    if(!check_input_validity(device_type_in, SIMPLE_ALPHA))
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

    if(!check_input_validity(code_in, SIMPLE_ALPHA))
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

    if(!check_input_validity(manufacturer_in, SIMPLE_ALPHA))
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

    if(!check_input_validity(mounting_type_in, SIMPLE_ALPHA))
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

    if(!check_input_validity(category_in, SIMPLE_ALPHA))
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

    if(!check_input_validity(description_in, SIMPLE_ALPHA))
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
    bool ret = false;
    string selection;

    return db_mgr.copy_db_to_cloud();
}