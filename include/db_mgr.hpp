#include "utility.hpp"

class db_mgr
{
public:
    db_mgr();
    ~db_mgr();

    //Create (if it does not exist) a db and apply an INSERT to add new device record
    bool add_resistor(std::vector<std::tuple<std::string, std::string, std::any>>resistor_vector_tuple);
    bool add_capacitor(std::vector<std::tuple<std::string, std::string, std::any>>capacitor_vector_tuple);
    bool add_inductor(std::vector<std::tuple<std::string, std::string, std::any>>inductor_vector_tuple);

protected:    
    
};
