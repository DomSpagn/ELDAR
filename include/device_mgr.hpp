#include "json_mgr.hpp"
#include "db_mgr.hpp"
#include <iostream>
#include <vector>

class device_mgr
{   
public:
    device_mgr();
    ~device_mgr();

    //Select the component to be inserted
    bool insert_mgr(void);
    bool delete_mgr(void);
    bool edit_mgr(void);

protected:
    json_mgr &json_mgr;
    db_mgr &db_mgr;

    //Retrieve all the existing components from device_type.txt
    bool list_types(bool load_example);

    //Insert a device into related DB starting from its meta info
    bool add_device(const std::string &device, std::map<uint16_t, std::pair<std::string, std::string>> &meta_map);

    //Load new device data starting from the old ones
    bool load_changes(std::vector<std::tuple<std::string, std::string, std::any>> &current_data, std::vector<std::tuple<std::string, std::string, std::any>> &new_data);

    //Edit a device into related DB starting from its meta info
    bool edit_device(const std::string &device, const std::string &code);
};