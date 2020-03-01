// Shoggoth Injector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <map>
#include "MemSearch.h"
#include "HPLint.h"
#include "HPLfloat.h"

void test1(map<string, HPLint*> HPLints, map<string, HPLfloat*> HPLfloats);

int main()
{
    // Open Amnesia window and hook into process
    char win_name[100];
    cout << "Enter name of Amnesia window, including whitespace: ";
    cin.getline(win_name, sizeof(win_name));
    cout << endl << "Opening process \"" << win_name << "\"" << endl;
    HWND hWnd = FindWindowA(0, win_name);
    if (hWnd == 0)
    {
        cout << "Whoops, could not find that window!" << endl;
        return 1;
    }
    DWORD PID;
    GetWindowThreadProcessId(hWnd, &PID);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
    if (!hProc)
    {
        cerr << "Cannot open that process." << endl;
        return 1;
    }

    // Initialize memory searcher
    MemSearch* memSearch = new MemSearch(&hProc);

    // Get names of all integers to hook into
    string pattern;
    vector<string> floatNames;
    cout << "Enter string patterns to find HPL floats in memory (or nothing to stop): ";
    while (getline(std::cin, pattern))
    {
        if (pattern.empty())
            break;
        floatNames.push_back(pattern);
    }
    cout << "Getting floats......" << endl;
    map<string, HPLfloat*> HPLfloats = HPLfloat::GetHPLFloats(floatNames, memSearch);

    // Get names of all floats to hook into
    vector<string> intNames;
    cout << "Enter string patterns to find HPL integers in memory (or nothing to stop): ";
    while (getline(std::cin, pattern))
    {
        if (pattern.empty())
            break;
        intNames.push_back(pattern);
    }
    cout << "Getting ints......" << endl;
    map<string, HPLint*> HPLints = HPLint::GetHPLints(intNames, memSearch);

    // RUN TESTS HERE
    test1(HPLints, HPLfloats);

    CloseHandle(hProc);
    cout << "Press Enter To Exit" << endl;
    cin.get();
}

/*
    Test 1: Add a value to all ints in a loop.
*/
void test1(map<string, HPLint*> HPLints, map<string, HPLfloat*> HPLfloats)
{
    cout << "How many to add to the ints per loop: ";
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
        for (map<string, HPLint*>::iterator iter = HPLints.begin(); iter != HPLints.end(); ++iter)
        {
            iter->second->SetVal(spawnCount);
        }
        this_thread::sleep_for(std::chrono::milliseconds(msDelay));
    }
}