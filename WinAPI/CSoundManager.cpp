#include "stdafx.h"
#include "CSoundManager.h"
#include "CSound.h"
#include "CCore.h"

CSoundManager::CSoundManager() :
    m_pSound(nullptr),
    m_pBGM(nullptr)
{
}

CSoundManager::~CSoundManager() {

}

int CSoundManager::Init()
{
    if (FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL))) {
        MessageBox(NULL, STR_TABLE_CameraVeilTexture, L"SYSTEM ERROR", MB_OK);
        return false;
    }

    // 사운드 디바이스 협조레벨 설정.
    HWND hWnd = CCore::GetInstance()->GetWndHandle();
    if (FAILED(m_pSound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) {
        MessageBox(NULL, STR_TABLE_FailSoundDeviceCooperativeLevelSetting, L"SYSTEM ERROR", MB_OK);
        return false;
    }

    return true;
}

void CSoundManager::RegisterToBGM(CSound* _pSound)
{
    if (m_pBGM != nullptr) {
        m_pBGM->Stop(true);
    }
}