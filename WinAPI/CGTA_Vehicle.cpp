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

CGTA_Vehicle::CGTA_Vehicle(E_GroupType _eGroupType) :
	CObject(_eGroupType = E_GroupType::VEHICLE),
	m_fPower(3000.f),
	m_pDriver(nullptr),
	m_bExplosion(false)
{
}

CGTA_Vehicle::CGTA_Vehicle(const CGTA_Vehicle& _origin) :
	CObject(_origin),
	m_fPower(_origin.m_fPower),
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
	}
}

void CGTA_Vehicle::DriveUpdate()
{
	Vector3 vHeadDir = GetUpVector();
	//RotateInfo().Update();
	if (InputKeyHold(E_Key::LEFT)) {
		RotateRP(-180 * DeltaTime);
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		RotateRP(180 * DeltaTime);
	}

	if (InputKeyHold(E_Key::UP)) {
		GetRigidbody()->AddForce(-GetUpVector() * m_fPower * DeltaTime);
	}
	if (InputKeyHold(E_Key::DOWN)) {
		GetRigidbody()->AddForce(GetUpVector() * m_fPower * DeltaTime);
	}
}