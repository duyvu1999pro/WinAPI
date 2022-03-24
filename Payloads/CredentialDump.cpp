#pragma once

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <iostream>
#include <wincred.h>
#include <vector>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#define DUMP_STRING(p, name)   do {                                         \
        if (p->name) _tprintf(_T("  % 18s: %s\r\n"), _T(#name), p->name);   \
        else _tprintf(_T("  % 18s:\r\n"), _T(#name));                       \
} while (0);


#define DUMP_DWORD(p, name) do {                                            \
        _tprintf(_T("  % 18s: 0x%08X\r\n"), _T(#name), pCred->name);        \
    } while (0);


void DumpBinary(unsigned char* p, DWORD count)
{
    if (!p || !count)
    {
        _tprintf(_T("\r\n"));

    }

    if (p)
    {
        std::vector<unsigned char> row;
        for (int i = 0; i < count;)
        {
            row.push_back(*(p + i++));
            if (0 == i % 8)
            {
                _tprintf(_T("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X | %c %c %c %c %c %c %c %c\r\n"),
                    row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7],
                    isprint(row[0]) ? row[0] : _T('.'),
                    isprint(row[1]) ? row[1] : _T('.'),
                    isprint(row[2]) ? row[2] : _T('.'),
                    isprint(row[3]) ? row[3] : _T('.'),
                    isprint(row[4]) ? row[4] : _T('.'),
                    isprint(row[5]) ? row[5] : _T('.'),
                    isprint(row[6]) ? row[6] : _T('.'),
                    isprint(row[7]) ? row[7] : _T('.'));
                row.clear();
                _tprintf(_T("                      "));
            }
        }

        if (!row.empty())
        {
            row.resize(8, 0);
            _tprintf(_T("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X | %c %c %c %c %c %c %c %c\r\n"),
                row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7],
                isprint(row[0]) ? row[0] : _T('.'),
                isprint(row[1]) ? row[1] : _T('.'),
                isprint(row[2]) ? row[2] : _T('.'),
                isprint(row[3]) ? row[3] : _T('.'),
                isprint(row[4]) ? row[4] : _T('.'),
                isprint(row[5]) ? row[5] : _T('.'),
                isprint(row[6]) ? row[6] : _T('.'),
                isprint(row[7]) ? row[7] : _T('.'));
            row.clear();
        }
    }
}
void DumpCredential(PCREDENTIAL pCred)
{
    if (pCred)
    {
        DUMP_STRING(pCred, TargetName);
        DUMP_STRING(pCred, UserName);
        DUMP_STRING(pCred, TargetAlias);
        DUMP_STRING(pCred, Comment);
        DUMP_DWORD(pCred, Flags);
        DUMP_DWORD(pCred, Type);
        DUMP_DWORD(pCred, Persist);
        DUMP_DWORD(pCred, AttributeCount);
        DUMP_DWORD(pCred, CredentialBlobSize);
        _tprintf(_T("      CredentialBlob: "));
        DumpBinary(pCred->CredentialBlob, pCred->CredentialBlobSize);
    }
}
LPCTSTR GetFilter(int argc, TCHAR* argv[])
{
    if (argc >= 2)
    {
        return argv[1];
    }

    return NULL;
}

int _tmain(int argc, TCHAR* argv[])
{
   
    LPCTSTR pFilter = GetFilter(argc, argv);

    DWORD dwCount = 0;
    PCREDENTIAL* ppCredentialList = nullptr;
    BOOL bRet = FALSE;
    if (::CredEnumerate(pFilter, 0, &dwCount, &ppCredentialList))
    {
        if (dwCount <= 0)
        {
            _tprintf(_T("No credential found on current context.\r\n"));
        }

        for (int i = 0; i < dwCount; i++)
        {
            _tprintf(_T("--------- Credential: %d\r\n"), i);
            DumpCredential(ppCredentialList[i]);
            _tprintf(_T("\r\n"), i);
        }

        ::CredFree(ppCredentialList);
    }
    else
    {
        _tprintf(_T("No credential exists matching the specified filter in current context."));
    }

    return 0;
}