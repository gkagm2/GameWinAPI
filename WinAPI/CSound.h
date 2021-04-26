#pragma once
#include "CResource.h"
#include "CSoundManager.h"
class CSound : public CResource
{
private:
	LPDIRECTSOUNDBUFFER m_pSoundBuffer;
	DSBUFFERDESC m_tBuffInfo;
	int m_iVolume;
public:
	virtual int Load(const wchar_t* _pFilePath) override;

	void Play(bool _bLoop = false); // �Ϲ� ���
	void PlayToBGM(bool _bLoop = false); // BGM���� ���
	void Stop(bool _bReset = false);
	void SetVolume(float _fVolume); // ���� ���� (0 ~ 100)
	void SetPosition(float _fPosition); // �������� ��ġ ���� (0 ~ 100 ����)

private:
	bool LoadWaveSound(const wstring& _strPath);
	int GetDecibel(float _vVolume);

public:
	CSound();
	virtual ~CSound() override;
};