#include "stdafx.h"
#include "DirectionLight.h"
namespace nsMuscle {
	DirectionLight::~DirectionLight()
	{
		//LightManager::GetInstance()->RemoveDirectionLight(int(m_dirLigData->directionLigNum));
	}
	DirectionLight::DirectionLight()
	{
		m_dirLigData = LightManager::GetInstance()->GetDirLigData();
		//m_dirLigData->directionLigNum = LightManager::GetInstance()->GetDirectionLigNum();//���ꂽ�ԍ����L�^����
		//LightManager::GetInstance()->AddDirectionLigNum(LightManager::GetInstance()->GetDirectionLigNum());
	}
	bool DirectionLight::Start()
	{
		return true;
	}
	void DirectionLight::Update()
	{
		//LightManager::GetInstance()->UpdateDirctionLight(int(m_dirLigData->directionLigNum), GetLigData());
	}
}