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


bool delete_file(string filename)
{   
    bool ret = false; 
    string abs_filename = DB_FILE_PATH + filename;

    if(!is_file_present(DB_FILE_PATH, filename))
    {
        cerr << endl << red << "Cannot delete " << filename << "because it does not exist" << white << endl;
        return ret;
    }

    if(remove(abs_filename.c_str()) == 0)
        ret = true;
    else
        cerr << endl << red << "Cannot delete " << filename << white << endl;

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
bool list_types(bool load_example)
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


bool check_uint8_validity(string &input, uint8_t &u8_value)
{
    //uint8_t = char (1 byte)
    unsigned long value;
    try
    {
        value = stoul(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(value > UINT8_MAX)
        return false;

    u8_value = (uint8_t)value;    

    return true;
}


bool check_uint16_validity(string &input, uint16_t &u16_value)
{
    //uint16_t = unsigned short (2 bytes)
    unsigned long value;
    try
    {
        value = stoul(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(value > UINT16_MAX)
        return false;

    u16_value = (uint16_t)value;

    return true;
}


bool check_uint32_validity(string &input, uint32_t &u32_value)
{
    //uint32_t = unsigned int (4 bytes)
    unsigned long value;
    try
    {
        value = stoul(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(value > UINT32_MAX)
        return false;

    u32_value = (uint32_t)value;    

    return true;
}


bool check_uint64_validity(string &input, uint64_t &u64_value)
{
    //uint64_t = unsigned long long (8 bytes)
    unsigned long long value;
    try
    {
        value = stoull(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(value > UINT64_MAX)
        return false;

    u64_value = (uint64_t)value;    

    return true;
}


bool check_int8_validity(string &input, int8_t &i8_value)
{
    //int8_t = signed char (1 byte)
    int value;
    try
    {
        value = stoi(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(!is_integer_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT8_MIN || value > INT8_MAX)
        return false;

    i8_value = (int8_t)value;    

    return true;
}


bool check_int16_validity(string &input, int16_t &i16_value)
{
    //int16_t = short (2 bytes)
    int value;
    try
    {
        value = stoi(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }

    if(!is_integer_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT16_MIN || value > INT16_MAX)
        return false;

    i16_value = (int16_t)value;

    return true;
}


bool check_int32_validity(string &input, int32_t &i32_value)
{
    //int32_t = int (4 bytes)
    int value;
    try
    {
        value = stoi(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }

    if(!is_integer_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT32_MIN || value > INT32_MAX)
        return false;

    i32_value = (int32_t)value;

    return true;
}


bool check_int64_validity(string &input, int64_t &i64_value)
{
    //int64_t = long long (8 bytes)
    long long value;
    try
    {
        value = stoll(input, nullptr, 10);
    }
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(!is_integer_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT64_MIN || value > INT64_MAX)
        return false;

    i64_value = (int64_t)value;    

    return true;
}


bool check_float_validity(string &input, float &f_value)
{
    float value;

    try
    {
        value = stof(input);        
    }    
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(!is_float_digits(input) || detected_sign_matter(input))
        return false;

    if(value < -FLT_MAX || value > FLT_MAX)
        return false;

    f_value = value;

    return true;    
}


bool check_double_validity(string &input, double &d_value)
{
    double value;

    try
    {
        value = stod(input);
    }    
    catch(...)
    {
        cerr << endl << red << "Wrong input..." << white << endl;
        return false;
    }
    
    if(!is_float_digits(input) || detected_sign_matter(input))
        return false;

    if(value < -DBL_MAX || value > DBL_MAX)
        return false;

    d_value = value;

    return true;    
}


bool check_string_validity(string &input)
{    
    return input.empty() ? false : true;
}


bool check_input_validity(string &input, input_validity validity_range)
{   
    string range;

    switch(validity_range)
    {
        case NUMERIC:
            range = "01234567890.-";   
        break;

        case SIMPLE_ALPHA:
            range = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.-";
        break;

        case COMPLEX_ALPHA:            
        break;
    }

    return input.find_first_not_of(range) != string::npos ? false : true;        
}


/***********************************************************************************************************/
/*                                             Miscellaneous                                               */ 
/***********************************************************************************************************/
void print_device_tuple_vector(vector<tuple<string, string, any>> &device_vector_tuple, tuple_operation operation)
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

    for (const auto& tuple_elem : device_vector_tuple)
    {
        string meta_name = get<0>(tuple_elem);
        string meta_type = get<1>(tuple_elem);
        any value = get<2>(tuple_elem);

        if(meta_name == "device")
            continue;

        cout << green << meta_name << ": " << white;

        switch(operation)
        {
            case INSERT:
            {
                if(meta_type == "uint8")            
                    printf("%d\n", any_cast<uint8_t>(value));
                if(meta_type == "int8")
                    printf("%d\n", any_cast<int8_t>(value));
                if(meta_type == "uint16")
                    cout << any_cast<uint16_t>(value) << endl;
                if(meta_type == "int16")
                    cout << any_cast<int16_t>(value) << endl;
                if(meta_type == "uint32")
                    cout << any_cast<uint32_t>(value) << endl;
                if(meta_type == "int32")
                    cout << any_cast<int32_t>(value) << endl;
                if(meta_type == "uint64")
                    cout << any_cast<uint64_t>(value) << endl;
                if(meta_type == "int64")
                    cout << any_cast<int64_t>(value) << endl;
                if(meta_type == "float")
                    cout << any_cast<float>(value) << endl;
                if(meta_type == "double")
                    cout << any_cast<double>(value) << endl;
                if(meta_type == "string")
                    cout << any_cast<string>(value) << endl;
            }
            break;

            case UPDATE:
            {
                if(meta_type == "integer")            
                    cout << any_cast<int64_t>(value) << endl;
                if(meta_type == "real")
                    cout << any_cast<double>(value) << endl;
                if(meta_type == "text")
                    cout << any_cast<string>(value) << endl;
            }
            break;
        }
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