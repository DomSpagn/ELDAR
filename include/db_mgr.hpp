#include "utility.hpp"
#include "sqlite3.h"
#include <functional>

extern std::string select_by_code_return;

class db_mgr
{
public:
    db_mgr();
    ~db_mgr();

    //Create (if it does not exist) a db and apply an INSERT to add new device record
    bool insert_device(const std::string &device, const char *device_db, std::vector<std::tuple<std::string, std::string, std::any>>device_vector_tuple);

    //Select a device record by code
    bool select_device(const std::string &device, const char *device_db);

    //Delete a device record
    bool delete_device(const std::string &device, const char *device_db);

    //Update an existing device record
    bool update_device(const std::string &device, const char *device_db, std::vector<std::pair<std::string, std::string>>device_change_pair);

    //Show device info and value stored into the related db
    void show_device(const std::string &device, const char *device_db);

protected:
    sqlite3 *database_connection(const char *device_db);
    
    /********************************************************************************************************/
    /*                                      CALLBACKS for SQLite                                            */
    /********************************************************************************************************/
    //SELECT CALLBACK
    static int select_by_code(void *NotUsed, int argc, char **argv, char **azColName)
    {
        // Take the code
        select_by_code_return = argv[1];       
        return 0;
    }
};


