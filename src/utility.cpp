#include "utility.hpp"
#include "const.hpp"
#include "console_color.hpp"
#include <dirent.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <float.h>

using namespace std;


/***********************************************************************************************************/
/*                                          File Management                                                */ 
/***********************************************************************************************************/
bool is_file_present(const char *path, string filename)
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
    {
        cerr << endl << red << "Cannot open directory..." << white << endl;
        return ret;
    }

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


bool create_file(string root_path, string filename)
{         
    fstream file;
    file.open(root_path + filename, ios::out); 

    if(!file)
    {
        cerr << endl << red << "File " << filename << " has not been created..." << white << endl;
        return false;
    }

    file.close();
    return true;
}


/***********************************************************************************************************/
/*                                            Show device list                                             */ 
/***********************************************************************************************************/
bool list_devices(list<string> &device_list, bool load_example)
{
    if(!is_file_present(TXT_FILE_PATH, DEVICE_TYPE_FILE))
        return false;

    cout << blue << "Select component type among: " << white << endl << endl;
    fstream file;
    file.open(string(TXT_FILE_PATH) + string(DEVICE_TYPE_FILE), ios::in); 

    if(!file)
    {
        cerr << endl << red << "File " << DEVICE_TYPE_FILE << " cannot be opened!" << white << endl;
        return false;
    }

    //Read line by line
    string line;
    if(load_example)
    {
        while(getline(file, line))
        {
            device_list.push_back(line.substr(0, line.find(" ")));
            cout << white << "\t- " << blue << line.substr(0, line.find(" ")) << white << line.substr(line.find(" ")) << white << endl;
        }
    }
    else
    {
        while(getline(file, line))
        {
            device_list.push_back(line.substr(0, line.find(" ")));
            cout << white << "\t- " << blue << line.substr(0, line.find(" ")) << white << endl;
        }
    }
    file.close();
    return true;
}


/***********************************************************************************************************/
/*                                              Check inputs                                               */ 
/***********************************************************************************************************/
bool detected_sign_matter(string &str)
{
    size_t minus_count = count(str.begin(), str.end(), '-');
    size_t plus_count = count(str.begin(), str.end(), '+');
    
    if(minus_count > 1 || plus_count > 1)
        return true;
    
    if((minus_count == 1 && str.at(0) != '-') || (plus_count == 1 && str.at(0) != '+'))
        return true;

    return false;
}


bool is_integer_digits(const string &str)
{
    return str.find_first_not_of("-+0123456789") == string::npos;
}


bool is_float_digits(const string &str)
{
    return str.find_first_not_of(".-+0123456789") == string::npos;
}


VALIDITY check_alphanumeric_validity(string &input)
{   
    VALIDITY ret = NOT_VALID;

    if(input == string())
        ret = SKIPPED;
    else if(!check_range_validity(input, ALPHANUMERIC))
        ret = NOT_VALID;
    else
        ret = VALID;

    return ret;
}


VALIDITY check_unsigned_integer_validity(string &input)
{
    if(input == string())
        return SKIPPED;
    else if(!check_range_validity(input, UNSIGNED_INTEGER))
        return NOT_VALID;

    //Check if multiple '0' are present in front of the input
    while (input.length() > 1)
    {
        if(input.front() != '0')
            break;
        else
            input.erase(0, 1);            
    }

    return VALID;
}


VALIDITY check_percentage_validity(string &input)
{
    string math_symbols = ".-+";    

    if(input == string())
        return SKIPPED;
    else if(!check_range_validity(input, PERCENT_VALUE))
        return NOT_VALID;
    
    //Check if input is consistent
    if(input.length() == 1 && input.find_first_of(math_symbols) != string::npos)
        return NOT_VALID;

    //Check if elements of math_symbols are repeated inside the input and if they are in the wrong position   
    for(char &letter : math_symbols)
    {
        size_t letter_count = count(input.begin(), input.end(), letter);
        if(letter_count > 1)
            return NOT_VALID;
        else if (letter_count == 1)
            if((letter == '+' || letter == '-') && letter != input.front())
                return NOT_VALID;        
    }
    if(input.back() == '.')
        return NOT_VALID;
    
    //Check if first char is '.' and add one '0' before it
    if(input.front() == '.')
        input.insert(0, "0");

    return VALID;
}


VALIDITY check_electric_value_validity(string &input, input_validity validity_range)
{
    string magnitudo_range = "fpnumkMGT";
    string math_symbols = ".-+";
    string all_symbols = magnitudo_range + math_symbols;

    if(input == string())
        return SKIPPED;
    else if(!check_range_validity(input, validity_range))
        return NOT_VALID;

    //Check if input is consistent
    if(input.length() == 1 && input.find_first_of(all_symbols) != string::npos)
        return NOT_VALID;

    //Check if elements of letters_range are repeated inside the input and if they are in the wrong position   
    for(char &letter : all_symbols)
    {
        size_t letter_count = count(input.begin(), input.end(), letter);
        if(letter_count > 1)
            return NOT_VALID;
        else if (letter_count == 1)
            if((letter == '+' || letter == '-') && letter != input.front())
                return NOT_VALID;        
    }
    for(char &letter : math_symbols)
    {
        if(letter == input.back())
            return NOT_VALID;
    }

    //Check if first char is '.' and add one '0' before it
    if(input.front() == '.')
        input.insert(0, "0");

    return VALID;
}


bool check_range_validity(string &input, input_validity validity_range)
{   
    string value_range;
    string magnitudo_range = "fpnumkMGT";

    switch(validity_range)
    {
        case ALPHANUMERIC:
            value_range = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.-";
            magnitudo_range.clear();
        break;

        case UNSIGNED_INTEGER:
            value_range = "0123456789";
            magnitudo_range.clear();
        break;

        case PERCENT_VALUE:
            value_range = "0123456789.-+";
            magnitudo_range.clear();
        break;

        case RESISTANCE:
        case POWER:
        case CAPACITANCE:
        case VOLTAGE:
        case INDUCTANCE:
        case CURRENT:
        case WAVELENGTH:
            value_range = "0123456789.-+";
        break;
    }

    return input.find_first_not_of(value_range + magnitudo_range) != string::npos ? false : true;
}


bool is_input_data_correct(pair<string, string> &validity_pair, string& input, vector<pair<string, string>> &input_data)
{
    bool ret = false;

    if(validity_pair.second == "alphanumeric")
    {
        switch(check_alphanumeric_validity(input))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong alpha-numeric value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                if(validity_pair.first == "code")
                {
                    cerr << endl << red << "This field cannot be empty..." << white << endl;
                    return false;
                }
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }                
    }
    else if(validity_pair.second == "unsigned_integer")
    {
        switch(check_unsigned_integer_validity(input))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong unsigned integer value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input));
                ret = true;
            }
            break;

            case SKIPPED:
            {                
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "percentage")
    {
        switch(check_percentage_validity(input))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong percent value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + PERCENTAGE_SYMBOL));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "ohm")
    {
        switch(check_electric_value_validity(input, RESISTANCE))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong resistance value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + RESISTANCE_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "watt")
    {
        switch(check_electric_value_validity(input, POWER))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong power value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + POWER_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "farad")
    {
        switch(check_electric_value_validity(input, CAPACITANCE))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong capacitance value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + CAPACITANCE_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "volt")
    {        
        switch(check_electric_value_validity(input, VOLTAGE))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong voltage value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + VOLTAGE_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "henry")
    {
        switch(check_electric_value_validity(input, INDUCTANCE))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong inductance value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + INDUCTANCE_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "ampere")
    {
        switch(check_electric_value_validity(input, CURRENT))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong current value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + CURRENT_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }
    }
    else if(validity_pair.second == "meter")
    {
        switch(check_electric_value_validity(input, WAVELENGTH))
        {
            case NOT_VALID:
            {
                cerr << endl << red << "Wrong wavelength value..." << white << endl;
            }
            break;

            case VALID:
            {
                input_data.push_back(make_pair(validity_pair.first, input + WAVELENGTH_UNIT));
                ret = true;
            }
            break;

            case SKIPPED:
            {
                input_data.push_back(make_pair(validity_pair.first, "-"));
                ret = true;
            }
            break;
        }                
    }

    return ret;
}


/***********************************************************************************************************/
/*                                             Miscellaneous                                               */ 
/***********************************************************************************************************/
void print_device_info_to_be_confirmed(vector<pair<string, string>> &device_info, map_operation operation)
{
    switch(operation)
    {
        case INSERT:
            cout << endl << green << "Inserted Data: " << endl << endl;
        break;
        case UPDATE:
            cout << endl << green << "Values to be confirmed: " << endl << endl;
        break;
    }

    for (const auto& elem_pair : device_info)
    {
        string meta_name = elem_pair.first;        
        string value = elem_pair.second;

        if(meta_name == "category")
            continue;

        cout << green << meta_name << ": " << white << value << endl;                
    }

    cout << white;
}


answer is_validated(void)
{    
    string input;    
    answer response;

    do
    {        
        cout << endl << blue << "Confirm? [y,n]: " << white;
        cin >> input;
        switch(input[0])
        {
            case 'y':
                response = CONFIRM;
            break;

            case 'n':
                response = NOT_CONFIRMED;
            break;            
            default:
                response = UNKNOWN;
            break;            
        }
    } 
    while (response == UNKNOWN);    
    
    return response;
}


bool load_software_info(void)
{
    fstream file;
    file.open(string(TXT_FILE_PATH) + string(RELEASE_NOTES_FILE), ios::in); 

    if(!file)
    {
        cerr << endl << red << "File " << RELEASE_NOTES_FILE << " cannot be opened!" << white << endl;
        return false;
    }

    //Read line by line
    string line;
    while(getline(file, line))
        cout << endl << white << line << white << endl;

    file.close();
    return true;
}


bool print_glossary(const char *filename)
{
    bool ret = false;
    DIR *dir; struct dirent *diread;
    vector<string> files;

    if ((dir = opendir(TXT_FILE_PATH)) != nullptr) 
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            if(diread->d_name[0] != '.' && diread->d_name[1] != '.')
                files.push_back(diread->d_name);
        }
        closedir (dir);
    } 
    else 
    {
        cerr << endl << red << "Cannot open directory..." << white << endl;
        return ret;
    }

    for (auto file : files) 
    {        
        if(file == filename)
        {
            fstream file;
            file.open(string(TXT_FILE_PATH) + string(filename), ios::in); 

            if(!file)
            {
                cerr << endl << red << "File " << DEVICE_TYPE_FILE << " cannot be opened!" << white << endl;
                ret = false;
            }
            else
            {
                //Read line by line
                string line;
                while(getline(file, line))
                    cout << blue << line.substr(0, line.find(" ")) << white << line.substr(line.find(" ")) << white << endl;
            }
            file.close();
            ret = true;
            break;
        }
    }

    if(!ret)
        cout << endl << red << "Glossary file is not present..." << white << endl;

    return ret;
}