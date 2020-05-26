#include "Main.h"
//interfaces

//extensions

//variables
	/*GraphX_API operation
	0 = run in DirectX 11 Mode (windows option)
	1 = RESERVED FOR FUTURE (multi system option)
	2 = RESERVED FOR FUTURE (apple option)
	*/
	extern int GrafX_API = 0;
	extern int SCREENW = 608;
	extern int SCREENH = 488;
	extern int BBUFFERW = 304;
	extern int BBUFFERH = 244;
	/*reset stat operation
	0 = run no reset
	1 = run and reset
	2 = post reset (in process of resetting window)
	10 = end program*/
	extern int reset = 0;
	/*gameover state operation
	0 = run
	1 = end program*/
	extern bool gameover = false;
	extern DWORD DebugVal = 0;
	//base screen config cache (for multi monitors)
	int loadscreensize = 0;
	//the app name
	const string APPTITLE = "Adelaide's Quest";

//global class instances
	extern DXEngine* DX = nullptr;

//entrypont
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//core program loop
	do
	{
		//establish the program main class
		SystemClass* System;
		bool result;
		//create the system object
		System = new SystemClass;
		//check if system was created correctly
		if (!System)
		{
			//if no, send error
			DebugVal = 0xDEADBEEF;
		}
		//initialize the program
		result = System->Initialize();
		if (result)
		{
			System->Run();
		}
		//once the system has exited its operation loop, shut down and destroy the application container
		System->Shutdown();
		delete System;
		System = 0;
		//check the reset flag
	} while (reset < 10);

	return 0;
}

//startup function for systemClass
SystemClass::SystemClass()
{
	//init drivers and vars
	//ie DirectX, OpenGL, DirectInput, ... 
}
//constructor for system class
SystemClass::SystemClass(const SystemClass& other)
{

}
//destructor for systemClass
SystemClass::~SystemClass()
{

}
//System Init
bool SystemClass::Initialize()
{
	bool result = true;
	//load from config file

	//placeholder loading in values
	loadscreensize = SCREENW;
	GrafX_API = 0;

	//init the window
	InitializeWindows(SCREENW, SCREENH);

	//run the init prog
	result = InitProgInst(m_hwnd);

	return result;
}
//contians the main program loop
void SystemClass::Run()
{
	MSG msg;
	bool result;
	 //init the message struct
	msg.message = DebugVal;
	ZeroMemory(&msg, sizeof(MSG));
	while (!gameover)
	{
		//internal message handler
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			//false message catching
			if (reset == 2)
			{
				msg.message = 0x0113;
				reset = 0;
			}
			DispatchMessage(&msg);
		}
		//kill prog
		if (msg.message == WM_QUIT)
		{
			gameover = true;
			reset = 10;
		}
		else //game loop
		{
			//game processor
			result = Frame();
			//reset handler
			if (reset == 1)
			{
				ResetWindow(m_hwnd, SCREENW, SCREENH);
				reset = 2;
				gameover = false;
			}
		}
	}
	//return to loop safety
	if (reset == 1)
	{
		gameover = false;
		reset = 2;
	}

	return;
}
//game entry point
bool SystemClass::Frame()
{
	bool L_Result = true;
	//game entry point
	RunPrgInst(m_hwnd);
	//call function to start game

	return L_Result = true;
}
//System Shutdown (does not kill the program)
void SystemClass::Shutdown()
{
	//shutdown drivers

	//shutdown lingering classes
	ShutdownWindows();
	//if logging, create the log file
	return;
}
//message handler interface
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}
//create the window structure
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	int posX, posY;
	//get the external application pointer
	ApplicationHandle = this;
	//get the instance of this application and attach the module handle to it
	m_hinstance = GetModuleHandle(NULL);
	//give the application its parameters
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = NULL;
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	//register the class with all of the parameters entered
	RegisterClassEx(&wc);

	//----------------------------------------------------------------------------------------------------------------------
	screenWidth = (int)loadscreensize + 16;
	screenHeight = (int)ceil((loadscreensize * 576) / 720) + 39;
	posX = ((GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2) + (GetSystemMetrics(SM_CXSCREEN) * 0/*This is the screen number, replace with a variable later*/);
	posY = ((GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2);
	//set the cursor
	ShowCursor(true);
	//buld the window appearance
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		APPTITLE.c_str(),
		APPTITLE.c_str(),
		WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_BORDER,
		posX, posY,
		screenWidth, screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL);
	//apply the set position
	SetWindowPos(m_hwnd, HWND_TOP, posX, posY, screenWidth, screenHeight, SWP_NOSIZE);
	//bring the window up front and in focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	//----------------------------------------------------------------------------------------------------------------------
	return;
}
//shutdown the window
void SystemClass::ShutdownWindows()
{
	//make sure sursor is showing
	ShowCursor(true);
	//fullscreen display reset

	//remove the base window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	//remove the application instance
	UnregisterClass(APPTITLE.c_str(), m_hinstance);
	m_hinstance = NULL;
	//release the application pointer
	ApplicationHandle = NULL;

	return;
}
//reset the window
void SystemClass::ResetWindow(HWND hwnd, int& screenWidth, int& screenHeight)
{
	int posX = 0;
	int posY = 0;
	//----------------------------------------------------------------------------------------------------------------------
	screenWidth = (int)loadscreensize + 16;
	screenHeight = (int)ceil((loadscreensize * 576) / 720) + 39;
	posX = ((GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2) + (GetSystemMetrics(SM_CXSCREEN) * 0/*This is the screen number, replace with a variable later*/);
	posY = ((GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2);
	//set the cursor
	ShowCursor(true);
	//buld the window appearance
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		APPTITLE.c_str(),
		APPTITLE.c_str(),
		WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_BORDER,
		posX, posY,
		screenWidth, screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL);
	//apply the set position
	SetWindowPos(m_hwnd, HWND_TOP, posX, posY, screenWidth, screenHeight, SWP_NOSIZE);
	MoveWindow(hwnd, posX, posY, screenWidth, screenHeight, true);

	//----------------------------------------------------------------------------------------------------------------------
	return;
}
//window interface and UI function controller
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	LRESULT lresult;
	switch (umessage)
	{
		//check if the window is being closed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//check if the window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		//enable/disable window dragging if the window is fullscreen
		case WM_NCHITTEST:
		{
			//if in fullscreen mode perform a hit check and deny the window drag

			//if in windowd mode allow hit check to pass
			lresult = HTNOWHERE;
			return lresult;
		}
		//desnd all other commands here
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}

	}

}

void GrafXCore(HWND hwnd, int gmessage)
{
	switch (gmessage)
	{
		case INITGRAFX:
		{
			//create the system object
			DX = new DXEngine;
			//check if system was created correctly
			if (!DX)
			{
				//if no, send error
				DebugVal = 0xDD0F0000;
			}
			DX->InitD3D(hwnd);
			return;
		}
		case SHUTDOWNGRAFX:
		{
			//call the shutdown function
			DX->ShutDownD3D();
			return;
		}
		case STARTFRAME:
		{
			//set up to start compiling the frame
			DX->StartFrame();
			return;
		}
		case ENDFRAME:
		{
			//end the frame compilation and send it to the driver
			DX->RenderFrame();
			return;
		}
		case DRAW3D:
		{
			//draw verteces
			DX->DrawVertex(4, 0);											//this has temp information
			return;
		}
	}
	return;
}
void IOCore(HWND hwnd, int iomessage, DWORD passable)
{

}
void SoundCore(HWND hwnd, int smessage, string soundsource)
{

}
void MicroAPI(HWND hwnd, int smessage)
{

}
