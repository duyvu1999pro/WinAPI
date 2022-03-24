#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
using namespace std;
void gotoxy(int x, int y);
BOOL GetProcessList();
int main()
{
    GetProcessList();
}
void gotoxy(int x, int y)
{
    HANDLE h;
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}
BOOL GetProcessList()
{
    HANDLE hProcessSnap;

    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return(FALSE);
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return(FALSE);
    }
    gotoxy(0, 4);//set vi tri con tro
    cout << "       Image Name" << setw(20) << "|" << setw(7) << "PID" << setw(5) << "|";
    int RowIndex = 5;
    Process32First(hProcessSnap, &pe32);
    while (1)
    {      
        _tprintf(TEXT("\n%s"), pe32.szExeFile);
        gotoxy(40, RowIndex); RowIndex++;      
        _tprintf(TEXT("%d"), pe32.th32ProcessID);
        if (!Process32Next(hProcessSnap, &pe32))
            break;
    }
    CloseHandle(hProcessSnap);
    return(TRUE);
}