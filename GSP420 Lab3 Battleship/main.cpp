
//precompiled headers to include
#include <tchar.h>


#include <crtdbg.h>

//our classes and stuff to include
#include "d3dApp.h"
#include "BallSmackers.h"

	
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	

	srand ( time(NULL) );

	//this is the constructor for our class, right now about the only thing we shd change is the title of the 
	//window
	BallSmackers app(hInstance, "Battleship", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	//Configure DInput here
	DInput di(DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	gDInput = &di;

	
	return gd3dApp->run();

}