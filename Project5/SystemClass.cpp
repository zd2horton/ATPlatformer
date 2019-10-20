#include "SystemClass.h"

SystemClass::SystemClass()
{
	//set input and graphics pointers to null for initialisation
	m_Input = 0;
	m_Graphics = 0;
}

//empty as initialised and deleted in WinMain.cpp and shut down in exit() 
SystemClass::SystemClass(const SystemClass& other)
{
	
}

SystemClass::~SystemClass()
{
	
}

bool SystemClass::Init()
{
	//intialise window
	int s_height = 0;
	int s_width = 0;
	initWindow(s_width, s_height);

	//initialise user input and graphics objects
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}
	m_Input->Init();

	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	bool init_graphics;
	init_graphics = m_Graphics->Init(s_width, s_height, m_hwnd);
	if(!init_graphics)
	{
		return false;
	}
	
	return true;
}

void SystemClass::Exit()
{
	//dispose of graphics and input
	if(m_Graphics)
	{
		m_Graphics->Exit();
		delete m_Graphics;
		m_Graphics = 0;
	}
	
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	shutWindow();
	return;
}

void SystemClass::Run()
{
	MSG message;
	bool finished = false;
	bool result;

	//initialise message system and loop until application quits
	ZeroMemory(&message, sizeof(MSG));
	while(!finished)
	{
		//message handling
		if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//end upon message telling to quit
		if(message.message == WM_QUIT)
		{
			finished = true;
		}
		
		//frame handling until application quits
		else
		{
			result = Frame();
			if(!result)
			{
				finished = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool continue_process;

	//quit if user quits
	if(m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//graphics frame handling
	continue_process = m_Graphics->Frame();
	if(!continue_process)
	{
		return false;
	}
	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		//send key held or released to input for recording
		case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		
		case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		//for other messages
		default:
		{
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}
}


void SystemClass::initWindow(int& s_width, int& s_height)
{
	//returns screen width and height for usage elsewhere
	//windows class, screen settings, x and y position
	WNDCLASSEX wc;
	DEVMODE dmScrSettings;
	int posX;
	int posY;

	//external pointer to this, instance of this 	
	ApplicationHandle = this;
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"DirectX Triangle";

	//default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	
	wc.hInstance = m_hinstance;
	
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	//register window class, get resolution of current hardware and adjust
	//to either fullscreen or an alternate 800 x 600 display
	RegisterClassEx(&wc);
	s_width  = GetSystemMetrics(SM_CXSCREEN);
	s_height = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN)
	{
		memset(&dmScrSettings, 0, sizeof(dmScrSettings));
		dmScrSettings.dmSize = sizeof(dmScrSettings);
		dmScrSettings.dmPelsWidth  = (unsigned long)s_width;
		dmScrSettings.dmPelsHeight = (unsigned long)s_height;
		dmScrSettings.dmBitsPerPel = 32;			
		dmScrSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&dmScrSettings, CDS_FULLSCREEN);
		posX = posY = 0;
	}
	
	else
	{
		s_width  = 800;
		s_height = 600;
		posX = (GetSystemMetrics(SM_CXSCREEN) - s_width)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - s_height) / 2;
	}

	//window creation
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, s_width, s_height, 
							NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	ShowCursor(false);
	return;
}

void SystemClass::shutWindow()
{
	ShowCursor(true);

	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//window and instance removed
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
	ApplicationHandle = NULL;
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	//messages sent here and over to messagehandler
	switch(message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, message, wparam, lparam);
		}
	}
}