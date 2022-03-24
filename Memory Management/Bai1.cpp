#include <winsock2.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <tchar.h>
#include <iomanip>



#pragma comment(lib, "iphlpapi.lib")





#define INFO_BUFFER_SIZE 32767


using namespace std;

void print(const char* header, TCHAR data[]);
void GetOS();
void GetNetworkAdapter();
void GetSystemInfo();

int main()
{
        GetOS();      
        GetSystemInfo();
        GetNetworkAdapter();
   
   
       //TIME_ZONE_INFORMATION TimeZoneInfo;
       // GetTimeZoneInformation(&TimeZoneInfo);
      
       // cout << TimeZoneInfo.StandardDate.;
  
    return 0;
}

void print(const char* header, TCHAR data[])
{
    cout << endl << setw(30) << left << header;
    _tprintf(TEXT("%s"), data);
}
void GetOS()
{
    NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osInfo;
    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (NULL != RtlGetVersion)
    {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);
    }
    cout << endl << setw(30) << left << "OS Version:";
    printf("%u.%u.%u", osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.dwBuildNumber);
    DWORD dwPInfo = NULL;

    GetProductInfo(osInfo.dwMajorVersion, osInfo.dwMinorVersion, 0, 0, &dwPInfo);
    cout << endl << setw(30) << left << "OS Name:" << "";
    switch (dwPInfo) {
    case PRODUCT_ULTIMATE:
        cout << "PRODUCT ULTIMATE";
        break;
    case PRODUCT_HOME_BASIC:
        cout << "HOME BASIC";
        break;
    case PRODUCT_HOME_PREMIUM:
        cout << "HOME PREMIUM";
        break;
    case PRODUCT_PROFESSIONAL:
        cout << "PROFESSIONAL";
        break;
    case PRODUCT_STARTER:
        cout << "STARTER";
        break;
    case PRODUCT_STANDARD_SERVER:
        cout << "STANDARD SERVER";
        break;
    case PRODUCT_STANDARD_SERVER_CORE:
        cout << "STANDARD SERVER CORE";
        break;
    case PRODUCT_STANDARD_SERVER_CORE_V:
        break;
    case PRODUCT_STANDARD_SERVER_V:
        break;

    }

}
void GetSystemInfo()
{
    TCHAR  infoBuf[INFO_BUFFER_SIZE] = { 0 };
    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    GetComputerName(infoBuf, &bufCharCount);
    print("Host name:", infoBuf);
    GetUserName(infoBuf, &bufCharCount);
    print("User name:", infoBuf);
    GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE);
    print("System Directory:", infoBuf);
    GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE);
    print("Windows Directory:", infoBuf);

    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    cout << endl << setw(30) << left << "OEM ID:" << siSysInfo.dwOemId;
    cout << endl << setw(30) << left << "Number of processors:" << siSysInfo.dwNumberOfProcessors;
    cout << endl << setw(30) << left << "Page size:" << siSysInfo.dwPageSize;
    cout << endl << setw(30) << left << "Processor type:" << siSysInfo.dwProcessorType;
    cout << endl << setw(30) << left << "Minimum application address:" << siSysInfo.lpMinimumApplicationAddress;
    cout << endl << setw(30) << left << "Maximum application address:" << siSysInfo.lpMaximumApplicationAddress;
    cout << endl << setw(30) << left << "Active processor mask:" << siSysInfo.dwActiveProcessorMask;
}
void GetNetworkAdapter()
{

    cout << "\n\nNetwork Card(s) :";
    IP_ADAPTER_INFO* pAdapterInfo;
    ULONG            ulOutBufLen;
    DWORD            dwRetVal;
    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
    }
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS) {
        printf("GetAdaptersInfo call failed with %d\n", dwRetVal);
    }

    PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
    int dem = 1;
    while (pAdapter) {


        printf("\n[0%d]: %s", dem, pAdapter->Description);
        cout << endl << setw(30) << left << "Adapter Name:" << pAdapter->AdapterName;
        cout << endl << setw(30) << left << "Adapter Addr:";
        for (UINT i = 0; i < pAdapter->AddressLength; i++) {
            if (i == (pAdapter->AddressLength - 1))
                printf("%.2X\n", (int)pAdapter->Address[i]);
            else
                printf("%.2X-", (int)pAdapter->Address[i]);
        }
        cout << setw(30) << left << "IP Address:" << pAdapter->IpAddressList.IpAddress.String;
        cout << endl << setw(30) << left << "IP Mask:" << pAdapter->IpAddressList.IpMask.String;
        cout << endl << setw(30) << left << "Gateway:" << pAdapter->GatewayList.IpAddress.String;

        if (pAdapter->DhcpEnabled) {
            cout << endl << setw(30) << left << "DHCP Enabled: Yes";
            cout << endl << setw(30) << left << "DHCP Server:" << pAdapter->DhcpServer.IpAddress.String;
        }
        else
            cout << endl << setw(30) << left << "DHCP Enabled: No";


        pAdapter = pAdapter->Next;
        dem++;
    }
    if (pAdapterInfo)
        free(pAdapterInfo);
}
