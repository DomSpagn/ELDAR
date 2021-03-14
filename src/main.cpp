#include <iostream>
#include "device_mgr.hpp"
#include "ConsoleColor.hpp"

using namespace std;

int main(int argc, char **argv)
{    
    device_mgr device;
    string selection;

    cout << endl;
    cout << blue << "Select action: " << endl << endl;
    cout << blue << "\ta: " << white << "add device" << endl;
    cout << blue << "\td: " << white << "delete device" << endl;
    cout << blue << "\te: " << white << "edit device" << endl;

    cout << endl << white << "in: ";
    cin >> selection;
    if(selection.size() > 1)
    {
        cerr << red << "Wrong input..." << white << endl;
        return EXIT_FAILURE;
    }        
    cout << endl;
    
    switch(selection[0])
    {
        case 'a':
            device.insert_mgr();
        break;

        case 'd':
        break;

        case 'e':
        break;

        default:
            cerr << red << "Wrong input..." << white << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}










