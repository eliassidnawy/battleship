#include "DInput.h"
#include "d3dUtil.h"
#include "d3dApp.h"
#include <commctrl.h>
#include <basetsd.h>

// Changes made by zach 01/28/08
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

IDirectInputDevice8		*mJoystick[4];


DIJOYSTATE2				js;


DIDEVCAPS				dcJoystick;

IDirectInput8			*mDInput;
IDirectInputDevice8		*mKeyboard;
char					mKeyboardState[256];

/////////////////////////////////////////////////////////////////////////
// CAI - from chapter 9 of directx book attempt to put in mouse 5/27/08
/////////////////////////////////////////////////////////////////////////
//DIMOUSESTATE mouseState;
//
//LONG curX;
//LONG curY;
//
//#define BUTTONDOWN(name, key) (name.rgbButtons[key] & 0x80) 
//
//enum { NONE=0,
//	   LEFT_ARROW,
//	   RIGHT_ARROW,
//	   CIRCLE
//};
/////////////////////////////////////////////////////////////////////////

int Count = 0;

DInput* gDInput = 0;

DInput::DInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags, DWORD joystickCoopFlags)
{
	//clear the memory
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));
	ZeroMemory(&js, sizeof(js));

//	ZeroMemory(&js2, sizeof(js2));

	//create Direct Input Object
	HR(DirectInput8Create(gd3dApp->getAppInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDInput, 0));

	//Create Direct Input Device (keyboard), then set parameters and acquire
	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(gd3dApp->getMainWindowHandle(), keyboardCoopFlags));
	HR(mKeyboard->Acquire());

	//Same as above but for mouse
	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gd3dApp->getMainWindowHandle(), mouseCoopFlags));

	HR(mMouse->Acquire());

	HR(mDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY));

/*	if(mJoystick[0] == NULL)
	{
		MessageBox(gd3dApp->getMainWindowHandle(), "No Joystick 1 Found", "Error", 0);
		//PostQuitMessage(0);
	}
	else*/
	if(mJoystick[0] != NULL)
	{
		HR(mJoystick[0]->SetDataFormat(&c_dfDIJoystick2));
		HR(mJoystick[0]->SetCooperativeLevel(gd3dApp->getMainWindowHandle(), joystickCoopFlags));
		dcJoystick.dwSize = sizeof(DIDEVCAPS);
		HR(mJoystick[0]->GetCapabilities(&dcJoystick));
		HR(mJoystick[0]->Acquire());
	}

	//if(mJoystick[1] == NULL)
	//	MessageBox(gd3dApp->getMainWindowHandle(), "No Joystick 2 Found", "Error", 0);

	
//	HR(mJoystick[1]->SetDataFormat(&c_dfDIJoystick2));
//	HR(mJoystick[1]->SetCooperativeLevel(gd3dApp->getMainWindowHandle(), joystickCoopFlags));
//	dcJoystick2.dwSize = sizeof(DIDEVCAPS);
//HR(mJoystick[1]->GetCapabilities(&dcJoystick2));
//HR(mJoystick[1]->Acquire());
}

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext)
{


	HRESULT hr;

    // Obtain an interface to the enumerated joystick.
	hr = mDInput->CreateDevice(pdidInstance->guidInstance, &mJoystick[Count], NULL);
	if(FAILED(hr)) 
		return DIENUM_CONTINUE;

	else
	{
		Count++;
		if (Count >= 1)
		{
			return DIENUM_STOP;
		}
	}

    return DIENUM_CONTINUE;
}

void DInput::poll()
{
	//poll the keyboard, find out what state keys are in
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);

	if(FAILED(hr))
	{
		//keyboard lost, zero out keyboard data structure
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		//try to acquire for next time we poll
		hr = mKeyboard->Acquire();
	}

	//poll mouse
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState);

	if(FAILED(hr))
	{
		//mouse lost, zero out data structure
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		//try and acquire for next poll
		mMouse->Acquire();
	}
}

void DInput::pollJoystick()
{
	HRESULT hr;
//	HRESULT hr2;
	hr = mJoystick[0]->Poll();
//	hr2 = mJoystick[1]->Poll();


	if(FAILED(hr))
	{
		//not sure if this is needed
		//joystick lost, zero our data structure
		ZeroMemory(&js, sizeof(js));
		//ZeroMemory(&js2, sizeof(js2));
		//try and acquire for next poll
		mJoystick[0]->Poll();

		//try and acquire for next poll
		//mJoystick[1]->Poll();

	}
	//if(FAILED(hr2))
	//{
	//	//not sure if this is needed
	//	//joystick lost, zero our data structure
	//	//ZeroMemory(&js, sizeof(js));
	//	ZeroMemory(&js2, sizeof(js2));
	//	//try and acquire for next poll
	//	//mJoystick[0]->Poll();

	//	//try and acquire for next poll
	//	mJoystick[1]->Poll();

	//}
}

void DInput::onLostDevice()
{
	HRESULT hr;
	ZeroMemory(&js, sizeof(js));

	if(mJoystick[0] != NULL)
		hr = mJoystick[0]->Acquire();

	//if(mJoystick[1] != NULL)
	//	hr = mJoystick[1]->Acquire();
}

bool DInput::keyDown(unsigned char key)
{
	// for debounce
	bool relevantState;

	//performing a bitwise operation to see if key is down, if the bits 0x80 are set, the key is 
	//considered to be pressed

	mCurrKeyboardState[key] = ((mKeyboardState[key] & 0x80) != 0);
	
	relevantState = (mCurrKeyboardState[key] != mLastKeyboardState[key]) && (mLastKeyboardState[key] == 0);
	
	mLastKeyboardState[key] = ((mKeyboardState[key] & 0x80) != 0);

	//return ((mKeyboardState[key] & 0x80) != 0);
	return relevantState;
}

bool DInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}
DInput::~DInput()
{
	//release Direct Input Object
	ReleaseCOM(mDInput);
	
	//unacquire devices, give them back to the system!!!
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	
	if(mJoystick[0] != NULL)
	{
		mJoystick[0]->Unacquire();
		ReleaseCOM(mJoystick[0]);
	}

//	mJoystick[1]->Unacquire();

	//release the devices
	ReleaseCOM(mKeyboard);
	ReleaseCOM(mMouse);
	
//	ReleaseCOM(mJoystick[1]);
}