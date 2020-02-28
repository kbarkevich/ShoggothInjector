#pragma once
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <list>
#include "HPLvar.h"
using namespace std;

class MemSearch
{
public:
	MemSearch();
	MemSearch(HANDLE *process);
	void SetProcess(HANDLE *process);
	HANDLE process();
	u_int FindVariable(string const& pattern, bool (*callback)(u_int, string, HANDLE*));
	string GetStringAt(u_int addr);
	int WriteStringAt(u_int addr, string val);

private:
	HANDLE *_process;
	template <class InIter1, class InIter2>
	static void FindAll(unsigned char* base, InIter1 buf_start, InIter1 buf_end, InIter2 pat_start, InIter2 pat_end, list<u_int>& res);
	void CorrectOne(u_int addrs[]);
};

