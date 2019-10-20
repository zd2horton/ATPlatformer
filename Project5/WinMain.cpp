#include "SystemClass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					PSTR pScmdline, int iCmdshow)
{
	//system class created, initialised and run
	SystemClass* System;
	bool result;
	
	System = new SystemClass;
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Init();
	if(result)
	{
		System->Run();
	}

	//exit and dispose of system after usage complete
	System->Exit();
	delete System;
	System = 0;
	return 0;
}