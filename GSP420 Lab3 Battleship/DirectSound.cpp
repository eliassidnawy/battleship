//////////////////////////////////////////////////////
// DirectSound.cpp
//////////////////////////////////////////////////////

#include "directsound.h"

//////////////////////////////////////////////////////
// CDirectSound()
//////////////////////////////////////////////////////
CDirectSound::CDirectSound()
{
    g_pDSObject = NULL;
}

//////////////////////////////////////////////////////
// ~CDirectSound()
//////////////////////////////////////////////////////
CDirectSound::~CDirectSound(void)
{
    
	if(g_pDSObject)
		g_pDSObject->Release();

}

void CDirectSound::CleanUp(IDirectSoundBuffer8* ReleaseMe)
{

	if (ReleaseMe)
        ReleaseMe->Release();

}

HRESULT CDirectSound::CreateSoundEffectBuffers(int iSoundID)
{
	// Not for songs..... zach!
//	BigBallsBuff = LoadWAV("BigBalls.wav");
//	WindRank1Buff = LoadWAV("Music\\Wind1.wav");

//  Introducing test sound 6.2.08 - Bobby
	BigBallsBuff = LoadWAV("bang1.wav");
	return DS_OK;
}

HRESULT CDirectSound::CreateMusicBuffer(int iSoundID)
{

    switch (iSoundID)
    {
 		case BigBallsMusic:
			//BigBalls2.wav
			BigBallsMusicBuff = LoadWAV("BigBalls2.wav");
		  break;		
		  
	}
	
	return DS_OK;
}

HRESULT CDirectSound::InitDirectSound()
{
	/*HRESULT WINAPI DirectSoundCreate8(
	LPGUID lpGuidDevice,   set to NULL(default sound device
	LPDIRECTSOUND8 *ppDS8, object you are creating
	LPUKNOUTER pUnkOuter); NULL - not used */

	HRESULT hResult = DirectSoundCreate8(NULL, &g_pDSObject, NULL);
	if (hResult != DS_OK)
	{
		//error occurred
		strcpy(m_szErrorMsg, "Error Creating DirectSound Device.");
        return hResult;
	}



	/* Once the object is created you must set the cooperative lvl, bascially, are you going to share access to the 
	sound card or hog it, etc.


	IDirectSound8::SetCooperativeLevel (

	HWND hwnd,			handle to parent window
	DWORD dwLevel)		cooperative level which may consist of:

	Level			Macro			Description
	Normal			DSSCL_NORMAL
	Priority			DSSCL_PRIORITY
	Exclusive		DSSCL_EXCLUSIVE
	WritePrimary		DSSCL_WRITEPRIMARY  */

	hResult = g_pDSObject->SetCooperativeLevel(g_hWnd, DSSCL_PRIORITY);
	if (hResult != DS_OK)
	{
		//error occurred
		strcpy(m_szErrorMsg, "Error Setting cooperative level in sound buffer.");
		return hResult;
	}

	//load music and sound here

	//load the buffer with sound, will be ready to use upon return
	CreateSoundEffectBuffers(BigBallSound);
	CreateMusicBuffer(BigBallsMusic);
	
	//Start playing title screen music when the title screen loads
	hResult = BigBallsMusicBuff->SetCurrentPosition(0);
	hResult = BigBallsBuff->SetCurrentPosition(0);	// sound effect test 6.2.08 - Bobby
    //hResult = DuelestCombatMusicBuff->SetCurrentPosition(0);
	
	if (hResult != DS_OK)
        return hResult;
		
	hResult = BigBallsMusicBuff->SetVolume(-1000);
	
	hResult = BigBallsMusicBuff->Play(0, 0, DSBPLAY_LOOPING);
	
	return hResult;
    
}

HRESULT CDirectSound::PlaySound(int iSoundID)
{
	IDirectSoundBuffer8* pBuf;

    switch (iSoundID)
    {
		case BigBallSound:
			CreateMusicBuffer(BigBallSound);
			pBuf = BigBallsBuff;
			SetCurrentlyPlaying(BigBallSound);
		break;	  
    }

    HRESULT hResult;

	hResult = pBuf->SetCurrentPosition(0);
    if (hResult != DS_OK)
        return hResult;

	hResult = pBuf->Play(0, 0, 0);
    if (hResult != DS_OK)
        return hResult;

    return hResult;
}

HRESULT CDirectSound::PlayMusic(int iSoundID)
{
	IDirectSoundBuffer8* pBuf;

    switch (iSoundID)
    {
		case BigBallsMusic:
			CreateMusicBuffer(BigBallsMusic);
			pBuf = BigBallsMusicBuff;
			SetCurrentlyPlaying(BigBallsMusic);
		break;		
    }

    HRESULT hResult = pBuf->SetCurrentPosition(0);
    if (hResult != DS_OK)
        return hResult;

	hResult = pBuf->Play(0, 0, DSBPLAY_LOOPING);
    if (hResult != DS_OK)
        return hResult;

    return hResult;
}

int CDirectSound::GetCurrentlyPlaying(void)
{
	return CurrentlyPlayingMusic;
}

void CDirectSound::SetCurrentlyPlaying(int NowPlaying)
{
	CurrentlyPlayingMusic = NowPlaying;
}

IDirectSoundBuffer8* CDirectSound::CreateBufferFromWAV(FILE *fp, sWaveHeader *Hdr)
{
  IDirectSoundBuffer *pDSB;
  IDirectSoundBuffer8 *pDSBuffer;
  DSBUFFERDESC dsbd;
  WAVEFORMATEX wfex;

  // read in the header from beginning of file
  fseek(fp, 0, SEEK_SET);
  fread(Hdr, 1, sizeof(sWaveHeader), fp);

  // check the sig fields, returning if an error
  if(memcmp(Hdr->RiffSig, "RIFF", 4) || memcmp(Hdr->WaveSig, "WAVE", 4) ||
     memcmp(Hdr->FormatSig, "fmt ", 4) || memcmp(Hdr->DataSig, "data",4))
  return NULL;

  // setup the playback format
  ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
  wfex.wFormatTag = WAVE_FORMAT_PCM;
  wfex.nChannels = Hdr->Channels;
  wfex.nSamplesPerSec = Hdr->SampleRate;
  wfex.wBitsPerSample = Hdr->BitsPerSample;
  wfex.nBlockAlign = wfex.wBitsPerSample / 8 * wfex.nChannels;
  wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

  // create the sound buffer using the header data
  ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
  dsbd.dwSize = sizeof(DSBUFFERDESC);
  dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
  dsbd.dwBufferBytes = Hdr->DataSize;
  dsbd.lpwfxFormat = &wfex;
  if(FAILED(g_pDSObject->CreateSoundBuffer(&dsbd, &pDSB, NULL)))
    return NULL;
  
  // get newer interface
  if(FAILED(pDSB->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer))) {
    pDSB->Release();
    return NULL;
  }

  // return the interface
  return pDSBuffer;
}

BOOL CDirectSound::LoadSoundData(IDirectSoundBuffer8 *pDSBuffer, long LockPos, FILE *fp, long Size)
{
  BYTE *Ptr1, *Ptr2;
  DWORD Size1, Size2;

  if(!Size)
    return FALSE;

  // lock the sound buffer at position specified
  if(FAILED(pDSBuffer->Lock(LockPos, Size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, 0)))
    return FALSE;

  // read in the data
  fread(Ptr1, 1, Size1, fp);
  if(Ptr2 != NULL)
    fread(Ptr2, 1, Size2, fp);
  
  // unlock it
  pDSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);

  // return a success
  return TRUE;
}

IDirectSoundBuffer8* CDirectSound::LoadWAV(char *Filename)
{
  IDirectSoundBuffer8 *pDSBuffer;
  sWaveHeader Hdr;
  FILE *fp;

  // open the source file
  if((fp=fopen(Filename, "rb"))==NULL)
    return NULL;

  // create the sound buffer
  if((pDSBuffer = CreateBufferFromWAV(fp, &Hdr)) == NULL) {
    fclose(fp);
    return NULL;
  }

  // read in the data
  fseek(fp, sizeof(sWaveHeader), SEEK_SET);
  LoadSoundData(pDSBuffer, 0, fp, Hdr.DataSize);

  // close the source file
  fclose(fp);

  // return the new sound buffer fully loaded with sound
  return pDSBuffer;
}

void CDirectSound::SetWindowHandle(HWND hWnd)
{
	g_hWnd = hWnd;
}

void CDirectSound::StopSound(int iSound)
{
	IDirectSoundBuffer8* pBuf;

    switch (iSound)
    {

		  

	}


	if(FAILED(pBuf->Stop()))
	{
		strcpy(m_szErrorMsg, "Error Stopping Sound.");
		//error occurred
	}
}


void CDirectSound::StopMusic(int iSound)
{
	IDirectSoundBuffer8* pBuf;
	IDirectSoundBuffer8* ReleaseStoppedMusic;

    switch (iSound)
    {
		case BigBallsMusic:
			pBuf = BigBallsMusicBuff;
			ReleaseStoppedMusic = BigBallsMusicBuff;
			break;	
    }

    if(FAILED(pBuf->Stop()))
	{
		strcpy(m_szErrorMsg, "Error Stopping Music.");
		//error occurred
	}

	CleanUp(ReleaseStoppedMusic);
}
