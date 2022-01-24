#include "stdafx.h"
#include "SpotLight.h"
namespace nsMuscle {	
	//�X�|�b�g���C�g�̐������炵����
	SpotLight::~SpotLight()
	{
		LightManager::GetInstance()->RemoveSpotLight(int(m_spotLigData->spotLigNum));
	}
	SpotLight::SpotLight()
	{
		m_spotLigData = LightManager::GetInstance()->GetSpotLigData();
		LightManager::GetInstance()->AddSpotLigNum(LightManager::GetInstance()->GetSpotLigData());
	}

	//�X�|�b�g���C�g�̐��𑝂₵����
	bool SpotLight::Start()
	{	
		
		return true;
	}

	//���t���[���J�����̈ʒu�ƃX�|�b�g���C�g�̃f�[�^���X�V
	void SpotLight::Update()
	{
		LightManager::GetInstance()->UpdateSpotLight(int(m_spotLigData->spotLigNum), GetLigData());
	}
}