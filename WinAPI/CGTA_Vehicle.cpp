#include "stdafx.h"

#include "CScene.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CGTA_Vehicle.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidbody2D.h"
#include "CGTA_Character.h"
#include "CGTA_Player.h"
#include "CGTA_Bullet.h"
#include "CSoundManager.h"
#include "CSound.h"
#include "CGTA_Citizen.h"
#include "CGTA_Cop.h"
#include "CTile.h"
#include "CGTA_EffectUI.h"
#include "CDebug.h"

CGTA_Vehicle::CGTA_Vehicle(E_GroupType _eGroupType) :
	CObject(_eGroupType = E_GroupType::VEHICLE),
	m_pDriver(nullptr),
	m_bExplosion(false),
	m_bReverse(false),
	m_eVehicleState(E_VehicleState::normal),
	m_pEngineSound(nullptr),
	m_pRadio(nullptr),
	m_pExplosionTex(nullptr)
{
}

void CGTA_Vehicle::SetExplosionTex(const wstring& _strTexureKey)
{
	m_pExplosionTex = CResourceManager::GetInstance()->GetTexture(_strTexureKey, _strTexureKey);
}

CGTA_Vehicle::CGTA_Vehicle(const CGTA_Vehicle& _origin) :
	CObject(_origin),
	m_pDriver(nullptr),
	m_bExplosion(_origin.m_bExplosion),
	m_bReverse(_origin.m_bReverse),
	m_eVehicleState(_origin.m_eVehicleState),
	m_pEngineSound(_origin.m_pEngineSound),
	m_pRadio(nullptr),
	m_pExplosionTex(_origin.m_pExplosionTex)
{
	InitRadio(rand() % Sound_CarRadio_Len);
}

CGTA_Vehicle::~CGTA_Vehicle()
{
}

void CGTA_Vehicle::Init()
{
	CRigidbody2D* pRigidbody = new CRigidbody2D(this);
	pRigidbody->SetMass(11.f);
	pRigidbody->SetDrag(8.5f);

	// 상속받은 클래스의 init함수 내에서 설정해줘야 함.
	assert(m_pEngineSound);
	assert(m_pRadio);
	assert(m_pExplosionTex);

	CObject::Init();
}

void CGTA_Vehicle::PrevUpdate()
{
	CObject::PrevUpdate();
}

void CGTA_Vehicle::Update()
{
	if (m_pDriver)
		m_pDriver->SetPosition(GetPosition());
}

void CGTA_Vehicle::LateUpdate()
{
	__super::LateUpdate();
}

void CGTA_Vehicle::Render(HDC _hDC)
{
	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (nullptr == GetTexture()) {
		Rectangle(_hDC,
			(int)(vRenderPosition.x - ScaleX() * 0.5f),
			(int)(vRenderPosition.y - ScaleY() * 0.5f),
			(int)(vRenderPosition.x + ScaleX() * 0.5f),
			(int)(vRenderPosition.y + ScaleY() * 0.5f));


		if (GetAsyncKeyState(0x30) & 0x8000) {
			// Print Position
			wchar_t pStrPosition[100] = { 0, };
			swprintf(pStrPosition, 100, L"(%.2f, %.2f)", vRenderPosition.x, vRenderPosition.y);

			SetTextAlign(_hDC, TA_CENTER);
			TextOut(_hDC, (int)vRenderPosition.x, (int)(vRenderPosition.y), pStrPosition, (int)wcslen(pStrPosition));
			SetTextAlign(_hDC, TA_LEFT | TA_TOP);
		}
	}
	else {

		if (nullptr != GetAnimator())
			GetAnimator()->Render(_hDC);
		else {
			RenderRotatedTex(_hDC, vRenderPosition, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight());
		}
	}
	
	if (nullptr != GetCollider()) {
		if(GetCollider()->IsRender())
			GetCollider()->Render(_hDC);
	}
}

void CGTA_Vehicle::OnCollisionEnter(CObject* _pOther)
{
	CGTA_Bullet* pBullet = dynamic_cast<CGTA_Bullet*>(_pOther);
	if (pBullet) {
		if (false == m_bExplosion) {
			VehicleInfo().fHp -= pBullet->GetDamage();
			VehicleInfo().fHp = VehicleInfo().fHp < 0.f ? 0.f : VehicleInfo().fHp;
			if (0.f == VehicleInfo().fHp)
				Explosion();
		}
		return;
	}

	CTile* pTile = dynamic_cast<CTile*>(_pOther);
	if (pTile) {
		if (E_TileType::Wall == pTile->GetTileType()) {
			if (m_bExplosion)
				return;
			if (GetRigidbody()->GetSpeed() * 2.f > 2.5f) {
				VehicleInfo().fHp -= 1.f;
				VehicleInfo().fHp = VehicleInfo().fHp < 0.f ? 0.f : VehicleInfo().fHp;
				if (0.f == VehicleInfo().fHp) {
					Explosion();
				}
				CSound* pSound = CResourceManager::GetInstance()->GetSound(Sound_Collision_CarPedBounce, Sound_Collision_CarPedBounce);
				//pSound->Stop(true);
				pSound->Play();
			}
		}
	}

	CGTA_Vehicle* pVehicle = dynamic_cast<CGTA_Vehicle*>(_pOther);
	if (pVehicle) {
		if (m_bExplosion)
			return;

		if (GetRigidbody()->GetSpeed() * 2.f > 2.5f) {
			VehicleInfo().fHp -= 1.f;
			VehicleInfo().fHp = VehicleInfo().fHp < 0.f ? 0.f : VehicleInfo().fHp;
			if (0.f == VehicleInfo().fHp) {
				Explosion();
			}
			CSound* pSound = CResourceManager::GetInstance()->GetSound(Sound_Collision_CarPedBounce, Sound_Collision_CarPedBounce);
			//pSound->Stop(true);
			pSound->Play();
		}
	}
}

void CGTA_Vehicle::OnCollisionStay(CObject* _pOther)
{
	CGTA_Character* pCharacter = dynamic_cast<CGTA_Character*>(_pOther);
	if (pCharacter) {
		// 차량을 타려고하는 캐릭터와 접촉 시 
		if (E_CharacterState::getInTheVehicle == pCharacter->GetCharacterState()) {
			// TODO : 차량의 속도에 따라 탈지 말지 결정하기
			pCharacter->Drive();
			CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(GetDriver());
			if (pPlayer) { // 플레이어면
				pPlayer->SetActiveAI(false); // AI를 끈다.
			}
			else {
				// TODO : 운전한다.
				// GetCharacter()->GetAI()->ChangeState(L"drive");
			}
		}
		// 지나가다 접촉할 경우
		else {
			float fSpeed = GetRigidbody()->GetSpeed() * 2.f;
			// 캐릭터와 접촉했지만 속도가 미미할때
			if (fSpeed < 7.f) {
				if (E_CharacterState::getInTheVehicle != pCharacter->GetCharacterState()) {
					CGTA_Citizen* pCitizen = dynamic_cast<CGTA_Citizen*>(_pOther);
					if (pCitizen) {
						if (fSpeed > 0.f) {
							wstring strWatchItBuddy = Sound_WatchItBuddy + std::to_wstring((rand() % Sound_WatchItBuddy_Len) + 1);
							CSound* pSound = CResourceManager::GetInstance()->GetSound(strWatchItBuddy, strWatchItBuddy);
							pSound->Play();
						}
					}
				}
			}
			// 속도가 빠른 상태로 접촉했을 경우
			else {
				pCharacter->CharacterInfo().fHp = 0.f;
				CSound* pSound = CResourceManager::GetInstance()->GetSound(Sound_Collision_CarPedSquash, Sound_Collision_CarPedSquash);
				pSound->Play();
				CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(m_pDriver);
				if (pPlayer) {
					CGTA_EffectUI* pEffectUI = new CGTA_EffectUI(E_GroupType::UI);
					pEffectUI->Init();
					pEffectUI->SetEffectPos(GetPosition());
					pEffectUI->SetText(L"300");
					CreateObject(pEffectUI);
					pPlayer->AddMoney(300);
				}
				pCharacter->Dead();

				// 시민들 상태값 변환
				vector<CObject*>& vecObjs = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::CITIZEN);
				for (UINT i = 0; i < vecObjs.size(); ++i) {
					Vector3 vPosition = GetPosition();
					Vector3 vTargetPos = vecObjs[i]->GetPosition();
					float fDistance = (vPosition - vTargetPos).GetDistance();
					CGTA_Character* pCharacter = dynamic_cast<CGTA_Character*>(vecObjs[i]);
					if (fDistance <= pCharacter->GetNoticeDistance()) {
						CGTA_Citizen* pCitizen = dynamic_cast<CGTA_Citizen*>(vecObjs[i]);
						if (pCitizen) {
							if (E_AIState::dead != pCitizen->GetCurAIState())
								if (E_AIState::runAway != pCitizen->GetAIState()) {
									wstring strHasGunSoundPath = Sound_OMG + std::to_wstring((rand() % Sound_OMG_Len) + 1);
									CSound* pSound = CResourceManager::GetInstance()->GetSound(strHasGunSoundPath, strHasGunSoundPath);
									pSound->Play();
									pCitizen->Runaway();
								}
							continue;
						}
						CGTA_Cop* pCop = dynamic_cast<CGTA_Cop*>(vecObjs[i]);
						if (pCop) {
							if (E_AIState::dead != pCop->GetCurAIState()) {
								if (E_AIState::trace != pCop->GetAIState()) {
									int iRandom = (rand() % Sound_HoldIt_Len) + 1;
									wstring strPath = Sound_HoldIt + std::to_wstring(iRandom);
									CSound* pFreezeSoun = CResourceManager::GetInstance()->GetSound(strPath, strPath);
									pFreezeSoun->Play();
									pCop->Trace();
								}
							}
							continue;
						}
					}
				}
			}
		}
	}
}

void CGTA_Vehicle::DriveUpdate()
{
	Vector3 vHeadDir = GetUpVector();
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		float fSpeed = GetRigidbody()->GetSpeed() * 40.f;
		fSpeed = min(fSpeed, 180.f);
		RotateRP(-fSpeed * DeltaTime);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		float fSpeed = GetRigidbody()->GetSpeed() * 40.f;
		fSpeed = min(fSpeed, 180.f);
		RotateRP(fSpeed * DeltaTime);
	}

	if (InputKeyHold(E_Key::UP)) {
		Debug->Print(Vector2(10, 300), L"ddd", -GetUpVector().x, -GetUpVector().y, -GetUpVector().z);
		Debug->Print(Vector2(10, 350), L"d", VehicleInfo().fPower);

		GetRigidbody()->AddForce(-GetUpVector() * VehicleInfo().fPower * DeltaTime);
		m_bReverse = false;
	}
	if (InputKeyHold(E_Key::DOWN)) {
		Debug->Print(Vector2(70, 300), L"ddd", GetUpVector().x, GetUpVector().y, GetUpVector().z);
		Debug->Print(Vector2(70, 350), L"d", VehicleInfo().fPower);
		GetRigidbody()->AddForce(GetUpVector() * VehicleInfo().fPower * DeltaTime);
		m_bReverse = true;
	}

	State();
}

void CGTA_Vehicle::Explosion()
{
	CSound* pSound = CResourceManager::GetInstance()->GetSound(Sound_Explosion, Sound_Explosion);
	pSound->Stop(true);
	pSound->SetVolume(100.f);
	pSound->Play();
	SetTexture(m_pExplosionTex);
	m_bExplosion = true;
	SetVehicleState(E_VehicleState::explode);
}

void CGTA_Vehicle::State()
{
	switch (m_eVehicleState) {
	case E_VehicleState::normal:
		m_pEngineSound->SetVolume(GetRigidbody()->GetSpeed() * 0.01f + 20.f);

		break;
	case E_VehicleState::explode:
		m_pEngineSound->Stop();
		// 애니메이션 후 -> 아예 타버린 모양으로 하기
		// TODO : 이미지 텍스쳐 변경
		break;
	}
}

void CGTA_Vehicle::InitEngineSound(const wstring& _strSoundKey)
{
	m_pEngineSound = CResourceManager::GetInstance()->GetSound(_strSoundKey, _strSoundKey);
}

void CGTA_Vehicle::InitRadio(int _iTypeIdx)
{
	_iTypeIdx = max(0, _iTypeIdx);
	_iTypeIdx = min(Sound_CarRadio_Len - 1, _iTypeIdx);
	wstring type[] = { Sound_CarRadio_1 ,Sound_CarRadio_2,Sound_CarRadio_3,Sound_CarRadio_4 };
	wstring strRadio = Sound_CarRadio + type[_iTypeIdx];
	m_pRadio = CResourceManager::GetInstance()->GetSound(strRadio, strRadio);
}

void CGTA_Vehicle::SetDriver(CObject* _pDriver)
{
	m_pDriver = _pDriver;

	if (nullptr != m_pDriver) {
		m_pEngineSound->Play(true);
		m_pEngineSound->SetVolume(20.f);
		CSound* pOutDoorSound = CResourceManager::GetInstance()->GetSound(STR_FILE_PATH_GTA_Sound_OutdoorNoise_Sound, STR_FILE_PATH_GTA_Sound_OutdoorNoise_Sound);
		pOutDoorSound->Stop();
		m_pRadio->SetVolume(20.f);
		m_pRadio->Play(true);
	}
	else {
		m_pEngineSound->Stop();
		CSound* pOutDoorSound = CResourceManager::GetInstance()->GetSound(STR_FILE_PATH_GTA_Sound_OutdoorNoise_Sound, STR_FILE_PATH_GTA_Sound_OutdoorNoise_Sound);
		pOutDoorSound->PlayToBGM(true);
		m_pRadio->Stop();
	}
		
}

void TVehicleInfo::Save(FILE* _pFile)
{
	fwrite(&fHp, sizeof(float), 1, _pFile);
	fwrite(&fMoveSpeed, sizeof(float), 1, _pFile);
	fwrite(&fPower, sizeof(float), 1, _pFile);
}

void TVehicleInfo::Load(FILE* _pFile)
{
	fread(&fHp, sizeof(float), 1, _pFile);
	fread(&fMoveSpeed, sizeof(float), 1, _pFile);
	fread(&fPower, sizeof(float), 1, _pFile);
}