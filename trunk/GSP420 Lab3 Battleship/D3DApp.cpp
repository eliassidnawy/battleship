#include "D3DApp.h"

D3DApp* gd3dApp              = 0;
IDirect3DDevice9* gd3dDevice = 0;


// this comment is cai testing svn
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if( gd3dApp != 0 )
		return gd3dApp->msgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance, std::string windowCaption, D3DDEVTYPE devType, DWORD requestedVP)
{
	mMainWindowCaption = windowCaption;
	mDevType = devType;
	mRequestedVP = requestedVP;

	mhAppInst = hInstance;
	mhMainWnd = 0;
	md3dObject = 0;
    mAppPaused = false;
	ZeroMemory(&d3dPP, sizeof(d3dPP));

	initMainWindow();
	initD3D();
}

D3DApp::~D3DApp()
{
	//release objects we allocated memory for
	ReleaseCOM(md3dObject);
	ReleaseCOM(gd3dDevice);
}

HINSTANCE D3DApp::getAppInstance()
{
	return mhAppInst;
}

HWND D3DApp::getMainWindowHandle()
{
	return mhMainWnd;
}

void D3DApp::initMainWindow()
{
	//the first step is to describe the window.  To do this, we use a WNDCLASS structure.
	WNDCLASS wc;
	
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= mhAppInst;
	wc.hIcon			= ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "D3DApp";

	

	//register the window discription we just created with Windows so we can create a window based on that description.
	if(!::RegisterClass(&wc))
	{
		::MessageBox(0, "RegisterClass - Failed", 0, 0);
		::PostQuitMessage(0);
	}

	/* With our window description created and registered, we can create the window.  This function
	   returns a HWND to the created window, which we save in mhMainWnd.
	   Default to a window with a client area rectangle of 800x600. */

	 // Create the window.

	RECT R = {0, 0, 1280, 768};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	mhMainWnd = CreateWindow("D3DApp", mMainWindowCaption.c_str(), WS_OVERLAPPEDWINDOW, 100, 100, 
						R.right, R.bottom, NULL, NULL, mhAppInst, NULL);
	
	if(!mhMainWnd)
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		::PostQuitMessage(0);
	}

	//finally, if everything is ok, we can show the window.  By passing mhMainWnd, these functions will know 
	//what window to operate on.
	::ShowWindow(mhMainWnd, SW_SHOW);
	::UpdateWindow(mhMainWnd);

	// hides the cursor when in window 5/27/2008
	::ShowCursor(false);
}

void D3DApp::initD3D()
{
	/*Step 1: Create the ID3DDirect9 Object
		acquire an interface
		always use D3D_SDK_VERSION so that the application is built against the correct header files
		will return null if it fails
	*/

	md3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	if( !md3dObject )
	{
		MessageBox(0, "Direct3eDCreate9 FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Step 2: Verify hardware support for specified formats in windowed and full screen modes.

	
	


	//next you set the color formats you will use for windowed and full screen mode
	//use GetDisplayMode to get the current settings, which we will use for windowed mode
	D3DDISPLAYMODE mode;
	md3dObject -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

	//check for windowed mode
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, mode.Format, mode.Format, true));

	//check for full screen
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));
	/*if(FAILED(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, mDevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false)))
	{
		MessageBox(0, "FULL SCREEN FAILED", 0,0);
		PostQuitMessage(0);
	}*/

	// Step 3: Check for requested vertex processing and pure device.

	//next we check to see if the graphics card can support the vertex processing we want, this setup 
	//checks for hardware processing, and if they support hardware processing, creates a pure device
	D3DCAPS9 caps;

	//get the info from the default display adapter
	HR(md3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, mDevType, &caps));

	//set vertex processing and pure device
	DWORD devBehaviorFlags = 0;

	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		devBehaviorFlags |= mRequestedVP;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//if pure device and HW T&L supported
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE && devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;
	

	// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.
	d3dPP.BackBufferWidth = 1280;
	d3dPP.BackBufferHeight = 768;
	d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dPP.BackBufferCount = 1;
	d3dPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dPP.MultiSampleQuality = 0;
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.hDeviceWindow = mhMainWnd;
	d3dPP.Windowed = true;
	d3dPP.EnableAutoDepthStencil = true;
	d3dPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dPP.Flags = 0;
	d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	


	// Step 5: Create the device.
	//since everything is initialized, get a d3ddevice
	HR(md3dObject->CreateDevice(
		D3DADAPTER_DEFAULT, //primary adapter
		mDevType,		//device type
		mhMainWnd,			//window association with device
		devBehaviorFlags,	//vertex processing/pure device
		&d3dPP,			//present parameters
		&gd3dDevice));		//return created device
}

int D3DApp::run()
{
	MSG msg;

	//set msg to null 
	msg.message = WM_NULL;

	//QueryPerformanceFrequency and QueryPerformanceCounter will get the counts per second and the counts, 
	//respectively, which will help us calculate 
	//how many seconds have elapsed between frames
	_int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	_int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	//loop until we get a WM_QUIT msg.  GetMessage() only returns 0 when a WM_QUIT msg is received.
	while(msg.message != WM_QUIT)
	{
		//if there are windows messages, then process them
		if(PeekMessage(&msg, 0,0,0,PM_REMOVE))
		{
			//translate the msg and then dispatch it
			//to the appropriate window procedure
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		//otherwise, do animation game stuff
		else
		{
			//if the applications is paused, give some CPU cycles to other applications
			//then go to next frame
			if(mAppPaused)
			{
				Sleep(20);
				continue;
			}
			
			//make sure the device is ready to be used
			if(!isDeviceLost())
			{
				_int64 currTimeStamp = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
				float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

				//update the scene	// this also polls DInput
				updateScene(dt);

				//draw the scene
				drawScene();

				/*Prepare for the next iteration: The current time
				  stamp becomes the previous time stamp for the
				  next iteration*/
				prevTimeStamp = currTimeStamp;
			}
		}
	}

	return (int)msg.wParam;
}

LRESULT D3DApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Is the application in a minimized or maximized state?
	static bool minOrMaxed = false;

	RECT clientRect = {0, 0, 0, 0};

	//handle some specific messages
	switch(msg)
	{
	
		// WM_ACTIVE is sent when the window is activated or deactivated.
		// We pause the game when the main window is deactivated and 
		// unpause it when it becomes active.
		case WM_ACTIVATE:
			if( LOWORD(wParam) == WA_INACTIVE )
				mAppPaused = true;
			else
				mAppPaused = false;
			return 0;

		// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			if( gd3dDevice )
			{
				d3dPP.BackBufferWidth  = LOWORD(lParam);
				d3dPP.BackBufferHeight = HIWORD(lParam);

				if( wParam == SIZE_MINIMIZED )
				{
					mAppPaused = true;
					minOrMaxed = true;
				}
				else if( wParam == SIZE_MAXIMIZED )
				{
					mAppPaused = false;
					minOrMaxed = true;
					onLostDevice();
					HR(gd3dDevice->Reset(&d3dPP));
					onResetDevice();
				}
				// Restored is any resize that is not a minimize or maximize.
				// For example, restoring the window to its default size
				// after a minimize or maximize, or from dragging the resize
				// bars.
				else if( wParam == SIZE_RESTORED )
				{
					mAppPaused = false;

					// Are we restoring from a mimimized or maximized state, 
					// and are we in windowed mode?  Do not execute this code if 
					// we are restoring to full screen mode.
					if( minOrMaxed && d3dPP.Windowed )
					{
						onLostDevice();
						HR(gd3dDevice->Reset(&d3dPP));
						onResetDevice();
					}
					else
					{
						// No, which implies the user is resizing by dragging
						// the resize bars.  However, we do not reset the device
						// here because as the user continuously drags the resize
						// bars, a stream of WM_SIZE messages is sent to the window,
						// and it would be pointless (and slow) to reset for each
						// WM_SIZE message received from dragging the resize bars.
						// So instead, we reset after the user is done resizing the
						// window and releases the resize bars, which sends a
						// WM_EXITSIZEMOVE message.
					}
					minOrMaxed = false;
				}
			}
			return 0;


		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			GetClientRect(mhMainWnd, &clientRect);
			d3dPP.BackBufferWidth  = clientRect.right;
			d3dPP.BackBufferHeight = clientRect.bottom;
			onLostDevice();
			HR(gd3dDevice->Reset(&d3dPP));
			onResetDevice();

			return 0;

		// WM_CLOSE is sent when the user presses the 'X' button in the
		// caption bar menu.
		case WM_CLOSE:
			DestroyWindow(mhMainWnd);
			return 0;

		//if escape was pressed, destroy main window
		case WM_KEYDOWN:
		
			if( wParam == VK_ESCAPE )
				PostQuitMessage(0);
			else if( wParam == 'F' )
			{
				//mAppPaused = true;
				// causes a bug right now?
				enableFullScreenMode(true);
			}
			else if( wParam == VK_F1 )
			{
				//mAppPaused = true;
				enableFullScreenMode(false);
			}

			return 0;

			//in case of a destroy message, then send a quit msg, which will terminate the loop
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
	}

	//forward any other msg's to the default window procedure
	return ::DefWindowProc(mhMainWnd, msg, wParam, lParam);
}

bool D3DApp::isDeviceLost()
{
	//returns true if lost, false otherwise
	//get the state of the graphics device
	HRESULT hr = gd3dDevice->TestCooperativeLevel();

	//if the device is lost and cannot be reset yet, then
	//sleep for a bit and we'll try again on the next message loop cycle

	if(hr == D3DERR_DEVICELOST)
	{
		Sleep(20);
		return true;
	}
	else if(hr == D3DERR_DRIVERINTERNALERROR )
	{
		
		MessageBox(0, "Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	//the device is lost but we can reset it and restore it
	else if(hr == D3DERR_DEVICENOTRESET)
	{
		onLostDevice();
		HR(gd3dDevice->Reset(&d3dPP));
		onResetDevice();
		//not lost anymore
		return false;
	}
	else
		//not lost anymore
		return false;
}

/*bool D3DApp::isDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = gd3dDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if( hr == D3DERR_DEVICELOST )
	{
		Sleep(20);
		return true;
	}
	// Driver error, exit.
	else if( hr == D3DERR_DRIVERINTERNALERROR )
	{
		MessageBox(0, "Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// The device is lost but we can reset and restore it.
	else if( hr == D3DERR_DEVICENOTRESET )
	{
		onLostDevice();
		HR(gd3dDevice->Reset(&d3dPP));
		onResetDevice();
		return false;
	}
	else
		return false;
}*/

/* NOTES ON FULL SCREEN MODE THUS FAR 
1. AS OF NOW, WE ONLY SUPPORT THE CURRENT RESOLUTION, WE DO NOT CHANGE RESOLUTIONS
2. WHEN WE SWITCH TO FULL SCREEN, WE SWITCH TO A WINDOW THAT HAS NO BORDER, SYSTEM MENU, MIN/MAX ETC
3. REMEMBER THAT THE WINDOW SIZE IS LARGER THAN THE ACTUAL CLIENT WINDOW (DRAWING AREA) THEREFORE, TO HAVE A 
   DRAWING AREA THAT IS 800 BY 600, WE NEED TO MAKE A LARGER WINDOW.  THE ADJUSTWINDOWRECT FUNTION CALCULATES 
   HOW MUCH BIGGER THE WINDOW NEEDS TO BE BASED OFF THE STYLE OF WINDOW AND DESIRED CLIENT RECTANGLE WE WANT. 
void D3DApp::enableFullScreen(bool enabled)
{
	//switch to full screen mode
	if(enabled)
	{
		//check to see if we are already in full screen mode
		if( !d3dPP.Windowed )
		{
			//mAppPaused = false;
			return;
		}

		//Get the current screen resolution
		int width = GetSystemMetrics(SM_CXFULLSCREEN);
		int height = GetSystemMetrics(SM_CYFULLSCREEN);

		//set the present parameters to this width and height, set backbuffer format, and set windowed to false
		d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dPP.BackBufferHeight = height;
		d3dPP.BackBufferWidth = width;
		d3dPP.Windowed = false;

		may add this stuff back in later, right now it is causing an error, so i am going to email
		  the author and see what he says lol
		//change the window style to one that is more full screen user friendly
		//SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_POPUP);

		//if we call SetWIndowLongPtr, MSDN states we have to call SetWindowPos for the change to take effect
		//we have to call it anyway so we can update the window dimensions
		//SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW );
		
	}
	//switch to windowed mode
	else
	{
		//are we in windowed mode?
		if( d3dPP.Windowed )
		{
			//mAppPaused = false;
			return;
		}

		//default to a client rectangle of 800X600
		RECT R = {0, 0, 1280, 800};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dPP.BackBufferWidth = 1280;
		d3dPP.BackBufferHeight = 800;
		d3dPP.Windowed = true;

		//change the window style to one that is windowed friendly
		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	
		//again, if we call setwindowlongptr, we must call setwindowpos to update the window
		SetWindowPos(mhMainWnd, HWND_TOP, 100, 100, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	//Reset the device with the changes
	onLostDevice();
	HR(gd3dDevice->Reset(&d3dPP));
	onResetDevice();

	mAppPaused = false;
}*/

void D3DApp::enableFullScreenMode(bool enable)
{
	// Switch to fullscreen mode.
	if( enable )
	{
		// Are we already in fullscreen mode?
		if( !d3dPP.Windowed ) 
			return;

		const int width  = GetSystemMetrics(SM_CXSCREEN);
		const int height = GetSystemMetrics(SM_CYSCREEN);
		//const int width  = 1280;
		//const int height = 1024;
	
		//char buffer[256];

		//MessageBox(mhMainWnd, itoa(width, buffer, 10), "TEST", 0);
		d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dPP.BackBufferWidth  = width;
		d3dPP.BackBufferHeight = height;
		d3dPP.Windowed         = false;

		// Change the window style to a more fullscreen friendly style.
		//SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_POPUP);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		//SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);	
	}
	// Switch to windowed mode.
	else
	{
		// Are we already in windowed mode?
		if( d3dPP.Windowed ) 
			return;

		RECT R = {0, 0, 1280, 768};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dPP.BackBufferWidth  = 1280;
		d3dPP.BackBufferHeight = 768;
		d3dPP.Windowed         = true;
	
		// Change the window style to a more windowed friendly style.
		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(mhMainWnd, HWND_TOP, 100, 100, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	// Reset the device with the changes.
	onLostDevice();
	HR(gd3dDevice->Reset(&d3dPP));
	onResetDevice();
}
	