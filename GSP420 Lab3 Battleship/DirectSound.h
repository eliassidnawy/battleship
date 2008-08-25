//////////////////////////////////////////////////////
// DirectSound.h
//////////////////////////////////////////////////////

#pragma once

#include "dsound.h"
#include <stdio.h>
#include <D3dx9tex.h>

typedef struct sWaveHeader {
  char  RiffSig[4];         // 'RIFF'
  long  WaveformChunkSize;  // 8
  char  WaveSig[4];         // 'WAVE'
  char  FormatSig[4];       // 'fmt ' (notice space after)
  long  FormatChunkSize;    // 16
  short FormatTag;          // WAVE_FORMAT_PCM
  short Channels;           // # of channels
  long  SampleRate;         // sampling rate
  long  BytesPerSec;        // bytes per second
  short BlockAlign;         // sample block alignment
  short BitsPerSample;      // bits per second
  char  DataSig[4];         // 'data'
  long  DataSize;           // size of waveform data
} sWaveHeader;

enum g_sounds
{
	BigBallsMusic,
	BigBallSound	// Added for sound effects 6.2.08 - Bobby
};

class CDirectSound
{
protected:
    IDirectSound8* g_pDSObject;
    HWND g_hWnd;
    char m_szErrorMsg[256];
	int CurrentlyPlayingMusic;	//Variable that holds enum to currently Playing Music
	IDirectSoundBuffer8* BigBallsMusicBuff;

//	IDirect sound variable for sound effects 6.2.08 - Bobby
	IDirectSoundBuffer8* BigBallsBuff;

	 
public:
    CDirectSound(void);
    ~CDirectSound(void);
    void SetWindowHandle(HWND hWnd);
	HRESULT InitDirectSound(void);
    char* GetErrorString(void);
    HRESULT PlaySound(int iSoundID);
	HRESULT PlayMusic(int iSoundID);
	int GetCurrentlyPlaying(void);				//So you can know what music is playing
	void SetCurrentlyPlaying(int NowPlaying);	//Set what is currently Playing
	void StopSound(int);
	void StopMusic(int);
	void Handlekeys(WPARAM);

private:
	HRESULT CreateSoundEffectBuffers(int iSoundID);	// Added iSoundID 6.2.08 - Bobby
	HRESULT CreateMusicBuffer(int iSoundID);
	IDirectSoundBuffer8 *CreateBufferFromWAV(FILE *fp, sWaveHeader *Hdr);
	BOOL LoadSoundData(IDirectSoundBuffer8 *pDSBuffer, long LockPos, FILE *fp, long Size);
	IDirectSoundBuffer8 *LoadWAV(char *Filename);
	void CleanUp(IDirectSoundBuffer8* ReleaseMe);
	
};