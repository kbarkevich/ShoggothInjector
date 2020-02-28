#pragma once
#include <iostream>
#include <Windows.h>
#include <list>
#include <string>
#include "HPLvar.h"
#include "MemSearch.h"

using namespace std;

class HPLint : public HPLvar
{
public:
	HPLint(std::string name, MemSearch *memSearch);
	int GetVal();
	void SetVal(int val);
	bool exists();
	u_int GetAddr();
	static bool IsIt(u_int, string, HANDLE*);

private:
	u_int _addr;
	MemSearch *_memSearch;
	string _name;
};

