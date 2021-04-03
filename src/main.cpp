#include <iostream>
#include "device_mgr.hpp"
#include "ConsoleColor.hpp"
#include "utility.hpp"

using namespace std;


int main(int argc, char **argv)
{    
    bool result = false;
    device_mgr device;
    string selection;

    while(1)
    {
        cout << endl;
        cout << blue << "Select action: " << endl << endl;
        cout << blue << "\ta: " << white << "add device" << endl;
        cout << blue << "\td: " << white << "delete device" << endl;
        cout << blue << "\te: " << white << "edit device" << endl;
        cout << blue << "\ti: " << white << "software info" << endl;
        cout << blue << "\tq: " << white << "quit" << endl;

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
                result = device.insert_mgr();
            break;

            case 'd':
                result = device.delete_mgr();
            break;

            case 'e':                
                result = device.edit_mgr();
            break;

            case 'i':                
                result = load_software_info();
            break;

            case 'q':
                result = true;
            break;

            default:
                cerr << red << "Wrong input..." << white << endl;
                result = false;
        }

        if(result)
        {
            cout << green << "Successfull Operation" << white << endl;
            break;
        }
        else
        {
            cout << yellow << "Still to be decided how to handle eldar..." << white << endl;
            break;
        }
    }
    return EXIT_SUCCESS;
}










