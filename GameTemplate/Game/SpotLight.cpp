#include "stdafx.h"
#include "SpotLight.h"
namespace nsMuscle {	
	//スポットライトの数を減らした時
	SpotLight::~SpotLight()
	{
		LightManager::GetInstance()->RemoveSpotLight(int(m_spotLigData->spotLigNum));
	}
	SpotLight::SpotLight()
	{
		m_spotLigData = LightManager::GetInstance()->GetSpotLigData();
		LightManager::GetInstance()->AddSpotLigNum(LightManager::GetInstance()->GetSpotLigData());
	}

	//スポットライトの数を増やした時
	bool SpotLight::Start()
	{	
		
		return true;
	}

	//毎フレームカメラの位置とスポットライトのデータを更新
	void SpotLight::Update()
	{
		LightManager::GetInstance()->UpdateSpotLight(int(m_spotLigData->spotLigNum), GetLigData());
	}
}