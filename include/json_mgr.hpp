#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <vector>
#include <map>
#include <tuple>
#include <any>

class json_mgr
{
public:
    json_mgr();
    ~json_mgr();

    //Read from meta_device.json to build a meta map for each device
    bool retrieve_device_metadata(const char *device, std::map<uint16_t, std::pair<std::string, std::string>> &meta_map);    

    //Insert a device into related DB starting from its meta info
    bool add_device(const char *device, std::map<uint16_t, std::pair<std::string, std::string>> &meta_map);

protected:
    //Just fills the meta map
    bool get_meta_info_from_json(const rapidjson::Value &device_info, std::map<uint16_t, std::pair<std::string, std::string>> &meta_map);

    //Fill the device info using values inserted by the user from standard input
    bool load_device_meta_info(std::map<uint16_t, std::pair<std::string, std::string>> &meta_map, std::vector<std::tuple<std::string, std::string, std::any>> &device_vector_tuple);

    //Specialize resistor insertion
    bool add_resistor(std::map<uint16_t, std::pair<std::string, std::string>> &meta_map, std::vector<std::tuple<std::string, std::string, std::any>> &resistor_vector_tuple);

    //Specialize capacitor insertion
    bool add_capacitor(std::map<uint16_t, std::pair<std::string, std::string>> &meta_map, std::vector<std::tuple<std::string, std::string, std::any>> &capacitor_vector_tuple);

    //Specialize inductor insertion
    bool add_inductor(std::map<uint16_t, std::pair<std::string, std::string>> &meta_map, std::vector<std::tuple<std::string, std::string, std::any>> &inductor_vector_tuple);
};