project(ELDAR)
cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include rapidjson sqlite3)

set(sources ${sources} 
    src/main.cpp
    src/utility.cpp
    src/device_mgr.cpp
    src/json_mgr.cpp
)

add_executable(eldar ${sources})  