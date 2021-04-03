#include <string>
#include <vector>
#include <any>
#include <tuple>

const std::string NOT_NULL_CONSTRAINT = "NOT NULL";
const std::string UNIQUE_CONSTRAINT = "UNIQUE";

const std::string SPACE = " ";
const std::string COMMA = ",";
const std::string SEMICOLON = ";";
const std::string EQUAL = "=";
const std::string END = ");";
const std::string SINGLE_QUOTE = "'";
const std::string DOUBLE_QUOTE = "\"";


std::string create_table(const std::string &table, std::vector<std::tuple<std::string, std::string, std::any>> &device_vector_tuple);
std::string insert_row(const std::string &table, std::vector<std::tuple<std::string, std::string, std::any>> &device_vector_tuple);
std::string select_row_by_code(const std::string &table, const std::string &code);
std::string delete_row(const std::string &table, const std::string &code);
std::string update_row(const std::string &table, std::vector<std::pair<std::string, std::string>> &device_change_pair);