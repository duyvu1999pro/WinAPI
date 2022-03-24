#include <windows.h>
#include "pch.h"
#include "Bai2.h"
//#define BAI2_API extern "C" __declspec(dllexport)

int Message()
{
    int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"Hi! Im a DLL",
        (LPCWSTR)L"",
        MB_OK
    );
    return msgboxID;
}