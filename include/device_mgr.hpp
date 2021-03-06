#include "json_mgr.hpp"
#include <iostream>
#include <vector>

class device_mgr
{   
public:
    device_mgr();
    ~device_mgr();

protected:
    json_mgr json_mgr;
    
    bool list_types(void);
    bool insert_device(void);
    bool delete_device(void);
    bool edit_device(void);
    bool load_devices(void);            
    bool search_by_category(void);
    bool search_by_name(void);
    bool search_by_manufacturer(void);
    bool search_by_code(void);
    bool search_by_package(void);
    bool search_by_mounting_type(void);
};