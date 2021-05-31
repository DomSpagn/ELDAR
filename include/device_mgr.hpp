#include "json_mgr.hpp"
#include "db_mgr.hpp"
#include <iostream>
#include <vector>

class device_mgr
{   
public:
    device_mgr();
    ~device_mgr();
    
    bool insert_mgr(void);
    bool delete_mgr(void);
    bool edit_mgr(void);
    bool search_mgr(void);

protected:
    json_mgr &json_mgr;
    db_mgr &db_mgr;

    //Insert a device into related DB starting from its meta info
    bool add_device(const std::string &device, std::map<uint16_t, std::pair<std::string, std::string>> &meta_map);

    //Load new device data starting from the old ones
    bool load_changes(std::vector<std::tuple<std::string, std::string, std::any>> &current_data, std::vector<std::tuple<std::string, std::string, std::any>> &new_data);

    //Edit a device into related DB starting from its meta info
    bool edit_device(const std::string &device, const std::string &code);

    //Search by device type
    bool search_by_type(void);

    //Search by device code
    bool search_by_code(void);

    //Search by device manufacturer
    bool search_by_manufacturer(void);

    //Search by mounting type
    bool search_by_mounting_type(void);

    //Search by description
    bool search_by_description(void);
    
    //Search by value
    bool search_by_value(void);
};