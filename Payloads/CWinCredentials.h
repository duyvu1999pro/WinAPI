#pragma once
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <iostream>
#include <wincred.h>
#include <vector>

class CWinCredentials
{
protected:
	typedef BOOL(WINAPI* CredReadFuncType)(LPCTSTR TargetName,DWORD Type,DWORD Flags,PCREDENTIAL* Credential);
	typedef BOOL(WINAPI* CredEnumerateType)(LPCTSTR Filter,DWORD Flags,DWORD* Count,PCREDENTIAL** Credentials);
	typedef VOID(WINAPI* CredFreeFuncType)(PVOID Buffer);

	HMODULE hAdvApi32;
	BOOL loaded;
	CredReadFuncType pCredRead;
	CredFreeFuncType pCredFree;
	CredEnumerateType pCredEnumerate;

public:
	CWinCredentials();
	~CWinCredentials();
	BOOL Init();
	void Fresh();
	BOOL IsLoaded();
	BOOL CredRead(LPCTSTR TargetName,DWORD Type,DWORD Flags,PCREDENTIAL* Credential);
	BOOL CredEnumerate(LPCTSTR Filter,DWORD Flags,DWORD* Count,PCREDENTIAL** Credentials);
	VOID CredFree(PVOID Buffer);
};
