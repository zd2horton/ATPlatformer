#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN //reduces win32 header file size through
						    //excluding lesser used APIs
#include <Windows.h> //used for window functions

#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& other);
	~SystemClass();

	bool Init();
	void Exit();
	void Run();

	//handles system messages
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, 
							WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void initWindow(int& s_width, int& s_height);
	void shutWindow();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	//pointers
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

//redirect windows messaging to messagehandler
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, 
							WPARAM wparam, LPARAM lparam);
static SystemClass* ApplicationHandle = 0;

#endif // !_SYSTEMCLASS_H