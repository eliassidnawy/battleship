#pragma once
#include <d3dx9.h>
#include "D3DApp.h"
#include <tchar.h>

class GfxStats
{

	private:
		//Prevent Copying
		GfxStats(const GfxStats& rhs);
		GfxStats& operator=(const GfxStats& rhs);

	private:
		ID3DXFont* mFont;
		float mFPS;
		float mMilliSecPerFrame;
		DWORD mNumTris;
		DWORD mNumVertices;
		int intNext;
		int intPrev;
		int intAi;
		int intLoc;
		int intAiT;
		int intLocT;        
		int intPower;
	public:
		GfxStats();
		~GfxStats();


		void onLostDevice();
		void onResetDevice();

		void addVertices(DWORD n);
		void subVertices(DWORD n);
		void addTriangles(DWORD n);
		void subTriangles(DWORD n);

		void setTriCount(DWORD n);
		void setVertexCount(DWORD n);

		void update(float dt);
		void display();

		void setNext(int next);
		void setPrev(int prev);
		void setAi(int Ai);
		void setLoc(int Loc);

		void setAiT(int Ai);
		void setLocT(int Loc);
		void setPower(int Power);
		float getFPS() {return mFPS; }

};