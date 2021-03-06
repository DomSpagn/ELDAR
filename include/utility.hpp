#include <string>

bool is_file_present(const char *path, std::string filename);   //filename is the relative file path
bool delete_file(std::string filename);                         //filename is the relative file path
bool create_file(std::string filename);                         //filename is the relative file path

bool detected_sign_matter(std::string &str);
bool is_digits(const std::string &str);
bool check_uint8_validity(std::string input);
bool check_uint16_validity(std::string input);
bool check_uint32_validity(std::string input);
bool check_uint64_validity(std::string input);
bool check_int8_validity(std::string input);
bool check_int16_validity(std::string input);
bool check_int32_validity(std::string input);
bool check_int64_validity(std::string input);
bool check_float_validity(std::string input);
bool check_double_validity(std::string input);
bool check_string_validity(std::string input);
