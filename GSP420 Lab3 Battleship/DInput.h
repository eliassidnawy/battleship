#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

class DInput
{
	private:
		//Make private to prevent copying of members of this class
		DInput(const DInput& rhs);
		DInput& operator=(const DInput& rhs);

		IDirectInputDevice8		*mMouse;
		DIMOUSESTATE2			mMouseState;

		// for keyboard debouncing
		bool mLastKeyboardState[256];
		bool mCurrKeyboardState[256];
		
	public:
		DInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags, DWORD joystickCoopFlags);
		~DInput();

		void InitJoystick(DWORD joystickCoopFlags);
		void poll();
		void pollJoystick();
		bool keyDown(unsigned char key);
		bool mouseButtonDown(int button);
		float mouseDX();
		float mouseDY();
		float mouseDZ();

		void onLostDevice();
};

extern DInput* gDInput;