#include "BallSmackers.h"
#include "DirectSound.h"
#include "Enum.h"
#include "defines.h"

// at camera height 20	#define		GRID_UNIT		1.20f
#define		COLUMN_UNIT		3.7f
#define		ROW_UNIT		3.8f
#define		MARK_GRID_UNIT	32.0f
#define		MAIN_GRID_UNIT	64.0f

#define		COLUMN_A_OFFSET		(0 * COLUMN_UNIT)
#define		COLUMN_B_OFFSET		(1 * COLUMN_UNIT)
#define		COLUMN_C_OFFSET		(2 * COLUMN_UNIT)
#define		COLUMN_D_OFFSET		(3 * COLUMN_UNIT)
#define		COLUMN_E_OFFSET		(4 * COLUMN_UNIT)
#define		COLUMN_F_OFFSET		(5 * COLUMN_UNIT)
#define		COLUMN_G_OFFSET		(6 * COLUMN_UNIT)
#define		COLUMN_H_OFFSET		(7 * COLUMN_UNIT)
#define		COLUMN_I_OFFSET		(8 * COLUMN_UNIT)
#define		COLUMN_J_OFFSET		(9 * COLUMN_UNIT)

#define		ROW_0_OFFSET		(0 * COLUMN_UNIT)
#define		ROW_1_OFFSET		(1 * COLUMN_UNIT)
#define		ROW_2_OFFSET		(2 * COLUMN_UNIT)
#define		ROW_3_OFFSET		(3 * COLUMN_UNIT)
#define		ROW_4_OFFSET		(4 * COLUMN_UNIT)
#define		ROW_5_OFFSET		(5 * COLUMN_UNIT)
#define		ROW_6_OFFSET		(6 * COLUMN_UNIT)
#define		ROW_7_OFFSET		(7 * COLUMN_UNIT)
#define		ROW_8_OFFSET		(8 * COLUMN_UNIT)
#define		ROW_9_OFFSET		(9 * COLUMN_UNIT)

/*
Columns	unit = 3.7
A	-35.15
B
C
D
E
F
G
H
I	-5.55
J	-1.85

Rows	unit = 3.9
0	-18
1
2
3
4
5	1.5
6	5.4
7
8
9

*/

BallSmackers::BallSmackers(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	//make sure the computer has the proper hardware for our settings
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	//added 6/3/08 mjp
	//initialize our vertex declarations
	InitAllVertexDeclarations();
	//end add

	//this creates a new class for the font class (well what will become our font class)
	mGfxStats = new GfxStats();

	//set up camera info
	//mCameraRadius    = 10.0f;
	mCameraRadius    = 2.0f;
	//mCameraRotationY = 0.5 * D3DX_PI;
	//mCameraRotationY = 1.2 * D3DX_PI;
	mCameraRotationY = 0.0 * D3DX_PI;
	//mCameraHeight    = 5.0f;
	mCameraHeight    = 60.0f;

	//added 6/3/08 mjp
	//set up light 
	mLight.dirW = D3DXVECTOR3(0.0f, 1.0f, 2.0f);
	D3DXVec3Normalize(&mLight.dirW, &mLight.dirW);
	mLight.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mLight.diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mLight.spec = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	//load xfile
	LoadXFile("bayliner.x", &mMesh, mMtrl, mTex);
	D3DXMatrixIdentity(&mWorld);

	HR(D3DXCreateTextureFromFile(gd3dDevice, "whitetex.dds", &mWhiteTex));
	//end add

//	//obtain a pointer to a ID3DXSprite class, mSprite being the pointer
	HR(D3DXCreateSprite(gd3dDevice, &mSprite));
	
	//added 6/4/08 mjp
	HR(D3DXCreateTextureFromFile(gd3dDevice, "grid2.bmp", &mAvaGirl));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "markgrid.bmp", &mMarkGrid));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "hitcircle.bmp", &mHitCircle));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "misscircle.bmp", &mMissCircle));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Hhitcircle.bmp", &mHHitCircle));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Hmisscircle.bmp", &mHMissCircle));

	HR(D3DXCreateTextureFromFile(gd3dDevice, "cursorcircle.bmp", &mCursorCircle));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Markcursorcircle.bmp", &mMarkCursorCircle));
	

	//Instruction textures
	HR(D3DXCreateTextureFromFile(gd3dDevice, "instructions1.bmp", &mInstructions[0]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPlaceCShip.bmp", &mInstructions[1]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsOrientCShip.bmp", &mInstructions[2]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPlaceBShip.bmp", &mInstructions[3]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsOrientBShip.bmp", &mInstructions[4]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPlaceDShip.bmp", &mInstructions[5]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsOrientDShip.bmp", &mInstructions[6]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPlaceSShip.bmp", &mInstructions[7]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsOrientSShip.bmp", &mInstructions[8]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPlacePShip.bmp", &mInstructions[9]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsOrientPShip.bmp", &mInstructions[10]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPSelectAction.bmp", &mInstructions[11]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPFireTarget.bmp", &mInstructions[12]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "instructionsPItemSelect.bmp", &mInstructions[13]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPTorpedoTarget.bmp", &mInstructions[14]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPTorpedoDirect.bmp", &mInstructions[15]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "InstructionsPSonarTarget.bmp", &mInstructions[16]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "instructions1.bmp", &mInstructions[17]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "instructionsPWin.bmp", &mInstructions[18]));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "instructionsAIWin.bmp", &mInstructions[19]));

	mAvaGirlCenter = D3DXVECTOR3(256.0f, 256.0f, 0.0f);
	mAvaGirlPos = D3DXVECTOR3(-144.0f, -144.0f, 0.0f);
	mInstruc1Center = D3DXVECTOR3(128.0f, 128.0f, 0.0f);
	mInstruc1Pos = D3DXVECTOR3(256.0f, 256.0f, 0.0f);
	mMarkGridCenter = D3DXVECTOR3(256.0f, 256.0f, 0.0f);
	mMarkGridPos = D3DXVECTOR3(320.0f, -256.0f, 0.0f);

	mCursorCenter = D3DXVECTOR3(16.0f, 16.0f, 0.0f);
	mCursorPos = D3DXVECTOR3(-48.0f, -48.0f, 0.0f);

	mHitCircleCenter = D3DXVECTOR3(8.0f, 8.0f, 0.0f);
	mMarkGridUpperLeftPos = D3DXVECTOR3(111.0f + (0 * MARK_GRID_UNIT), -47.0f  - (0 * MARK_GRID_UNIT), 0.0f);

	//mBoatPos = D3DXVECTOR3(-16.6 + (0 * ROW_UNIT), 0.0f, -32.40 + (0 * COLUMN_UNIT));
	//end add

	// mboat column and row initilzations - cai 06/08/2008
	mBoatColumn = 0;
	mBoatRow = 0;

	// actual ships now
	//mCarrierShip = new Ship(5, "Carrier");
	//mBattleShip = new Ship(4, "Battleship");
	//mDestroyerShip = new Ship(3, "Destroyer");
	//mSubmarineShip = new Ship(3, "Submarine");
	//mPatrolShip = new Ship(2, "Patrol Boat");
	mShip[0] = new Ship(5, "Carrier", 'C');
	mShip[1] = new Ship(4, "Battleship", 'B');
	mShip[2] = new Ship(3, "Destroyer", 'D');
	mShip[3] = new Ship(3, "Submarine", 'S');
	mShip[4] = new Ship(2, "Patrol Boat", 'P');
	
	//font declaration just cuz i didn't want a plain window
	D3DXFONT_DESC fontDesc;
	 fontDesc.Height = 80;
		fontDesc.Width = 40;
		fontDesc.Weight = FW_BOLD;
		fontDesc.MipLevels = 0;
		fontDesc.Italic = true;
		fontDesc.CharSet = DEFAULT_CHARSET;
		fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDesc.Quality = DEFAULT_QUALITY;
		fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		_tcscpy(fontDesc.FaceName, _T("Comic Sans"));

		HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));
		


	// Adds in Direct Sound
	sound.SetWindowHandle(mhMainWnd);
	sound.InitDirectSound();

	//set up players (balls)
//	ball[0] = new Ball(0.5f, 20, 20, 3.0f, 0.0f, 3.0f);	
//	ball[1] = new Ball(0.5f, 20, 20, 0.0f, 0.0f, 0.0f);	
	setupFX();

	//we call this because we want the device states to be set during construction as well as when a device is reset
	onResetDevice();

	GameState = PLACE_CSHIP;
	CurrShip = mShip[0];
	mShip[0]->placed = true;

	AIPlayer.getBoard(&AIBoard);
	AIPlayer.placeShips();

	shipsSunkHum = 0;
	shipsSunkComp = 0;

	// fur AI

	compLastHitCol = -1;
	compLastHitRow = -1;
	compFirstHitCol = -1;
	compFirstHitRow = -1;

	shipUp = false;
	shipDown = false;
	shipLeft = false;
	shipRight = false;
	shipSunk = false;
}

BallSmackers::~BallSmackers()
{
	delete mGfxStats;	
	ReleaseCOM(mFX);
	ReleaseCOM(mFont);
	ReleaseCOM(mSprite);
	ReleaseCOM(mMesh);
	

	for(int i = 0; i < mTex.size(); ++i)
		ReleaseCOM(mTex[i]);

	ReleaseCOM(mWhiteTex);

	DestroyAllVertexDeclarations();

	/*delete mBattleShip;
	delete mCarrierShip;
	delete mDestroyerShip;
	delete mSubmarineShip;
	delete mPatrolShip;*/
	delete mShip[0];
	delete mShip[1];
	delete mShip[2];
	delete mShip[3];
	delete mShip[4];

	// release textures
	ReleaseCOM(mAvaGirl);
}

bool BallSmackers::checkDeviceCaps()
{
	//check for shader support
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	//check for vertex and pixel shader version 2.0 support
	if(caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
		MessageBox(0, "Your computer does not support Vertex Shader Version 2.0!", "Vertex Shader Support", 0);
		return false;
	}

	if(caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		MessageBox(0, "Your computer does not support Pixel Shader Version 2.0!", "Pixel Shader Support", 0);
		return false;
	}
	return true;
}

void BallSmackers::onLostDevice()
{
	//this is where we put any code that needs to be executed before the rendering device is reset
	mGfxStats->onLostDevice();
	HR(mFont->OnLostDevice());
	gDInput->onLostDevice();
	HR(mSprite->OnLostDevice());
	
	//added to fix full screen bug MJP 6/2/08
	HR(mFX->OnLostDevice());
}

void BallSmackers::onResetDevice()
{
	//call the onResetDevice of other objects
	mGfxStats->onResetDevice();
	HR(mSprite->OnResetDevice());
	HR(mFont->OnResetDevice());

	//added to fix full screen bug MJP 6/2/08
	HR(mFX->OnResetDevice());	
	
	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void BallSmackers::updateScene(float dt)
{
	static float timeAccum = 0.0f;

	timeAccum += dt;
	//this is somewhat how font is used, i will add a different function tho that is separate, but this
	//will give you a good idea if you look at the update function or display function in GfxStats
	//I will add another function to actually draw, but I wanted to try and get you a basic working bare bone
	//model so you could start looking around and see what is needed to get the window up adn then see what
	//is needed to actually add things such as sprites and what not

	//this function basically updates everything. It polls the keyboard, updates statistics and graphics, etc
	//two triangles for each sprite--two for background, two for ship, and two for each bullet.  Similarly, 4 vertices 
	//for each sprite.

	//notice how u update, then draw, order is important
	//mGfxStats->setTriCount(4 + mBulletList.size()*2);
	//mGfxStats->setVertexCount(8 + mBulletList.size()*4);
	//mGfxStats->update(dt);

	//Get snapshot of input devices
	gDInput->poll();

//////////////////////////////////////////////////////////////////////////////////
	// cursor movement

	//added 6/3/08 mjp 
	//some camera functionality just for testing
		// Check input.
	// changed 6/8/2008 to handle model coordinate grid placement

	if( GameState == PLAYER_TORPEDO_DIRECT)
	{
		char torpedoDir = 'X';
		if( gDInput->keyDown(DIK_W) )	 
		{
			// set torp direct
			torpedoDir = 'W';
		}
		if( gDInput->keyDown(DIK_S) ) 
		{
			// set torp direct
			torpedoDir = 'S';
		}			
		if( gDInput->keyDown(DIK_A) )
		{
			// set torp direct
			torpedoDir = 'A';
		}			
		if( gDInput->keyDown(DIK_D) )
		{
			// set torp direct
			torpedoDir = 'D';
		}
		if(torpedoDir != 'X')
		{
			int row = mBoatRow;
			int col = mBoatColumn;
			// need to imnplement torpedo logic here
			while(row >= 0 && row <= 9 && col >= 0 && col <= 9)
			{
				char c = 0; 

				///////////////////////
				if( (c = AIBoard.getAt(col, row) ) != '.'  && (c = AIBoard.getAt(col, row) ) != MISS)
				{
					SHIPTYPE ship;

					//get ship that was hit, ask ai
					ship = AIPlayer.getShip(c);

					//you hit
					
					if( AIPlayer.shipHit(ship, 1) == SHIP_SUNK)
						++shipsSunkHum;

					//mark upper board
					AIBoard.setAt(col, row, HIT);
					break;
				}
				else
				{
					//mark upper board
					AIBoard.setAt(col, row, MISS);
				}
				///////////////////////

				if(torpedoDir == 'W')
				{
					--row;
				}
				else if(torpedoDir == 'A')
				{
					--col;
				}
				else if(torpedoDir == 'S')
				{
					++row;
				}
				else if(torpedoDir == 'D')
				{
					++col;
				}
			}

			//
			GameState = COMPUTER_TURN;
		}
	}
	else
	{
		if( gDInput->keyDown(DIK_W) )	 
		{
			//mCameraHeight   += 25.0f * dt;
			mBoatRow -= 1;
		}
		if( gDInput->keyDown(DIK_S) )
		{
			//mCameraHeight   -= 25.0f * dt;
			mBoatRow += 1;
		}
		if( gDInput->keyDown(DIK_A) )
			mBoatColumn -= 1;
		if( gDInput->keyDown(DIK_D) )
			mBoatColumn+= 1;

		if(mBoatColumn < 0)
			mBoatColumn = 0;
		if(mBoatColumn > 9)
			mBoatColumn = 9;
		if(mBoatRow < 0)
			mBoatRow = 0;
		if(mBoatRow > 9)
			mBoatRow = 9;
	}

	mCursorPos.y = 272.0f - ((int)mBoatRow * MAIN_GRID_UNIT);
	mCursorPos.x = -560.0f + ((int)mBoatColumn * MAIN_GRID_UNIT);

//////////////////////////////////////////////////////////////////////////////////

	switch(GameState)
	{
	case PLACE_CSHIP:
	case ORIENT_CSHIP:
		CurrShip = mShip[0];
		break;
	case PLACE_BSHIP:
	case ORIENT_BSHIP:
		CurrShip = mShip[1];
		break;
	case PLACE_DSHIP:
	case ORIENT_DSHIP:
		CurrShip = mShip[2];
		break;
	case PLACE_SSHIP:
	case ORIENT_SSHIP:
		CurrShip = mShip[3];
		break;
	case PLACE_PSHIP:
	case ORIENT_PSHIP:
		CurrShip = mShip[4];
		break;
	}

	if( gDInput->keyDown(DIK_P) )
	{
		int n = humanBoard.getAt(mBoatColumn, mBoatRow);
		n;
		n = humanBoard.getAt(mBoatColumn, mBoatRow);
	}

	if( gDInput->keyDown(DIK_SPACE) )
	{
		switch(GameState)
		{
		case PLACE_CSHIP:
		case PLACE_BSHIP:
		case PLACE_DSHIP:
		case PLACE_SSHIP:
		case PLACE_PSHIP:
			// check for grid conflicts
			if((int)mBoatRow + CurrShip->getSize() <= 10 || (int)mBoatColumn + CurrShip->getSize() <= 10)
			{
				//bool safePlace = false;
				for(int i = 0; i < 2; ++i)
				{
					// check for boat conflicts
					bool safePlace = true;
					for(int p = 0; p < CurrShip->getSize(); ++p)
					{
						int r = 0, c = 0;
						if(i == 0)
						{
							r = p;
						}
						else
						{
							c = p;
						}
						
						if(mBoatColumn + c < 10 && mBoatRow + r < 10)
						{
							if(humanBoard.getAt(mBoatColumn + c, mBoatRow + r) != '.')
							{
								safePlace = false;
							}
						}
						else
						{
							safePlace = false;
						}
					}
					if(safePlace == true)
					{
						// place boat
						for(int p = 0; p < CurrShip->getSize(); ++p)
						{
							int r = 0, c = 0;
							if(i == 0)
							{
								r = p;
							}
							else
							{
								c = p;
							}
							humanBoard.setAt(mBoatColumn + c, mBoatRow + r, CurrShip->face);
						}
						CurrShip->col = (int)mBoatColumn;
						CurrShip->row = (int)mBoatRow;
						CurrShip->Pos = D3DXVECTOR3(-16.6 + ((int)mBoatRow * ROW_UNIT), 0.0f, -32.40 + ((int)mBoatColumn * COLUMN_UNIT));
						CurrShip->placed = true;
						CurrShip->dir = i;
						++GameState;
						i = 3;		// this will break out of the for loop
					}
				}
			}
			else
			{
				sound.PlaySound(BigBallSound);
			}
			break;
		case ORIENT_CSHIP:
		case ORIENT_BSHIP:
		case ORIENT_DSHIP:
		case ORIENT_SSHIP:
		case ORIENT_PSHIP:
			++GameState;
			break;
		case PLAYER_FIRE_TARGET:
			// update for fired coordinate
			////////////////////////////////////////////////////////
			//check for hit
			if(		AIBoard.getAt(mBoatColumn, mBoatRow) != HIT
				&&	AIBoard.getAt(mBoatColumn, mBoatRow) != MISS)
			{
				char c = 0;
				if( (c = AIBoard.getAt(mBoatColumn, mBoatRow) ) != '.'  )
				{
					SHIPTYPE ship;

					//get ship that was hit, ask ai
					ship = AIPlayer.getShip(c);

					//you hit
					
					if( AIPlayer.shipHit(ship, 1) == SHIP_SUNK)
						++shipsSunkHum;

					//mark upper board
					AIBoard.setAt(mBoatColumn, mBoatRow, HIT);
				}
				else
				{
					//mark upper board
					AIBoard.setAt(mBoatColumn, mBoatRow, MISS);
				}
				////////////////////////////////////////////////////////
				// set gamestate to COMPUTER_TURN
				GameState = COMPUTER_TURN;
				//GameState = PLAYER_ACTION_SELECT;
			}
			break;
		case PLAYER_TORPEDO_TARGET:
			// make sure it's a valid starting spot
			if(		AIBoard.getAt(mBoatColumn, mBoatRow) != HIT
				&&	AIBoard.getAt(mBoatColumn, mBoatRow) != MISS)
			{
				GameState = PLAYER_TORPEDO_DIRECT;
			}
			break;
		case PLAYER_SONAR_TARGET:
			//////////////////////////////////////////////////
			for(int i = 0; i < 4; ++i)
			{
				//////// CHECK CODE
				if(		AIBoard.getAt(mBoatColumn, mBoatRow) != HIT
				&&	AIBoard.getAt(mBoatColumn, mBoatRow) != MISS)
				{
					char c = 0;
					if( (c = AIBoard.getAt(mBoatColumn, mBoatRow) ) != '.'  )
					{
						//SHIPTYPE ship;

						////get ship that was hit, ask ai
						//ship = AIPlayer.getShip(c);

						////you hit
						//
						//if( AIPlayer.shipHit(ship, 1) == SHIP_SUNK)
						//	++shipsSunkHum;

						////mark upper board
						//AIBoard.setAt(mBoatColumn, mBoatRow, HIT);
					}
					else
					{
						//mark upper board
						AIBoard.setAt(mBoatColumn, mBoatRow, MISS);
					}
					////////////////////////////////////////////////////////
				}
				//////// MODIFIERS
				if(i == 0)
				{
					mBoatColumn++;
					if(mBoatColumn > 9)
					{
						mBoatColumn = 9;
					}
				}
				else if(i == 1)
				{
					mBoatRow++;
					if(mBoatRow > 9)
					{
						mBoatRow = 9;
					}
				}
				else if(i == 2)
				{
					mBoatColumn--;
					if(mBoatColumn < 0)
					{
						mBoatColumn = 0;
					}
				}
				else if(i == 3)
				{
					mBoatRow--;
					if(mBoatRow < 0)
					{
						mBoatRow = 0;
					}
				}
			}
			// set gamestate to COMPUTER_TURN
			GameState = COMPUTER_TURN;
			//GameState = PLAYER_ACTION_SELECT;
			//////////////////////////////////////////////////
			break;
		}
		/*
		mBattleShip->col = mBoatColumn;
		mBattleShip->row = mBoatRow;
		mBattleShip->Pos = D3DXVECTOR3(-16.6 + ((int)mBoatRow * ROW_UNIT), 0.0f, -32.40 + ((int)mBoatColumn * COLUMN_UNIT));
		mBattleShip->placed = true;
		*/
	}

	if(	GameState == ORIENT_CSHIP || 
		GameState == ORIENT_BSHIP || 
		GameState == ORIENT_DSHIP || 
		GameState == ORIENT_SSHIP || 
		GameState == ORIENT_PSHIP)
	{
		if( gDInput->keyDown(DIK_Q) )
		{
			if(CurrShip->row + CurrShip->getSize() <= 10)
			{
				// check for boat conflicts
				bool safePlace = true;
				for(int p = 1; p < CurrShip->getSize(); ++p)
				{
					if(CurrShip->col < 10 && CurrShip->row + p < 10)
					{
						if(humanBoard.getAt(CurrShip->col, CurrShip->row + p) != '.')
						{
							safePlace = false;
						}
					}
					else
					{
						safePlace = false;
					}
				}
				if(safePlace == true)
				{
					// replace boat
					for(int p = 0; p < CurrShip->getSize(); ++p)
					{
						humanBoard.setAt(CurrShip->col + p, CurrShip->row, '.');
						humanBoard.setAt(CurrShip->col, CurrShip->row + p, CurrShip->face);
					}
					CurrShip->dir = 0;
				}
				else
				{
					sound.PlaySound(BigBallSound);
				}
			}
		}
		else if( gDInput->keyDown(DIK_E) )
		{
			if(CurrShip->col + CurrShip->getSize() <= 10)
			{
				// check for boat conflicts
				bool safePlace = true;
				for(int p = 1; p < CurrShip->getSize(); ++p)
				{
					if(CurrShip->col + p < 10 && CurrShip->row < 10)
					{
						if(humanBoard.getAt(CurrShip->col + p, CurrShip->row) != '.')
						{
							char n = humanBoard.getAt(CurrShip->col + p, CurrShip->row);
							safePlace = false;
						}
					}
					else
					{
						safePlace = false;
					}
				}
				if(safePlace == true)
				{
					// place boat
					for(int p = 0; p < CurrShip->getSize(); ++p)
					{
						humanBoard.setAt(CurrShip->col, CurrShip->row + p, '.');
						humanBoard.setAt(CurrShip->col + p, CurrShip->row, CurrShip->face);
					}
					CurrShip->dir = 1;
				}
				else
				{
					sound.PlaySound(BigBallSound);
				}
			}
		}
	}

	if(GameState == PLAYER_ACTION_SELECT)
	{
		if( gDInput->keyDown(DIK_1) )
		{
			GameState = PLAYER_FIRE_TARGET;
		}
		else if( gDInput->keyDown(DIK_2) )
		{
			GameState = PLAYER_ITEM_SELECT;
		}
	}

	if(GameState == PLAYER_ITEM_SELECT)
	{
		if( gDInput->keyDown(DIK_1) )
		{
			GameState = PLAYER_TORPEDO_TARGET;
		}
		else if( gDInput->keyDown(DIK_2) )
		{
			GameState = PLAYER_SONAR_TARGET;
		}
	}

	if(GameState == COMPUTER_TURN)
	{
		checkWin();
		computerTurn();
		checkWin();
	}

	/*if(gDInput->mouseButtonDown(0))
	{
		sound.PlaySound(BigBallSound);
	}*/

	if(timeAccum >= 1.0f/30.0f)
	{
		
//		ball[0]->updatePosX(0.1f);
//		ball[1]->updatePosX(-0.1f);
		timeAccum = 0.0f;
	}
	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();
	}


//this function is very important, although it will change greatly for our game
void BallSmackers::drawScene()
{

 //   // Now we can clear just view-port's portion of the buffer to red...
  HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());
    
	//added 6/3/08 mjp
	HR(mFX->SetValue(mhLight, &mLight, sizeof(DirLight)));

	////set pos of boat
	//D3DXMATRIX B, S;
	//D3DXMatrixTranslation(&B, mBoatPos.x, mBoatPos.y, mBoatPos.z);
	//D3DXMatrixScaling(&S, 0.2f, 0.2f, 0.2f);

	//HR(mFX->SetMatrix(mhWVP, &(S*B*mWorld*mView*mProj)));

	//////set pos of ship[0]
	////D3DXMATRIX S1_B, S1_S;
	////D3DXMatrixTranslation(&S1_B, mShipPos[0].x, mShipPos[0].y, mShipPos[0].z);
	////D3DXMatrixScaling(&S1_S, 0.2f, 0.2f, 0.2f);

	////HR(mFX->SetMatrix(mhWVP, &(S1_S*S1_B*mWorld*mView*mProj)));

	D3DXMATRIX worldInvTrans;
	//D3DXMatrixInverse(&worldInvTrans, 0, &mWorld);
	//D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	//HR(mFX->SetMatrix(mhWorldInvTrans, &worldInvTrans));
	//HR(mFX->SetMatrix(mhWorld, &mWorld));
	//end add

    //setup rendering technique
	HR(mFX->SetTechnique(mhTech));

	//begin pass
	UINT numPasses = 0;

	//begin fx rendering
	HR(mFX->Begin(&numPasses, 0));

	//because I am only doing one pass I will hard code it
	HR(mFX->BeginPass(0));


	for(int i =0; i < mMtrl.size(); ++i)
	{	
		//added 6/3/08 mjp
		HR(mFX->SetValue(mhMtrl, &mMtrl[i], sizeof(Mtrl)));

		//if there is a texture, use it
		if(mTex[i] != 0)
		{
			HR(mFX->SetTexture(mhTex, mTex[i]));
		}

		//if not, use the pure white texture,
		//When the texture color
		// is multiplied by the color from lighting, it is like multiplying by
		// 1 and won't change the color from lighting.
		else
		{
			HR(mFX->SetTexture(mhTex, mWhiteTex));
		}

		HR(mFX->CommitChanges());
		HR(mMesh->DrawSubset(i));
	}

	HR(mFX->EndPass());

	//////////////////////////////////////////////////////////////////////////////////////////////

	// trying to draw battleship mBattleShip

	for(int i = 0; i < 5; ++i)
	{
		if(mShip[i]->placed == true)
		{
			for(int p = 0; p < mShip[i]->getSize(); ++p)
			{
				int r = 0, c = 0;
				if(mShip[i]->dir == 0)
				{
					r = p;
				}
				else
				{
					c = p;
				}
				D3DXMATRIX S1_B, S1_S;
				D3DXMatrixTranslation(&S1_B, mShip[i]->Pos.x + (r * ROW_UNIT), mShip[i]->Pos.y, mShip[i]->Pos.z + (c * COLUMN_UNIT));
				D3DXMatrixScaling(&S1_S, 0.2f, 0.2f, 0.2f);

				HR(mFX->SetMatrix(mhWVP, &(S1_S*S1_B*mWorld*mView*mProj)));

				//D3DXMATRIX worldInvTrans;
				D3DXMatrixInverse(&worldInvTrans, 0, &mWorld);
				D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
				HR(mFX->SetMatrix(mhWorldInvTrans, &worldInvTrans));
				HR(mFX->SetMatrix(mhWorld, &mWorld));
				//end add

				//setup rendering technique
				//HR(mFX->SetTechnique(mhTech));

				//begin pass
				//UINT numPasses = 0;

				//begin fx rendering
				//HR(mFX->Begin(&numPasses, 0));

				//because I am only doing one pass I will hard code it
				HR(mFX->BeginPass(0));

				for(int i =0; i < mMtrl.size(); ++i)
				{	
					//added 6/3/08 mjp
					HR(mFX->SetValue(mhMtrl, &mMtrl[i], sizeof(Mtrl)));

					//if there is a texture, use it
					if(mTex[i] != 0)
					{
						HR(mFX->SetTexture(mhTex, mTex[i]));
					}

					//if not, use the pure white texture,
					//When the texture color
					// is multiplied by the color from lighting, it is like multiplying by
					// 1 and won't change the color from lighting.
					else
					{
						HR(mFX->SetTexture(mhTex, mWhiteTex));
					}

					HR(mFX->CommitChanges());
					HR(mMesh->DrawSubset(i));
				}

				HR(mFX->EndPass());
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	
	//because I am only doing one pass I will hard code it
/*	HR(mFX->BeginPass(0));	

	//for each ball
/for(int b = 0; b < 2; ++b)
	{
		//definition for translation maxtrix
		D3DXMATRIX T;
		D3DXMatrixTranslation(&T, ball[b]->getPosX(), ball[b]->getPosY(), ball[b]->getPosZ());
		HR(mFX->SetMatrix(mhWVP, &(T*mView*mProj)));
		HR(mFX->CommitChanges());
		
		//jon this section will probably change, but it shdn't affect u
		ball[b]->draw();
	}

	HR(mFX->EndPass());*/
	HR(mFX->End());

	
	
	HR(gd3dDevice->EndScene());

	setupCamera2D();
	draw2D();
	
	//present the back buffer
	HR(gd3dDevice->Present(0, 0, 0, 0));

	
	
}

void BallSmackers::setupFX()
{
	ID3DXBuffer* mFXErrors = 0;

	//create the effects object, if there are errors, display them
		//the following functions takes in the device to attach it too (our Direct3DDevice),
		//the name of the file to pull the effects from
		//defines & includes -- this shd be 0 for us
		//any flags (I want to know if we have errors so I set the Debug flag)
		//the next parameter specifies if we are sharing variables in the fx file with other fx files, we
		//aren't so it is 0
		//the last two parameters are the addresses of pointers to the fx object and the error buffer
	HR(D3DXCreateEffectFromFile(gd3dDevice, "PhongDirLtTex.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &mFXErrors));

	if( mFXErrors )
		MessageBox(0, (char*)mFXErrors->GetBufferPointer(), 0, 0);

	//get handles
	mhTech = mFX->GetTechniqueByName("PhongDirLtTexTech");
	mhWVP = mFX->GetParameterByName(0, "gWVP");
	mhWorldInvTrans = mFX->GetParameterByName(0, "gWorldInvTrans");
	mhMtrl = mFX->GetParameterByName(0, "gMtrl");
	mhLight = mFX->GetParameterByName(0, "gLight");
	mhEyePos = mFX->GetParameterByName(0, "gEyePosW");
	mhWorld = mFX->GetParameterByName(0, "gWorld");
	mhTex = mFX->GetParameterByName(0, "gTex");
}	

//the following functions help set up the camera and the world view
void BallSmackers::buildViewMtx()
{
	float x = mCameraRadius * cosf(mCameraRotationY);
	float z = mCameraRadius * sinf(mCameraRotationY);
	D3DXVECTOR3 pos(1, mCameraHeight, 0);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

	HR(mFX->SetValue(mhEyePos, &pos, sizeof(D3DXVECTOR3)));
}

void BallSmackers::buildProjMtx()
{
	float w = WIDTH;
	float h = HEIGHT;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}

void BallSmackers::setupCamera2D()
{
	//sets up the camera 1000 units back looking at the origin
	D3DXMATRIX V;
	D3DXVECTOR3 pos(0, 0, -1000.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &V));

	//The following code defines the volume of space the camera sees
	D3DXMATRIX P;
	RECT R;
	GetClientRect(mhMainWnd, &R);
	float width = (float)R.right;
	float height = (float)R.bottom;
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI*0.25f, width/height, 1.0f, 5000.0f);
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &P));

	//this code sets texture filters, which helps to smooth out distortions when you scale a texture
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HR(gd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR));

	//this line of code disables Direct3D Lighting
	HR(gd3dDevice->SetRenderState(D3DRS_LIGHTING, false));

	//WE DO ALPHA TESTING AND BLENDING SO THAT THE BLACK BORDER AROUND THE SHIP IN THIS CASE DOES NOT SHOW
	//the following code specifies an alpha test and reference value
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAREF, 10));
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));

	//the following code is used to set up alpha blending 
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	//indicates that we are using 2D texture coordinates
	HR(gd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2));
}

void BallSmackers::draw2D()
{
	// Now we can clear just view-port's portion of the buffer to red...
	// HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

    HR(gd3dDevice->BeginScene());

    //draw stuff here
	HR(mSprite->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE));

	//without transformation, image will draw at center of screen
	D3DXMATRIX T, S;
	D3DXMatrixTranslation(&T, mAvaGirlPos.x, mAvaGirlPos.y, 0.0f);
	D3DXMatrixScaling(&S, 2.0f, 2.0f, 0.0f);
	HR(mSprite->SetTransform(&(S*T)));

	HR(mSprite->Draw(mAvaGirl, 0, &mAvaGirlCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
	HR(mSprite->Flush());

	//
	D3DXMatrixTranslation(&T, mInstruc1Pos.x, mInstruc1Pos.y, 0.0f);
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 0.0f);
	//HR(gd3dDevice->SetTransform(D3DTS_TEXTURE0, &S));		// this will fix the bug but I'm too far in to care
	HR(mSprite->SetTransform(&(S*T)));

	HR(mSprite->Draw(mInstructions[GameState], 0, &mInstruc1Center, 0, D3DCOLOR_XRGB(255, 255, 255)));
	HR(mSprite->Flush());

	//
	D3DXMatrixTranslation(&T, mMarkGridPos.x, mMarkGridPos.y, 0.0f);
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 0.0f);
	HR(mSprite->SetTransform(&(S*T)));

	HR(mSprite->Draw(mMarkGrid, 0, &mMarkGridCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
	HR(mSprite->Flush());

	//
	for(int row = 0; row < AIBoard.getBoardHeight(); ++row)
	{
		for(int col = 0; col < AIBoard.getBoardHeight(); ++col)
		{
			if(AIBoard.getAt(col, row) != '.')
			{
				D3DXMatrixTranslation(&T, mMarkGridUpperLeftPos.x + (col * MARK_GRID_UNIT), mMarkGridUpperLeftPos.y - (row * MARK_GRID_UNIT), 0.0f);
				D3DXMatrixScaling(&S, 1.0f, 1.0f, 0.0f);
				HR(mSprite->SetTransform(&(S*T)));

				if(AIBoard.getAt(col, row) == HIT)
				{
					HR(mSprite->Draw(mHitCircle, 0, &mHitCircleCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
				}
				else if(AIBoard.getAt(col, row) == MISS)
				{
					HR(mSprite->Draw(mMissCircle, 0, &mHitCircleCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
				}
				HR(mSprite->Flush());
			}

			if(humanBoard.getAt(col, row) != '.')
			{
				D3DXMatrixTranslation(&T, -560.0f + (col * MAIN_GRID_UNIT), 272.0f - (row * MAIN_GRID_UNIT), 0.0f);
				D3DXMatrixScaling(&S, 1.5f, 1.5f, 0.0f);
				HR(mSprite->SetTransform(&(S*T)));

				if(humanBoard.getAt(col, row) == HIT)
				{
					HR(mSprite->Draw(mHHitCircle, 0, &mCursorCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
				}
				else if(humanBoard.getAt(col, row) == MISS)
				{
					HR(mSprite->Draw(mHMissCircle, 0, &mCursorCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
				}
				HR(mSprite->Flush());
				/*
					mCursorPos.y = 272.0f - ((int)mBoatRow * MAIN_GRID_UNIT);
					mCursorPos.x = -560.0f + ((int)mBoatColumn * MAIN_GRID_UNIT);
				*/
			}
		}
	}

	//
	if(GameState >= PLACE_CSHIP && GameState <= ORIENT_PSHIP)
	{
		D3DXMatrixTranslation(&T, mCursorPos.x, mCursorPos.y, 0.0f);
		D3DXMatrixScaling(&S, 1.5f, 1.5f, 0.0f);
		HR(mSprite->SetTransform(&(S*T)));

		HR(mSprite->Draw(mCursorCircle, 0, &mCursorCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
		HR(mSprite->Flush());
	}
	else
	{
		// draw cursor on mark grid
		D3DXMatrixTranslation(&T, mMarkGridUpperLeftPos.x + (mBoatColumn * MARK_GRID_UNIT), mMarkGridUpperLeftPos.y - (mBoatRow * MARK_GRID_UNIT), 0.0f);
		D3DXMatrixScaling(&S, 0.8f, 0.8f, 0.0f);
		HR(mSprite->SetTransform(&(S*T)));

		HR(mSprite->Draw(mMarkCursorCircle, 0, &mCursorCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
		HR(mSprite->Flush());
	}

	//anything that needs to be drawn should be above this line and below the mSprite->Begin line
	HR(mSprite->End());

	HR(gd3dDevice->EndScene());
	
}

void BallSmackers::computerTurn()
{
	int tx, ty;
	AIPlayer.getFiringSolution(tx, ty);

	char c = 0;
	if( (c = humanBoard.getAt(tx, ty) ) != '.'  )
	{
		//SHIPTYPE ship;

		//get ship that was hit, ask ai
		//ship = AIPlayer.getShip(c);

		//AI hit

		/*if( AIPlayer.shipHit(ship, 1) == SHIP_SUNK)
			++shipsSunkComp;*/

		//mark upper board
		humanBoard.setAt(tx, ty, HIT);
		AIPlayer.receiveSuccess(tx, ty, true);
	}
	else
	{
		//mark upper board
		humanBoard.setAt(tx, ty, MISS);
		AIPlayer.receiveSuccess(tx, ty, false);
	}
	////////////////////////////////////////////////////////
	// set gamestate to COMPUTER_TURN
	//GameState = COMPUTER_TURN;
	GameState = PLAYER_ACTION_SELECT;
}

void BallSmackers::checkWin()
{
	int PCheckDeath = 0;
	int AICheckDeath = 0;
	// very lazy way of implementing lose conditions: detecting 17 hits:
	for(int row = 0; row < AIBoard.getBoardHeight(); ++row)
	{
		for(int col = 0; col < AIBoard.getBoardHeight(); ++col)
		{
			if(AIBoard.getAt(col, row) == HIT)
			{
				++AICheckDeath;
			}
		}
	}
	if(AICheckDeath == 17)
	{
		GameState = PLAYER_WINS;
		return;
	}

	for(int row = 0; row < humanBoard.getBoardHeight(); ++row)
	{
		for(int col = 0; col < humanBoard.getBoardHeight(); ++col)
		{
			if(humanBoard.getAt(col, row) == HIT)
			{
				++PCheckDeath;
			}
		}
	}
	if(PCheckDeath == 17)
	{
		GameState = AI_WINS;
		return;
	}

}