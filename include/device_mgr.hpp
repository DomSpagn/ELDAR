#include "json_mgr.hpp"
#include <iostream>
#include <vector>

class device_mgr
{   
public:
    device_mgr();
    ~device_mgr();

    //Select the component to be inserted
    bool insert_mgr(void);

protected:
    json_mgr json_mgr;
    
    //Retrieve all the existing components from device_type.txt
    bool list_types(void);
};