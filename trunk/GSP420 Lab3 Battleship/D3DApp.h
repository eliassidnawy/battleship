#ifndef D3DAPP_H
#define D3DAPP_H

#include "d3dUtil.h"
#include <string>
class State;

class D3DApp
{
	protected:
		std::string mMainWindowCaption;
		D3DDEVTYPE mDevType;
		DWORD mRequestedVP;

		HINSTANCE mhAppInst;
		HWND mhMainWnd;
		
		IDirect3D9* md3dObject;
		bool		mAppPaused;
		D3DPRESENT_PARAMETERS d3dPP;
		State* currState;

	public:
		
		D3DApp(HINSTANCE hInstance, std::string windowCaption, D3DDEVTYPE devType, DWORD requestedVP);
		virtual ~D3DApp();

		HINSTANCE getAppInstance();
		HWND	  getMainWindowHandle();

		virtual bool checkDeviceCaps()	{ return true; }
		virtual void onLostDevice()		{}
		virtual void onResetDevice()	{}
		virtual void updateScene(float dt)	{}
		virtual void drawScene()			{}

		virtual void initMainWindow();
		virtual void initD3D();
		virtual int run();
		virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

		void enableFullScreenMode(bool);
		bool isDeviceLost();
		DWORD getBackBufferWidth() { return d3dPP.BackBufferWidth; }
		DWORD getBackBufferHeight() { return d3dPP.BackBufferHeight; }
		void setState(State* newState) {currState = newState;}
};



#endif 