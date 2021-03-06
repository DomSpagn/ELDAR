project(ELDAR)
cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 14)

include_directories(include rapidjson)

set(sources ${sources} 
    src/main.cpp
    src/device_mgr.cpp
    src/json_mgr.cpp
)

add_executable(eldar ${sources})  