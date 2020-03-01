#pragma once
#include <iostream>
#include <Windows.h>
#include <list>
#include <string>
#include "HPLvar.h"
#include "MemSearch.h"
#include "lib/CTPL/ctpl_stl.h"
#include <map>
#include <future>

using namespace std;

class HPLint : public HPLvar
{
public:
	HPLint(std::string name, MemSearch *memSearch);
	int GetVal();
	void SetVal(int val);
	// Does the variable exist in Amnesia memory?
	bool exists();
	u_int GetAddr();
	static bool IsIt(u_int, string, HANDLE*);
	static map<string, HPLint*> GetHPLints(vector<string>, MemSearch*);

private:
	u_int _addr;
	MemSearch *_memSearch;
	string _name;
};

