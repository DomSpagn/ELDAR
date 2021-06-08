#pragma once

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <any>

enum answer { CONFIRM, NOT_CONFIRMED, UNKNOWN };
enum tuple_operation { INSERT, UPDATE };
enum input_validity{ NUMERIC, SIMPLE_ALPHA, COMPLEX_ALPHA };

bool is_file_present(const char *path, std::string filename);
bool create_file(std::string root_path, std::string filename);

bool list_types(bool load_example);

bool detected_sign_matter(std::string &str);
bool is_integer_digits(const std::string &str);
bool is_float_digits(const std::string &str);

enum VALIDITY{ VALID, NOT_VALID, SKIPPED };

VALIDITY check_uint8_validity(std::string &input, uint8_t &u8_value);
VALIDITY check_uint16_validity(std::string &input, uint16_t &u16_value);
VALIDITY check_uint32_validity(std::string &input, uint32_t &u32_value);
VALIDITY check_uint64_validity(std::string &input, uint64_t &u64_value);
VALIDITY check_int8_validity(std::string &input, int8_t &i8_value);
VALIDITY check_int16_validity(std::string &input, int16_t &i16_value);
VALIDITY check_int32_validity(std::string &input, int32_t &i32_value);
VALIDITY check_int64_validity(std::string &input, int64_t &i64_value);
VALIDITY check_float_validity(std::string &input, float &f_value);
VALIDITY check_double_validity(std::string &input, double &d_value);
VALIDITY check_string_validity(std::string &input);
bool check_input_validity(std::string &input, input_validity validity_range);

void print_device_tuple_vector(std::vector<std::tuple<std::string, std::string, std::any>> &device_vector_tuple, tuple_operation operation);

answer is_validated(void);
bool load_software_info(void);
bool print_glossary(const char *device);