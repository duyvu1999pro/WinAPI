#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <iomanip>
#include <tlhelp32.h>
#include <iostream>
using namespace std;

HANDLE FindProcess(int Pid);
bool is_number(char k[]);
int main(void)
{
    
        cout << "Pid: ";
        char input[30];
        cin >> input;
        if (is_number(input))
        {
            HANDLE hProcess = FindProcess(atoi(input));
            MEMORY_BASIC_INFORMATION mib;
            unsigned char* p = NULL;
            cout << "Cac vung nho co Protection Attribute PAGE_EXECUTE_READWRITE la:" << endl;
            while (1)
            {

                if (VirtualQueryEx(hProcess, p, &mib, sizeof(mib)) != sizeof(mib))
                {
                    break;
                }
                if (mib.Protect == PAGE_EXECUTE_READWRITE)
                {                   
                    printf("\n0x%08X", p);
                }
                p += mib.RegionSize;
            }
        }
     
    return 0;
}
HANDLE FindProcess(int Pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Pid);

    return hProcess;
}
bool is_number(char k[])
{
    for (int i = 0; i < sizeof(k) / sizeof(k[0]); i++)
    {
        if (!isdigit(k[i]))
        {
            return false;
        }
    }
    return true;
}
