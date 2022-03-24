#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <TlHelp32.h>

#define MAX_THREADS 3

using namespace std;

int CountWord(const char* name);
int CountFileCurrentDir();
int CountThread(const char* processName);
PROCESSENTRY32 FindProcess(const char* filename);
bool compare(WCHAR* input1, const char* input2);
DWORD WINAPI Thread(PVOID pvParam);
int GetTime(HANDLE thread);


int main()
{
    int ThreadInput[MAX_THREADS];
    DWORD dwThreadID[MAX_THREADS];
    HANDLE hThread[MAX_THREADS];
    int ThreadTime[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
    {
        ThreadInput[i] = i;
       hThread[i] = CreateThread(NULL, 0, Thread, (PVOID)&ThreadInput[i], 0, &dwThreadID[i]);
       if (hThread[i] == NULL)
       {
           ExitProcess(3);
       }
       WaitForSingleObject(hThread[i], INFINITE);
       ThreadTime[i] = GetTime(hThread[i]);
       CloseHandle(hThread[i]);
    }


    int temp = 1000;
    for (int i = 0; i < MAX_THREADS; i++)
    {
        int ThreadIndex = 0;
        for (int i = 0; i < MAX_THREADS; i++)
        {
            if (ThreadTime[i] < temp)
            {
                temp = ThreadTime[i];
                ThreadIndex = i;
            }
        }
        cout << endl << "Thread "<< (ThreadIndex+1)<< " : Giai "<<i+1<< " (Thoi gian hoan thanh: " << ThreadTime[ThreadIndex]<< " Milliseconds)";
        ThreadTime[ThreadIndex] = 9999;
        temp = 1000;

    }

   
}

DWORD WINAPI Thread(PVOID pvParam) {
    int a = *((int*)pvParam);
    switch (a)
    {
    case 0:
        cout << "\nThread-1: \nso tu co trong file 1.txt la: " << CountWord("1.txt") << endl;
        break;
    case 1:
        cout << "\nThread-2: \nSo file co trong thu muc hien tai la: " << CountFileCurrentDir() << endl;
        break;
    case 2:
        cout << "\nThread-3: \nSo thread cua process explorer.exe la: " << CountThread("explorer.exe") << endl;
        break;

    }

    return(0);
}
int GetTime(HANDLE thread)
{
    SYSTEMTIME CreationTime = { 0 };
    SYSTEMTIME ExitTime = { 0 };

    FILETIME ftCreationTime, ftExitTime, ftKernelTime, ftUserTime;
    GetThreadTimes(thread, &ftCreationTime, &ftExitTime, &ftKernelTime, &ftUserTime);
    FileTimeToSystemTime(&ftCreationTime, &CreationTime);
    FileTimeToSystemTime(&ftExitTime, &ExitTime);

    ExitTime.wMinute = ExitTime.wMinute - CreationTime.wMinute;
    ExitTime.wSecond = ExitTime.wSecond - CreationTime.wSecond;
    ExitTime.wMilliseconds = ExitTime.wMilliseconds - CreationTime.wMilliseconds;

    int Milisecond = ExitTime.wMilliseconds + ExitTime.wSecond * 1000 + ExitTime.wMinute * 60 * 1000;
    return Milisecond;
}
int CountWord(const char* name)
{
    int count = 0;
    int c;
    FILE* file;
    file = fopen(name, "r");
    if (file) {
        while ((c = getc(file)) != EOF)
            count++;
        fclose(file);
    }
    return count;

    //#define _CRT_SECURE_NO_DEPRECATE
}
int CountFileCurrentDir()
{
    int a = 0;
    TCHAR CurrentDir[200];
    GetCurrentDirectory(200, CurrentDir);
    int length = _tcsclen(CurrentDir);
    CurrentDir[length] = TEXT('\\');
    CurrentDir[length + 1] = TEXT('*');
    for (int i = length + 2; i < _tcsclen(CurrentDir); i++)
    {
        CurrentDir[i] = 0;
    }
    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFile(CurrentDir, &ffd);
    while (1)
    {
        //_tprintf(TEXT("  %s  \n"), ffd.cFileName);
       // if (ffd.cFileName[0] != 46)
       // {
            a++;
       // }
        if (FindNextFile(hFind, &ffd) == 0)
        {
            break;
        }
    }
    a -= 2;
    FindClose(hFind);
    return a;
   //#include <tchar.h>
    //#include <windows.h>
}
int CountThread(const char* processName)
{
    DWORD dwOwnerPID = FindProcess(processName).th32ProcessID;
    int count = 0;
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);
    Thread32First(hThreadSnap, &te32);
    while (1)
    {
        if (te32.th32OwnerProcessID == dwOwnerPID)
            count++;

        if (!Thread32Next(hThreadSnap, &te32))
            break;
    }
    return count;
    //#include <windows.h>
    //#include <TlHelp32.h>
}
PROCESSENTRY32 FindProcess(const char* filename)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hProcessSnap, &pe32);
    while (1)
    {
        if (compare(pe32.szExeFile, filename))
        {
            return pe32;
        }
        if (!Process32Next(hProcessSnap, &pe32))
            break;
    }
    return pe32;
    CloseHandle(hProcessSnap);
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


