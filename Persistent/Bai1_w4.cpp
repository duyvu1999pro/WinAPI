#include <Windows.h>
#include <iostream>
using namespace std;

HKEY OpenRegistry(HKEY root, const char* path);
void addValueIntoRegistry(HKEY hKey, const char* value, const char* path);

int main(int argc, char* argv[])
{
    if (argc==3)
    {
        HKEY hKey = OpenRegistry(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
        if (hKey)
        {
            addValueIntoRegistry(hKey, argv[1], argv[2]);
        }
    }
    return 1;
}

HKEY OpenRegistry(HKEY root, const char* path)
{
    HKEY hKey;
    LONG openRes = RegOpenKeyExA(root, path, NULL, KEY_ALL_ACCESS, &hKey);

    if (openRes == ERROR_SUCCESS)
    {
        cout << "Success opening key: \n";
        return hKey;
    }
    printf("Error opening key.");
    return hKey;
}
void addValueIntoRegistry(HKEY hKey, const char* value, const char* path)
{
    LONG setRes = RegSetValueExA(hKey, value, 0, REG_SZ, (LPBYTE)path, strlen(path));

    if (setRes == ERROR_SUCCESS) {
        printf("Success writing to Registry.");
    }
    else {
        printf("Error writing to Registry.");
    }

    LONG closeOut = RegCloseKey(hKey);
}
