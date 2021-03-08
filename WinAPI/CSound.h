#pragma once
#include "CResource.h"
class CSound : public CResource
{
public:
	virtual int Load(const wchar_t* _pFilePath) override;

	CSound();
	~CSound();
};