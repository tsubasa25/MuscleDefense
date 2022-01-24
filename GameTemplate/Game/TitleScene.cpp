#include "stdafx.h"
#include "TitleScene.h"
#include "BackGround.h"
#include "GameScene.h"
#include "Map.h"

namespace nsMuscle {
	TitleScene::~TitleScene()
	{
		
	}
	bool TitleScene::Start()
	{
		m_backGround=NewGO<BackGround>(0, "backGround");
		m_titleLogo=NewGO<SpriteRender>(1);
		m_titleLogo->Init("Assets/image/TitleLogo.DDS", 1280.0f, 720.0f);
		m_startLogo = NewGO<SpriteRender>(1);
		m_startLogo->Init("Assets/image/START.dds", 1280.0f, 720.0f);
		g_camera3D[0]->SetTarget({ 0.0f,500.0f,0.0f });		
		GameObjectManager::GetInstance()->SetWipeScreenMode(false);
		PostEffectManager::GetInstance()->SetBokeAlphaBlendMode(AlphaBlendMode_None);
		return true;
	}
	void TitleScene::Update()
	{
		switch (m_titleStatus)
		{
		case nsMuscle::TitleScene::enStart:
			MoveCamera();
			break;
		case nsMuscle::TitleScene::enOnButton:
			GameStart();
			break;
		default:
			break;
		}

	}
	void TitleScene::MoveCamera()
	{
		m_rotation = 0.001;
		m_qRot.SetRotationY(m_rotation);
		m_qRot.Apply(m_cameraDir);
		m_cameraPos = m_cameraDir * 1000;

		g_camera3D[0]->SetPosition(m_cameraPos);
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_titleStatus= enOnButton;
			SoundOneShotPlay(L"Assets/sound/Determination.wav", nsTitleSceneConstant::DETERMINATION_SE_VOL);
		}
	}
	void TitleScene::GameStart()
	{
		GameScene* gameScene = FindGO<GameScene>("gameScene");
		gameScene->SetCameraPos(m_cameraPos);
		gameScene->SetEnGameStatus(GameScene::EnGameStatus::enGameStatus_Opening);
		DeleteGO(m_titleLogo);
		DeleteGO(m_startLogo);
		//PostEffectManager::GetInstance()->SetBlurMode(false);
		PostEffectManager::GetInstance()->SetBokeAlphaBlendMode(AlphaBlendMode_Add);
		PostEffectManager::GetInstance()->SetIsWipeRender(true);
		//m_backGround->GetDirectionLight().SetColor({ 5.5f,5.7f,5.5f });
		Map* map = NewGO<Map>(0, "map");
		GameObjectManager::GetInstance()->SetWipeScreenMode(true);
		DeleteGO(this);
	}
}