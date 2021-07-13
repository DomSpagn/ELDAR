#pragma once

#include <string>
#include <map>
#include <vector>
#include <list>

enum answer { CONFIRM, NOT_CONFIRMED, UNKNOWN };
enum map_operation { INSERT, UPDATE };
enum input_validity
{ 
    ALPHANUMERIC,
    UNSIGNED_INTEGER,
    PERCENT_VALUE,
    RESISTANCE,
    POWER,
    CAPACITANCE,
    VOLTAGE,
    INDUCTANCE,
    CURRENT,
    WAVELENGTH
};

bool is_file_present(const char *path, std::string filename);
bool create_file(std::string root_path, std::string filename);

bool list_devices(std::list<std::string> &device_list, bool load_example);

bool detected_sign_matter(std::string &str);
bool is_integer_digits(const std::string &str);
bool is_float_digits(const std::string &str);

bool check_range_validity(std::string &input, input_validity validity_range);
enum VALIDITY{ VALID, NOT_VALID, SKIPPED };
bool is_input_data_correct(std::pair<std::string, std::string> &validity_type, std::string &input, std::vector<std::pair<std::string, std::string>> &input_data);
VALIDITY check_alphanumeric_validity(std::string &input);
VALIDITY check_unsigned_integer_validity(std::string &input);
VALIDITY check_percentage_validity(std::string &input);
VALIDITY check_electric_value_validity(std::string &input, input_validity validity_range);

void print_device_info_to_be_confirmed(std::vector<std::pair<std::string, std::string>> &device_info, map_operation operation);

answer is_validated(void);
bool load_software_info(void);
bool print_glossary(const char *device);