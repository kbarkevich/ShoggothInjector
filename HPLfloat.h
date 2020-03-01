#pragma once
#include "HPLvar.h"
#include "lib/CTPL/ctpl_stl.h"
#include <map>
#include <future>
#include <iostream>
using namespace std;

class HPLfloat : public HPLvar
{
public:
	const string _SI_PREFIX = "_";
	const string _SI_FLOATD_SUFFIX = "_D";
	const string _SI_FLOATW_SUFFIX = "_W";
	HPLfloat(std::string name, MemSearch* memSearch);
	float GetVal();
	void SetVal(float val);
	// Does the variable exist in Amnesia memory?
	bool exists();
	u_int GetWAddr();
	u_int GetDAddr();
	static bool IsIt(u_int, string, HANDLE*);
	static map<string, HPLfloat*> GetHPLFloats(vector<string>, MemSearch*);

private:
	u_int _addr_w;
	u_int _addr_d;
	MemSearch* _memSearch;
	string _name;
	string GetWName();
	string GetDName();
};

