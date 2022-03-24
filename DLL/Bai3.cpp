#include <windows.h>
#include "pch.h"
#include "Bai3.h"
#include <fstream>
#include <iostream>

using namespace std;
wchar_t* toLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}
int string(const char* charArray)
{
    int msgboxID = MessageBox(
        NULL,
        toLPCWSTR(charArray),
        (LPCWSTR)L"",
        MB_OK
    );
    return msgboxID;
}
void writeFile(const char* filename, const char* content)
{   
    fstream f;                      
    f.open(filename, ios::out);
    f << content;
    f.close();
}
