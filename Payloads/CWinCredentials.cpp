#pragma once
#include "CWinCredentials.h"

CWinCredentials::CWinCredentials()
{
	hAdvApi32 = NULL;
	loaded = FALSE;
}
CWinCredentials::~CWinCredentials()
{
	Fresh();
}
BOOL CWinCredentials::IsLoaded()
{
	return loaded;
}
BOOL CWinCredentials::Init()
{
	if (loaded) return TRUE;

	hAdvApi32 = LoadLibrary(_T("advapi32.dll"));
	if (hAdvApi32 != NULL)
	{
		//Dynamically load CredRead, CredEnumerate, and CredFree API functions.
		pCredRead = (CredReadFuncType)GetProcAddress(hAdvApi32, "CredReadW");
		pCredFree = (CredFreeFuncType)GetProcAddress(hAdvApi32, "CredFree");
		pCredEnumerate = (CredEnumerateType)GetProcAddress(hAdvApi32, "CredEnumerateW");

		//If all 3 functions are available, return TRUE.
		if (pCredRead != NULL && pCredFree != NULL && pCredEnumerate != NULL)
			loaded = TRUE;
		else
		{
			//Failed to load the credentials API functions.
			Fresh();
		}
	}

	return loaded;
}
void CWinCredentials::Fresh()
{
	//Free advapi32 library, if we previously loaded it.
	if (hAdvApi32 != NULL)
	{
		FreeLibrary(hAdvApi32);
		hAdvApi32 = NULL;
	}

	loaded = FALSE;

}
BOOL CWinCredentials::CredRead(LPCTSTR TargetName,DWORD Type,DWORD Flags,PCREDENTIAL* Credential)
{
	if (loaded)
		return pCredRead(TargetName, Type, Flags, Credential);
	else
		return FALSE;
}
BOOL CWinCredentials::CredEnumerate(LPCTSTR Filter,DWORD Flags,DWORD* Count,PCREDENTIAL** Credentials)
{
	if (loaded)
		return pCredEnumerate(Filter, Flags, Count, Credentials);
	else
		return FALSE;
}
VOID CWinCredentials::CredFree(PVOID Buffer)
{
	if (loaded)
		pCredFree(Buffer);
}