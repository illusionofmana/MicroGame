#pragma once
#define _MAIN_H_WIN

//global macros
#define ArrayEX vector
#define WIN32_LEAN_AND_MEAN
#define INITGRAFX 0
#define SHUTDOWNGRAFX 1
#define STARTFRAME 2
#define ENDFRAME 3
#define DRAW3D 4

//global interfaces
#include <ctime>
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

//extensions
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <string>

//-------------Direct-X-11--------------------------------------------------
/*do not forget to add these into include after installing the DX SDK base (you can also build this SDK into your game source)
in Visual Studio go to -> Debug -> "YOUR PROGRAM NAMD"'s Properties... and open its window.
When the window opens change "Configuration:" and "Platform": to "All Configurations" and "All Platforms" respectively.
go to the "Linker" tab on the left and open it, click on the "Input" tab then click on the dropdown button for the text field
next to the "Additional Dependancies" option and click "Edit...". A new window will open up, Copy this exacly as it is in the
empty text field at the top of that window...
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86\d3dx11.lib;
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86\d3dx10.lib;
Press "OK" to close that window...
While still in the Program Properties window move up to the "VC++ Directories" tab on the left near the top and click on it.
next to the "Include Directories" option and click "Edit...". A new window will open up, Copy this exacly as it is in the
empty text field at the top of that window...
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
Press the ENTER key then press the "OK" button to close that window...
While its still up, press "Apply" button then the "OK" button at the bottom of your Program Properties window.
You are now all set to go back to programming!
NOTE 1: These directories may be different or this step may not be needed depending on where you have installed your
"Microsoft DirectX SDK" Copy or if you are using the built in DXSource found in your Windows System32 or SysWOW64 directories.
NOTE 2: Other Extensions may require this method as well such as Direct Input and Direct Sound.
*/
//extensions
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
//libraries
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
//include
#include "DXCore.h"
//--------------------------------------------------------------------------

//global variables
extern int GrafX_API;
extern int SCREENW;
extern int SCREENH;
extern int BBUFFERW;
extern int BBUFFERH;
extern int reset;
extern bool gameover;
extern DWORD DebugVal;

//namespaces
using namespace std;

//global class lookup
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void ResetWindow(HWND, int&, int&);

public:


private:
	HINSTANCE m_hinstance;
	HWND parent_hwnd;
	HWND m_hwnd;

};

//non protected universally accessable objects
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass*ApplicationHandle = 0;
bool InitProgInst(HWND hwnd);
int RunPrgInst(HWND hwnd);
void ShutDownPrgInst(HWND hwnd);
void GrafXCore(HWND hwnd, int gmessage);
void IOCore(HWND hwnd, int iomessage, DWORD passable = 0x00000000);
void SoundCore(HWND hwnd, int smessage, string soundsource = "");
void MicroAPI(HWND hwnd, int smessage);
