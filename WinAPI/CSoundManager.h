#pragma once

// sound �뵵 (��� ���� �߿�)
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

class CSound;

class CSoundManager
{
	SINGLETON(CSoundManager);

private:
	LPDIRECTSOUND8 m_pSound; // ����ī�� ��ǥ ��ü
	CSound* m_pBGM;			 // ���� ������ BGM sound

public:
	int Init();
	LPDIRECTSOUND8 GetSoundDevice() { return m_pSound; }
	void RegisterToBGM(CSound* _pSound);
};