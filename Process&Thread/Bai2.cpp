#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include <string>
using namespace std;
bool compare(WCHAR* input1, const char* input2);
void killProcess(const char* filename);
void killProcess(int Pid);

bool is_number(char k[])
{
    for (int i = 0; i < sizeof(k) / sizeof(k[0]) ; i++)
    {
        if (!isdigit(k[i]))
        {
            return false;
        }
    }
    return true;
}
int main()
{
    cout << "taskkill ";
    char input[30];
    cin >> input;
    if (is_number(input))
    {
        killProcess(atoi(input));
    }
    else
    {     
        killProcess(input);
    }
      
    
    
 
}
bool compare(WCHAR* input1, const char* input2)
{
    for (size_t i = 0; i < wcslen(input1); i++)
    {
        if ((int)input2[i] != input1[i])
        {
            return false;//khac
        }
    }
    return true; //giong
}
void killProcess(const char* filename)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hProcessSnap, &pe32);
    while (1)
    {
        if (compare(pe32.szExeFile, filename))
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pe32.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        if (!Process32Next(hProcessSnap, &pe32))
            break;
    }
    CloseHandle(hProcessSnap);
}
void killProcess(int Pid)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hProcessSnap, &pe32);
    while (1)
    {
        if (pe32.th32ProcessID == Pid)
        {

            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pe32.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        if (!Process32Next(hProcessSnap, &pe32))
            break;
    }
    CloseHandle(hProcessSnap);
}