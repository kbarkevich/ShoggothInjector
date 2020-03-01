#include "HPLint.h"

const byte DWORD_LEN = sizeof(DWORD);

HPLint::HPLint(string name, MemSearch *memSearch)
{
	_name = name;
	_memSearch = memSearch;
	_addr = _memSearch->FindVariable(name, HPLint::IsIt);
}

int HPLint::GetVal()
{
	if (_addr == 0)
		return -1;
	u_int location = _addr + 28;
	string value = _memSearch->GetStringAt(location);
	int result = atoi(value.c_str());
	return result;
}

void HPLint::SetVal(int val)
{
	if (_addr == 0)
		return;
	u_int location = _addr + 28;
	int stat = _memSearch->WriteStringAt(location, to_string(val));
	if (stat > 0)
	{
		cout << "Memory written to process." << endl;
	}
	else
	{
		cout << "Memory couldn't be written!" << endl;
	}
}

bool HPLint::exists()
{
	return _addr != 0;
}

u_int HPLint::GetAddr()
{
	return _addr;
}

bool HPLint::IsIt(u_int addr, string name, HANDLE* proc)
{
	// TODO IMPLEMENT THIS
	HANDLE process = *proc;
	u_int val = addr + 28;
	char value[12];
	ReadProcessMemory(process, (LPVOID)val, &value, 8, 0);
	value[11] = '\0';
	if (string(value) == "0")
		return true;
	else
		return false;
}

map<string, HPLint*> HPLint::GetHPLints(vector<string> patterns, MemSearch* memSearch)
{
	map<string, HPLint*> HPLints;
	map<string, future<HPLint*>> futures;
	ctpl::thread_pool pool(8);
	for (int i = 0; i < patterns.size(); i++)
	{
		string pattern = patterns[i];
		futures[pattern] = pool.push([pattern, memSearch]
		(int id)
			{
				return new HPLint(pattern, memSearch);
			});
	}
	pool.stop(true);
	for (int i = 0; i < patterns.size(); i++)
	{
		HPLints[patterns[i]] = futures[patterns[i]].get();
		if (HPLints[patterns[i]]->exists())
			cout << "HPL int " << patterns[i] << " exists! Value: " << HPLints[patterns[i]]->GetVal() << endl;
		else
			cout << "HPL int " << patterns[i] << " does not exist..." << endl;
	}
	return HPLints;
}