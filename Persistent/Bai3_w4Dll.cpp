// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>
#include "pch.h"
#include <windows.h>
#include <tchar.h>
#define DllExport __declspec (dllexport)

int dll_hijack()
{
	MessageBox(0, _T("DLL Hijacking DEMO!"), _T("DLL HAS BEEN HIJACKED"), MB_OK);
	return 0;
}

BOOL WINAPI  DllMain(
	HANDLE    hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved)
{
	dll_hijack();
	return 0;
}