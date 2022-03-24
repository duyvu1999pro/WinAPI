#include <stdio.h>
#include <Windows.h>
#include <iostream>

using namespace std;

bool is_number(char k[]);
wchar_t* toLPCWSTR(const char* charArray);
bool hookMethod(const char* pathDLL, const char* funcName, int idHook, int threadID);
bool remoteThreadMethod(int targetPid, const char* pathDLL);

int main(int argc, TCHAR* argv[])
{
	//	cout << "nhap Pid muc tieu: ";
	//char pid[30];
	//cin >> pid;
	//if (is_number(pid))
	//{
	//	char dll[300];
	//	cout << "nhap DLL path: ";
	//	cin >> dll;
	//	remoteThreadMethod(atoi(pid),dll);
	//	//hookMethod("D:\\tempDLL.dll", "meconnect",2,0);
	//}	
	cout<<remoteThreadMethod(12908, "D:\\tempDLL.dll");
	return 0;
}

bool remoteThreadMethod(int targetPid, const char* pathDLL)
{
	//target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, targetPid);
	if (!hProcess)
		return false;
	//		
	
	char dllPath[4096] = { 0 };
	GetFullPathNameA(pathDLL, 4096, dllPath, NULL);

	//get size dll
	HANDLE hDll = CreateFileA(dllPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int dllSize = GetFileSize(hDll, NULL);
	//	
	LPVOID memory = VirtualAllocEx(hProcess, NULL, dllSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!memory)
		return false;
	//read dll
	unsigned long BytesRead;
	//LPVOID lpBuffer = HeapAlloc(GetProcessHeap(), 0, dllSize); 
	LPVOID lpBuffer = GlobalLock(GlobalAlloc(GMEM_MOVEABLE, dllSize));
	ReadFile(hDll, lpBuffer, dllSize, &BytesRead, NULL);
	//
	if (!WriteProcessMemory(hProcess, memory, dllPath, strlen(dllPath), NULL))
		return false;
	
	//PTHREAD_START_ROUTINE loadLibraryFunction = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	PTHREAD_START_ROUTINE loadLibraryFunction = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if (!loadLibraryFunction)
		return false;
	
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibraryFunction, memory, 0, NULL);
	if (!hThread)
		return false;
	WaitForSingleObject(hThread, INFINITE);
	getchar();
	VirtualFreeEx(hProcess, memory, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}


bool hookMethod(const char* pathDLL, const char* funcName, int idHook, int threadID)
{
	HMODULE hDLL = LoadLibraryA(pathDLL);
	if (hDLL == NULL) {
		printf("The DLL could not be found.\n");
		return false;
	}

	HOOKPROC addrFunc = (HOOKPROC)GetProcAddress(hDLL, funcName);
	if (addrFunc == NULL) {
		printf("The function was not found.\n");
		return false;
	}
	HHOOK handle = SetWindowsHookEx(idHook, addrFunc, hDLL, threadID);
	if (handle == NULL) {
		printf("Could not be hooked.\n");
		return false;
	}
	//UnhookWindowsHookEx(handle);
	return true;
	
}
bool is_number(char k[])
{
	for (int i = 0; i < sizeof(k) / sizeof(k[0]); i++)
	{
		if (!isdigit(k[i]))
		{
			return false;
		}
	}
	return true;
}
wchar_t* toLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}
