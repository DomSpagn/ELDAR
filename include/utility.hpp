#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <any>

enum answer { CONFIRM, NOT_CONFIRMED, UNKNOWN };


bool is_file_present(const char *path, std::string filename);
bool delete_file(std::string filename);
bool create_file(std::string root_path, std::string filename);

bool detected_sign_matter(std::string &str);
bool is_integer_digits(const std::string &str);
bool is_float_digits(const std::string &str);
bool check_uint8_validity(std::string &input, uint8_t &u8_value);
bool check_uint16_validity(std::string &input, uint16_t &u16_value);
bool check_uint32_validity(std::string &input, uint32_t &u32_value);
bool check_uint64_validity(std::string &input, uint64_t &u64_value);
bool check_int8_validity(std::string &input, int8_t &i8_value);
bool check_int16_validity(std::string &input, int16_t &i16_value);
bool check_int32_validity(std::string &input, int32_t &i32_value);
bool check_int64_validity(std::string &input, int64_t &i64_value);
bool check_float_validity(std::string &input, float &f_value);
bool check_double_validity(std::string &input, double &d_value);
bool check_string_validity(std::string &input);

void print_device_tuple_vector(std::vector<std::tuple<std::string, std::string, std::any>> &device_vector_tuple);
answer is_validated(void);
bool load_software_info(void);