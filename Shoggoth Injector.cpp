// Shoggoth Injector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include "HPLint.h"
#include "HPLfloat.h"

int main()
{
    char win_name[100];
    cout << "Enter name of Amnesia window, including whitespace: ";
    cin.getline(win_name, sizeof(win_name));
    cout << endl << "Opening process \"" << win_name << "\"" << endl;
    HWND hWnd = FindWindowA(0, win_name);
    if (hWnd == 0)
    {
        cout << "Whoops, could not find that window!" << endl;
    }
    else
    {
        DWORD PID;
        GetWindowThreadProcessId(hWnd, &PID);
        HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
        if (!hProc)
        {
            cerr << "Cannot open that process." << endl;
        }
        else
        {
            MemSearch* memSearch = new MemSearch(&hProc);
            char pattern[100];
            cout << "Enter string pattern to find in memory: ";
            cin.getline(pattern, sizeof(pattern));
            cout << endl << "Finding instance of \"" << pattern << "\"" << endl;
            string stdpattern(pattern);
            /* UNCOMMENT TO USE AN INT INSTEAD
            HPLint *hplint = new HPLint(stdpattern, memSearch);
            if (hplint->exists())
                cout << "It exists: Address: " << hex << hplint->GetAddr() << "; Value: " << dec << hplint->GetVal() << endl;
            else
                cout << "Variable does not exist..." << endl;
            */
            HPLfloat* hplfloat = new HPLfloat(stdpattern, memSearch);
            if (hplfloat->exists())
                cout << "It exists!" << endl;
            cout << "WAddress: " << hex << hplfloat->GetWAddr() << endl;
            cout << "DAddress: " << hex << hplfloat->GetDAddr() << endl;
            cout << "Value: " << hplfloat->GetVal() << endl;
            cout << "How many to add per loop: ";
            int spawnCount = 0;
            cin >> spawnCount;
            cout << endl << "How many loops to do: ";
            int loopCount = 0;
            cin >> loopCount;
            cout << endl << "And finally, how long (in ms) between each pass of the loop: ";
            int msDelay = 0;
            cin >> msDelay;
            for (int i = 5; i > 0; i--)
            {
                cout << "Starting in " << i << "..." << endl;
                this_thread::sleep_for(std::chrono::seconds(1));
            }

            for (int i = 0; i < loopCount; i++)
            {
                //hplint->SetVal(spawnCount); UNCOMMENT TO USE AN INT INSTEAD
                hplfloat->SetVal(spawnCount * 0.1f);
                this_thread::sleep_for(std::chrono::milliseconds(msDelay));
            }

            CloseHandle(hProc);
            cout << "Press Enter To Exit" << endl;
            cin.get();
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
