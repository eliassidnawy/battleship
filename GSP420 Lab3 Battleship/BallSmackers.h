#ifndef __BALLSMACKERS_H_
#define __BALLSMACKERS_H_

#include "d3dApp.h"
#include <tchar.h>
#include <crtdbg.h>
#include "GfxStats.h"
#include "DInput.h"
#include <time.h>
#include "DirectSound.h"
#include "dsound.h"
#include "Ball.h"

#include "Ship.h"
#include "Board.h"
#include "AI.h"

#include "Vertex.h"
// enums
#include "enums.h"



class BallSmackers : public D3DApp
{

public:
	BallSmackers(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~BallSmackers();

	/********************Needed for any program you instantiate***************************/
	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();
	/*************************************************************************************/

	void drawMenuState();
	void drawTwoPlayerState();
	void drawThreePlayerState();
	void drawFourPlayerState();

	

//	//the one and only sprite interface through which we do all of our sprite drawing
	ID3DXSprite* mSprite;


	//camera and matrix build functions
	void buildViewMtx();
	void buildProjMtx();

	//added 6/4
	void setupCamera2D();
	void draw2D();

private:
	GfxStats* mGfxStats;	
	ID3DXFont* mFont;
	Ball* ball[2];
	// adds in the sound class
	CDirectSound sound;

	//camera stuffs and others
	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	

	//added MJP 6/3/2008
	ID3DXMesh *mMesh;
	std::vector<Mtrl> mMtrl;
	std::vector<IDirect3DTexture9*> mTex;

	IDirect3DTexture9* mWhiteTex;

	//effects object and buffer object for effect
	ID3DXEffect* mFX;

	//handles for technique and world, view, proj matrices (combined)
	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;

	
	
	//added 6/3/08 mjp
	D3DXHANDLE mhWorldInvTrans;
	D3DXHANDLE mhEyePos;
	D3DXHANDLE mhWorld;
	D3DXHANDLE mhTex;
	D3DXHANDLE mhMtrl;
	D3DXHANDLE mhLight;

	DirLight mLight;
	D3DXMATRIX mWorld;
	
	//end add
	
	// added 6/10/08
	// background properties
	IDirect3DTexture9* mAvaGirl;
	IDirect3DTexture9* mMarkGrid;
	IDirect3DTexture9* mHitCircle;
	IDirect3DTexture9* mMissCircle;
	IDirect3DTexture9* mHHitCircle;
	IDirect3DTexture9* mHMissCircle;

	D3DXVECTOR3 mAvaGirlCenter;
	D3DXVECTOR3 mAvaGirlPos;
	D3DXVECTOR3 mMarkGridCenter;
	D3DXVECTOR3 mMarkGridPos;
	D3DXVECTOR3 mHitCircleCenter;
	D3DXVECTOR3 mMissCircleCenter;

	D3DXVECTOR3 mMarkGridUpperLeftPos;

	//added 6/4/08
	//cursor properties


	IDirect3DTexture9* mCursorCircle;
	IDirect3DTexture9* mMarkCursorCircle;
	
	

	D3DXVECTOR3 mCursorCenter;
	D3DXVECTOR3 mCursorPos;

	// 6/10/08
	// incstruction properties
	IDirect3DTexture9* mInstructions[19];

	D3DXVECTOR3 mInstruc1Center;
	D3DXVECTOR3 mInstruc1Pos;
	

	
	//D3DXVECTOR3 mBoatPos;
	float mBoatColumn;
	float mBoatRow;

	void setupFX();
	void cleanUp();

	// battleship objects
	Ship * mBattleShip;
	Ship * mCarrierShip;
	Ship * mDestroyerShip;
	Ship * mSubmarineShip;
	Ship * mPatrolShip;

	Ship * mShip[5];

	Ship * CurrShip;

	// game state 6/9/08
	int GameState;

	// board 6/10/08
	Board humanBoard;
	Board AIBoard;

	AI AIPlayer;

	int shipsSunkHum;
	int shipsSunkComp;


	// For the AI
	int compFirstHitCol;
	int compFirstHitRow;
	int compLastHitCol;
	int compLastHitRow;
	bool shipUp;
	bool shipDown;
	bool shipLeft;
	bool shipRight;
	bool shipSunk;

	void computerTurn();
	void checkWin();
};
#endif