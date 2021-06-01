#include "utility.hpp"
#include "sqlite3.h"
#include <functional>


class db_mgr
{
public:
    db_mgr();
    ~db_mgr();

    enum SEARCH_RESULT
    {
        SEARCH_ERROR,
        SEARCH_NOT_FOUND,
        SEARCH_FOUND
    };

    //Create (if it does not exist) a db and apply an INSERT to add new device record
    bool insert_device(const char *device_db, const std::string &table, std::vector<std::tuple<std::string, std::string, std::any>>device_vector_tuple);

    //Select a device record by code
    SEARCH_RESULT select_device(const char *device_db, const std::string &table, const std::string& code);

    //Delete a device record
    bool delete_device(const char *device_db, const std::string &table);

    //Get current device data starting from its code
    bool retrieve_current_device_data(const char *device_db, const std::string &table, const std::string& code, std::vector<std::tuple<std::string, std::string, std::any>> &current_data);

    //Update an existing device record
    bool update_device(const char *device_db, const std::string &table, const std::string &code, std::vector<std::tuple<std::string, std::string, std::any>> &new_data);

    //Show all components stored in a table
    bool show_table(const char *device_db, const std::string &table);

    //Show one device info selected by table column parameter
    bool show_device_by_parameter(const char *device_db, const std::string &table, const std::string &parameter, const std::string &parameter_value, bool exact, unsigned int &counter);

    //Function used to fill a map of db files and related tables
    bool has_suffix(const std::string &s, const std::string &suffix);
    std::string get_table(const char *db_name);
    bool build_db_table_map(std::map<std::string, std::string> &db_and_table_names);

    //Search a device by code among all available db files and prints its info
    SEARCH_RESULT search_code_among_dbs(std::map<std::string, std::string> &db_and_table_names, const std::string &code);

    //Transfer db files on cloud (OneDrive)
    bool copy_db_to_cloud(void);

protected:
    sqlite3 *database_connection(const char *device_db);
    
    /********************************************************************************************************/
    /*                                      CALLBACKS for SQLite                                            */
    /********************************************************************************************************/
    static std::string select_by_code_return;
    static int select_by_code(void *NotUsed, int argc, char **argv, char **azColName)
    {
        // Take the code
        select_by_code_return = argv[1];
        return 0;
    }

    static int table_count;
    static int select_to_count(void *NotUsed, int argc, char **argv, char **azColName) 
    {
        table_count = atoi(argv[0]);
        return 0;
    }
};


