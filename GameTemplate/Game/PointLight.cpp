#include "stdafx.h"
#include "PointLight.h"
namespace nsMuscle {
	//ポイントライトの数を減らした時
	PointLight::~PointLight()
	{
		LightManager::GetInstance()->RemovePointLight(int(m_pointLigData->pointLigNum));
	}
	PointLight::PointLight()//ポイントライトの数を増やした時
	{
		m_pointLigData = LightManager::GetInstance()->GetPointLigData();//データを確保する
		m_pointLigData->pointLigNum = LightManager::GetInstance()->GetPointLigNum();//作られた番号を記録する
		LightManager::GetInstance()->AddPointLig(GetLigData());//
	}

	bool PointLight::Start()
	{

		return true;
	}

	//毎フレームカメラの位置とポイントライトのデータを更新
	void PointLight::Update()
	{
		LightManager::GetInstance()->UpdatePointLight(int(m_pointLigData->pointLigNum), GetLigData());
	}
}