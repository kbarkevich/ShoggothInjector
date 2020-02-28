#include "MemSearch.h"

MemSearch::MemSearch()
{
    this->_process = NULL;
}

MemSearch::MemSearch(HANDLE *process)
{
    this->_process = process;
}

void MemSearch::SetProcess(HANDLE *process)
{
    this->_process = process;
}

HANDLE MemSearch::process()
{
    return *(this->_process);
}

u_int MemSearch::FindVariable(string const& pattern, bool (*callback)(u_int, string, HANDLE*))
{
    unsigned char* p = NULL;
    MEMORY_BASIC_INFORMATION info;
    list<u_int> mem_list;
    HANDLE pro = this->process();
    for (p = NULL;
        VirtualQueryEx(this->process(), p, &info, sizeof(info)) == sizeof(info);
        p += info.RegionSize)
    {
        vector<char> buffer;
        vector<char>::iterator pos;

        if (info.State == MEM_COMMIT &&
            (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE))
        {
            SIZE_T bytes_read;
            buffer.resize(info.RegionSize);
            ReadProcessMemory(this->process(), p, &buffer[0], info.RegionSize, &bytes_read);
            buffer.resize(bytes_read);
            MemSearch::FindAll(p, buffer.begin(), buffer.end(), pattern.begin(), pattern.end(), mem_list);
        }
    }

    while (mem_list.size() > 0)
    {
        u_int curr = mem_list.front();
        mem_list.pop_front();
        if (callback(curr, string(pattern), this->_process))
        {
            return curr;
        }
    }
    return NULL;
}

template <class InIter1, class InIter2>
void MemSearch::FindAll(unsigned char* base, InIter1 buf_start, InIter1 buf_end, InIter2 pat_start, InIter2 pat_end, list<u_int>& res)
{
    for (InIter1 pos = buf_start;
        buf_end != (pos = search(pos, buf_end, pat_start, pat_end));
        ++pos)
    {
        res.push_front(u_int(base + (pos - buf_start)));
    }
}

string MemSearch::GetStringAt(u_int addr)
{
    char value[128];
    ReadProcessMemory(this->process(), (LPVOID)addr, &value, 128, 0);
    value[128 - 1] = '\0';
    return string(value);
}

int MemSearch::WriteStringAt(u_int addr, string val)
{
    return WriteProcessMemory(this->process(), (LPVOID)addr, val.c_str(), (DWORD)sizeof(val.c_str()), NULL);
}