#include "utility.hpp"
#include "const.hpp"
#include <dirent.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

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
        cerr << "Cannot open directory..." << endl;
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


bool create_file(string filename)
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


bool is_digits(const std::string &str)
{
    return str.find_first_not_of("-+0123456789") == string::npos;
}


bool check_uint8_validity(string input)
{
    //uint8_t = char (1 byte)
    unsigned long value;
    try
    {
        value = stoul(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }
    
    if(value > UINT8_MAX)
        return false;

    return true;
}


bool check_uint16_validity(string input)
{
    //uint16_t = unsigned short (2 bytes)
    unsigned long value;
    try
    {
        value = stoul(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }
    
    if(value > UINT16_MAX)
        return false;

    return true;
}


bool check_uint32_validity(string input)
{
    //uint32_t = unsigned int (4 bytes)
    unsigned long value;
    try
    {
        value = stoul(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }
    
    if(value > UINT32_MAX)
        return false;

    return true;
}


bool check_uint64_validity(string input)
{
    //uint64_t = unsigned long long (8 bytes)
    unsigned long long value;
    try
    {
        value = stoull(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }
    
    if(value > UINT64_MAX)
        return false;

    return true;
}


bool check_int8_validity(string input)
{
    //int8_t = signed char (1 byte)
    int value;
    try
    {
        value = stoi(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }
    
    if(!is_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT8_MIN || value > INT8_MAX)
        return false;

    return true;
}


bool check_int16_validity(string input)
{
    //int16_t = short (2 bytes)
    int value;
    try
    {
        value = stoi(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }

    if(!is_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT16_MIN || value > INT16_MAX)
        return false;

    return true;
}


bool check_int32_validity(string input)
{
    //int32_t = int (4 bytes)
    int value;
    try
    {
        value = stoi(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }

    if(!is_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT32_MIN || value > INT32_MAX)
        return false;

    return true;
}


bool check_int64_validity(string input)
{
    //int64_t = long long (8 bytes)
    long long value;
    try
    {
        value = stoll(input, nullptr, 10);
    }
    catch(...)
    {
        cout << "Wrong input..." << endl;
        return false;
    }
    
    if(!is_digits(input) || detected_sign_matter(input))
        return false;

    if(value < INT64_MIN || value > INT64_MAX)
        return false;

    return true;
}


bool check_float_validity(string input)
{
    return true;
}


bool check_double_validity(string input)
{
    return true;
}


bool check_string_validity(string input)
{
    return true;
}
