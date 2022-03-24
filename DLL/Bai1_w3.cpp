#include <tchar.h>
#include <windows.h>
#include <iostream>
using namespace std;

typedef int(__stdcall* f_funci)();
typedef int(__stdcall* f_funci1)(char* );
typedef int(__stdcall* f_funci2)(char*,char*);
wchar_t* toLPCWSTR(const char* charArray);

int main( int argc, char* argv[])
{
    
    HINSTANCE hDLL = LoadLibrary(toLPCWSTR(argv[1]));    
    if (!hDLL) {  
        cout << "could not load the dynamic library" <<endl;
        return EXIT_FAILURE;
    }
    if (argc==3)
    {
        f_funci funcDLL = (f_funci)GetProcAddress(hDLL, argv[2]);
        if (!funcDLL) {
            cout << "could not locate the function" << endl;
            return EXIT_FAILURE;
        }
        funcDLL();
    }
    if (argc == 4)
    {
        f_funci1 funcDLL = (f_funci1)GetProcAddress(hDLL, argv[2]);
        if (!funcDLL) {
            cout << "could not locate the function" << endl;
            return EXIT_FAILURE;
        }
        funcDLL(argv[3]);
    }
    if (argc == 5)
    {
        f_funci2 funcDLL = (f_funci2)GetProcAddress(hDLL, argv[2]);
        if (!funcDLL) {
            cout << "could not locate the function" << endl;
            return EXIT_FAILURE;
        }
        funcDLL(argv[3], argv[4]);
    }


    //return EXIT_SUCCESS;
}

wchar_t* toLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}