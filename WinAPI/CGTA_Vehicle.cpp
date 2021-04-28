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

CGTA_Vehicle::CGTA_Vehicle(E_GroupType _eGroupType) :
	CObject(_eGroupType = E_GroupType::VEHICLE),
	m_pDriver(nullptr),
	m_bExplosion(false)
{
}

CGTA_Vehicle::CGTA_Vehicle(const CGTA_Vehicle& _origin) :
	CObject(_origin),
	m_pDriver(nullptr),
	m_bExplosion(_origin.m_bExplosion)
{
}

CGTA_Vehicle::~CGTA_Vehicle()
{
}

void CGTA_Vehicle::Init()
{
	CRigidbody2D* pRigidbody = new CRigidbody2D(this);
	pRigidbody->SetMass(11.f);
	pRigidbody->SetDrag(1.f);
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
	CGTA_Character* pCharacter = dynamic_cast<CGTA_Character*>(_pOther);
	if (pCharacter) {
		// 차량을 타려고하는 캐릭터와 접촉 시 
		if (E_CharacterState::getInTheVehicle == pCharacter->GetCharacterState()) {
			// TODO : 차량의 속도에 따라 탈지 말지 결정하기
			pCharacter->Drive();
			pCharacter->SetDrive(true);
			CGTA_Player* pPlayer = dynamic_cast<CGTA_Player*>(GetDriver());
			if (pPlayer) { // 플레이어면
				pPlayer->SetActiveAI(false); // AI를 끈다.
			}
			else {
				// TODO : 운전한다.
				// GetCharacter()->GetAI()->ChangeState(L"drive");
			}
		}
		return;
	}

	CGTA_Bullet* pBullet = dynamic_cast<CGTA_Bullet*>(_pOther);
	if (pBullet) {
		VehicleInfo().fHp -= pBullet->GetDamage();
		VehicleInfo().fHp = VehicleInfo().fHp < 0.f ? 0.f : VehicleInfo().fHp;
		if(0.f == VehicleInfo().fHp)
			Explosion();
	}
}

void CGTA_Vehicle::OnCollisionStay(CObject* _pOther)
{
	CGTA_Character* pCharacter = dynamic_cast<CGTA_Character*>(_pOther);
	if (pCharacter) {
		// 지나가다 접촉할 경우
		if (E_CharacterState::getInTheVehicle != pCharacter->GetCharacterState()) {
			float fSpeed = GetRigidbody()->GetSpeed() * 2.f;
			if (fSpeed >= 5.f) {
				pCharacter->CharacterInfo().fHp = 0.f;
				pCharacter->Dead();
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
		GetRigidbody()->AddForce(-GetUpVector() * VehicleInfo().fPower * DeltaTime);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		GetRigidbody()->AddForce(GetUpVector() * VehicleInfo().fPower * DeltaTime);
	}
}

void CGTA_Vehicle::Explosion()
{
	m_bExplosion = true;
	SetVehicleState(E_VehicleState::explode);
}

void CGTA_Vehicle::State()
{
	switch (m_eVehicleState) {
	case E_VehicleState::normal:
		break;
	case E_VehicleState::explode:
		// 애니메이션 후 -> 아예 타버린 모양으로 하기
		// TODO : 이미지 텍스쳐 변경
		break;
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
