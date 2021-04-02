#include "utility.hpp"
#include <functional>

extern std::string select_by_code_return;

class db_mgr
{
public:
    db_mgr();
    ~db_mgr();

    //Create (if it does not exist) a db and apply an INSERT to add new device record
    bool insert_device(const std::string &DEVICE, const char *DEVICE_DB, std::vector<std::tuple<std::string, std::string, std::any>>device_vector_tuple);

    //Delete a device record
    bool delete_device(const std::string &DEVICE, const char *DEVICE_DB);

protected:    
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


