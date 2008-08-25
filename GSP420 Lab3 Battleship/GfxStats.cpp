#include "GfxStats.h"

GfxStats::GfxStats()
: mFont(0), mFPS(0.0f), mMilliSecPerFrame(0.0f), mNumTris(0), mNumVertices(0), intPrev(0), intNext(0)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 15;
    fontDesc.Width           = 10;
    fontDesc.Weight          = FW_BOLD;
    fontDesc.MipLevels       = 0;
    fontDesc.Italic          = true;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    _tcscpy(fontDesc.FaceName, _T("Times New Roman"));

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));
}

GfxStats::~GfxStats()
{
	ReleaseCOM(mFont);
}

void GfxStats::onLostDevice()
{
	mFont->OnLostDevice();
}

void GfxStats::onResetDevice()
{
	mFont->OnResetDevice();
}

void GfxStats::display()
{
	//Make static copy so memory is not allocated every frame
	static char buffer[256];

	
	RECT R = {5, 5, 0, 0};
	HR(mFont->DrawText(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0)));
}

void GfxStats::update(float dt)
{
	//Make static so that their values persist across function calls
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	//Increment the frame count
	numFrames += 1.0f;

	//Accumulate how much time has passed
	timeElapsed += dt;

	//if one second has passed we compute the frame statistics once
	//per second.  Note that the time between frames can vary, so 
	//these stats are averages over a second
	if( timeElapsed >= 1.0f)
	{
		//Frames per second = numFrames / timeElapsed
		//but timeElapsed approx = 1.0 so
		//frames per second = numFrames

		mFPS = numFrames;

		//Average time, in milliseconds, it took to render a single frame
		mMilliSecPerFrame = 1000.0f / mFPS;

		//Reset the time counter and frame count to prepare for computing
		//the average stats over the next second
		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
}

void GfxStats::addVertices(DWORD n)    { mNumVertices += n; }
void GfxStats::subVertices(DWORD n)	   { mNumVertices -= n; }
void GfxStats::addTriangles(DWORD n)   { mNumTris += n;     }
void GfxStats::subTriangles(DWORD n)   { mNumTris -= n;     }
void GfxStats::setTriCount(DWORD n)    { mNumTris = n;      }
void GfxStats::setVertexCount(DWORD n) { mNumVertices = n;  }
void GfxStats::setNext(int next) { intNext = next; }
void GfxStats::setPrev(int prev) { intPrev = prev; }
void GfxStats::setAi(int ai) { intAi = ai; }
void GfxStats::setLoc(int Loc) { intLoc = Loc; }
void GfxStats::setAiT(int ai) { intAiT = ai; }
void GfxStats::setLocT(int Loc) { intLocT = Loc; }
void GfxStats::setPower(int Power) { intPower = Power; }
