#include <string>
#include <vector>
#include <map>

const std::string NOT_NULL_CONSTRAINT = "NOT NULL";
const std::string UNIQUE_CONSTRAINT = "UNIQUE";

const std::string SPACE = " ";
const std::string COMMA = ",";
const std::string SEMICOLON = ";";
const std::string EQUAL = "=";
const std::string LIKE = "LIKE";
const std::string TEXT = "TEXT";
const std::string PERCENTAGE = "%";
const std::string END = ");";
const std::string SINGLE_QUOTE = "'";
const std::string DOUBLE_QUOTE = "\"";


std::string create_table(const std::string &table, std::vector<std::pair<std::string, std::string>> &device_info);
std::string insert_row(const std::string &table, std::vector<std::pair<std::string, std::string>> &device_info);
std::string select_row_by_code(const std::string &table, const std::string &code);
bool show_sql_table(const char *device_db, const std::string &table);
bool show_sql_device(const char *device_db, const std::string &table, const std::string &parameter, const std::string &parameter_value, bool exact, unsigned int &counter);
std::string table_size(const char *device_db, const std::string &table);
bool get_table_from_db(const char *db, std::string &table_name);
std::string delete_row(const std::string &table, const std::string &code);
std::string update_row(const std::string &table, const std::string &code, std::vector<std::pair<std::string, std::string>> &device_changes);