#include "stdafx.h"
#include "CGTA_IdleState.h"

CGTA_IdleState::CGTA_IdleState() :
	m_pTarget(nullptr)
{
}

CGTA_IdleState::~CGTA_IdleState()
{
}

void CGTA_IdleState::Update()
{
	// idle로 하기는 쫌 그러네 ㅇㅇㅇ 차라리 wander가 좋을듯?
	// 
	// 어떻게 해야될까

}

void CGTA_IdleState::Start()
{
	// 길거리를 배회하기 위해 타일을 설정한다.

}

void CGTA_IdleState::End()
{
	// 뭘 해야되지?

}