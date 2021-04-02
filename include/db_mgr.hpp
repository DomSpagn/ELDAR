#include "utility.hpp"

class db_mgr
{
public:
    db_mgr();
    ~db_mgr();

    //Create (if it does not exist) a db and apply an INSERT to add new device record
    bool insert_device(const std::string &DEVICE, const char *DEVICE_DB, std::vector<std::tuple<std::string, std::string, std::any>>device_vector_tuple);

protected:    
    
};
