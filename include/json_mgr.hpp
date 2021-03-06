#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <map>

class json_mgr
{
public:
    json_mgr();
    ~json_mgr();
    bool retrieve_device_metadata(const char *device, std::map<std::string, std::string>& meta_map);
    bool is_file_present(const char *path, std::string filename);                                                   //filename is the relative file path    
    bool create_device(const char *device, std::map<std::string, std::string>& meta_map);

protected:    
    bool get_device_info(const rapidjson::Value& resistor_doc, std::map<std::string, std::string>& meta_map);    
    bool delete_json(std::string filename);                                                                         //filename is the relative file path
    bool create_new_json(std::string filename);                                                                     //filename is the relative file path
    bool create_resistor(std::map<std::string, std::string>& meta_map);
    bool create_capacitor(std::map<std::string, std::string>& meta_map);
    bool create_inductor(std::map<std::string, std::string>& meta_map);
};