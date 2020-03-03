#include "HPLfloat.h"

const byte DWORD_LEN = sizeof(DWORD);

HPLfloat::HPLfloat(std::string name, MemSearch* memSearch)
{
	_name = name;
	_memSearch = memSearch;
	_addr_w = _memSearch->FindVariable(this->GetWName(), HPLfloat::IsIt);
	_addr_d = _memSearch->FindVariable(this->GetDName(), HPLfloat::IsIt);
}

string HPLfloat::GetWName()
{
	return _SI_PREFIX + _name + _SI_FLOATW_SUFFIX;
}
string HPLfloat::GetDName()
{
	return _SI_PREFIX + _name + _SI_FLOATD_SUFFIX;
}

float HPLfloat::GetVal()
{
	if (_addr_w == 0 || _addr_d == 0)
		return -1.0f;
	u_int wlocation = _addr_w + 28;
	u_int dlocation = _addr_d + 28;
	string wvalue = _memSearch->GetStringAt(wlocation);
	string dvalue = _memSearch->GetStringAt(dlocation);
	reverse(dvalue.begin(), dvalue.end());
	float result = strtof((wvalue + "." + dvalue).c_str(), nullptr);
	return result;
}

void HPLfloat::SetVal(float val)
{
	if (_addr_w == 0 || _addr_d == 0)
		return;
	u_int wlocation = _addr_w + 28;
	u_int dlocation = _addr_d + 28;

	string valstr = to_string(val);
	int decindex = valstr.find('.');
	string wstr;
	string dstr;
	if (decindex == string::npos)
	{
		wstr = valstr.substr(0, decindex);
		dstr = valstr.substr(decindex + 1, valstr.length() - decindex - 1);
	}
	else
	{
		wstr = valstr;
		dstr = "0";
	}
	reverse(dstr.begin(), dstr.end());
	int stat = _memSearch->WriteStringAt(wlocation, wstr);
	if (stat > 0)
	{
		cout << "'W' Memory written to process." << endl;
	}
	else
	{
		cout << "'W'Memory couldn't be written!" << endl;
	}
	stat = _memSearch->WriteStringAt(dlocation, dstr);
	if (stat > 0)
	{
		cout << "'D' Memory written to process." << endl;
	}
	else
	{
		cout << "'D' Memory couldn't be written!" << endl;
	}
}

bool HPLfloat::exists()
{
	return _addr_w != 0 && _addr_d != 0;
}

u_int HPLfloat::GetWAddr()
{
	return _addr_w;
}

u_int HPLfloat::GetDAddr()
{
	return _addr_d;
}


bool HPLfloat::IsIt(u_int addr, string name, HANDLE* proc)
{
	// TODO REIMPLEMENT THIS
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

map<string, HPLfloat*> HPLfloat::GetHPLFloats(vector<string> patterns, MemSearch* memSearch)
{
	map<string, HPLfloat*> HPLfloats;
	map<string, future<HPLfloat*>> futures;
	ctpl::thread_pool pool(8);
	for (int i = 0; i < patterns.size(); i++)
	{
		string pattern = patterns[i];
		futures[pattern] = pool.push([pattern, memSearch]
		(int id)
			{
				return new HPLfloat(pattern, memSearch);
			});
	}
	pool.stop(true);
	for (int i = 0; i < patterns.size(); i++)
	{
		HPLfloats[patterns[i]] = futures[patterns[i]].get();
		if (HPLfloats[patterns[i]]->exists())
			cout << "HPL float " << patterns[i] << " exists! Value: " << HPLfloats[patterns[i]]->GetVal() << endl;
		else
			cout << "HPL float " << patterns[i] << " does not exist..." << endl;
	}
	return HPLfloats;
}