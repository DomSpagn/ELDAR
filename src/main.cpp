#include <iostream>
#include "device_mgr.hpp"
#include "console_color.hpp"
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
        cout << blue << "\ta: " << white << "Add device" << endl;
        cout << blue << "\td: " << white << "Delete device" << endl;
        cout << blue << "\te: " << white << "Edit device" << endl;
        cout << blue << "\ti: " << white << "Software info" << endl;
        cout << blue << "\ts: " << white << "Search device" << endl;
        cout << blue << "\tt: " << white << "Transfer DB files on cloud" << endl;
        cout << blue << "\tq: " << white << "Quit" << endl;

        cout << endl << white << "in: ";
        cin >> selection;
        if(selection.size() > 1)
        {
            cerr << endl << red << "Wrong input..." << white << endl;
            result = false;
        }
        else
        {
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

                case 's':
                    result = device.search_mgr();
                break;

                case 't':
                    result = device.transfer_mgr();
                break;

                case 'q':
                    result = true;
                break;

                default:
                {
                    cerr << endl << red << "Wrong input..." << white << endl;
                    result = false;
                }
                break;
            }

            if(result && selection[0] != 'q')
            {
                string key_in;
                cin.ignore();
                while(1)
                {                
                    cout << endl << green << "Successfull Operation." << white << endl;
                    cout << endl << blue << "Press ENTER to go back to menu" << white << endl;
                    getline(cin, key_in);
                    if(key_in.length() == 0)
                        break;
                }
            }
            else if(!result)
            {
                string key_in;
                cout << endl << blue << "Press ENTER to go back to menu" << white << endl;
                cin.ignore();
                while(1)
                {                                                
                    getline(cin, key_in);
                    if(key_in.length() == 0)
                        break;
                }
            }
            else
            {
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}










