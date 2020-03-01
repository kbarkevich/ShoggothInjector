#pragma once
#include "HPLvar.h"
#include <iostream>
using namespace std;

class HPLfloat : public HPLvar
{
public:
	const string _SI_PREFIX = "_SI_";
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

private:
	u_int _addr_w;
	u_int _addr_d;
	MemSearch* _memSearch;
	string _name;
	string GetWName();
	string GetDName();
};

