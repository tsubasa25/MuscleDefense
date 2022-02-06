#include "stdafx.h"
#include "ResultScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "BackGround.h"
#include "PlayerAnimation.h"
#include "Map.h"
namespace nsMuscle
{
	ResultScene::~ResultScene()
	{		
		if (m_isTitle == false) {//ゲーム終了			
			GameObjectManager::GetInstance()->SetIsEnd(true);
		}
		else {
			TitleScene* titleScene = NewGO<nsMuscle::TitleScene>(0, "titleScene");//タイトル
			GameScene* gameScene = NewGO<nsMuscle::GameScene>(0,"gameScene");//ゲームシーン
		}
	}
	bool ResultScene::Start()
	{
		m_gameScene = FindGO<GameScene>("gameScene");
		for (int i = 0; i < PLAYER_MAX_NUM; i++)
		{
			if (m_gameScene->GetIsDeathPlayer(i) == false)
			{					
				m_gameScene->GetPlayer(i).GetSkinModelRender()->SetIsMainModel(true);
				if (m_isClear)
				{
					switch (i)
					{
					case 0:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER0_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER0_POS);
						m_gameScene->GetPlayer(i).GetPlayerAnimation().SetAnimationStatus(PlayerAnimation::EnPlayerAnimStatus::enStatus_Clear0);
						break;
					case 1:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER1_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER1_POS);
						m_gameScene->GetPlayer(i).GetPlayerAnimation().SetAnimationStatus(PlayerAnimation::EnPlayerAnimStatus::enStatus_Clear1);
						break;
					case 2:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER2_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER2_POS);
						m_gameScene->GetPlayer(i).GetPlayerAnimation().SetAnimationStatus(PlayerAnimation::EnPlayerAnimStatus::enStatus_Clear2);
						break;
					case 3:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER3_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER3_POS);
						m_gameScene->GetPlayer(i).GetPlayerAnimation().SetAnimationStatus(PlayerAnimation::EnPlayerAnimStatus::enStatus_Clear3);
						break;
					default:
						break;
					}
				}
				else
				{
					for (int j = 0;j< m_gameScene->GetPlayer(i).GetSkinModelRender()->GetSkeleton().GetNumBones()-1; j++) {//
						m_gameScene->GetPlayer(i).SetMuscleRateArray(j,0.0f);
					}
					switch (i)
					{
					case 0:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER0_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER0_POS);
						break;
					case 1:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER1_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER1_POS);
						break;
					case 2:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER2_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER2_POS);
						break;
					case 3:
						m_gameScene->GetPlayer(i).SetPosition(nsResultSceneConstant::RESULT_PLAYER3_POS);
						m_gameScene->GetPlayer(i).GetSkinModelRender()->SetPosition(nsResultSceneConstant::RESULT_PLAYER3_POS);
						break;
					default:
						break;
					}
					m_gameScene->GetPlayer(i).GetPlayerAnimation().SetAnimationStatus(PlayerAnimation::EnPlayerAnimStatus::enStatus_Lose);
				}
			}
		}
		m_toTitle = NewGO<FontRender>(2);
		m_toTitle->SetText(L"タイトルへ");
		m_toTitle->SetPosition(nsResultSceneConstant::TITLE_FONT_POS);
		m_toTitle->SetColor(Vector4::Red);
		m_toTitle->SetShadowFlag(true);
		m_toTitle->SetShadowColor(Vector4::Black);
		m_toTitle->SetScale(nsResultSceneConstant::FONT_SCALE);
		m_toReStart = NewGO<FontRender>(2);
		m_toReStart->SetText(L"おわる");
		m_toReStart->SetPosition(nsResultSceneConstant::RESTART_FONT_POS);
		m_toReStart->SetColor(Vector4::White);
		m_toReStart->SetShadowFlag(true);
		m_toReStart->SetShadowColor(Vector4::Black);
		m_toReStart->SetScale(nsResultSceneConstant::FONT_SCALE);

		m_title = NewGO<SpriteRender>(2);
		if (m_isClear) {
			m_title->Init("Assets/Image/result/Clear.dds", UINT(nsResultSceneConstant::SPRITE_SCALE.x), UINT(nsResultSceneConstant::SPRITE_SCALE.y));
		}
		else {
			m_title->Init("Assets/Image/result/Fail.dds", UINT(nsResultSceneConstant::SPRITE_SCALE.x), UINT(nsResultSceneConstant::SPRITE_SCALE.y));
		}
		m_title->SetPosition(nsResultSceneConstant::TITLE_POS);
		g_camera3D[0]->SetPosition(nsResultSceneConstant::RESULT_CAMERA_POS);
		g_camera3D[0]->SetTarget(nsResultSceneConstant::RESULT_CAMERA_TARGET);
		/*if (m_isClear)
		{
			SoundOneShotPlay(L"Assets/sound/gameclear.wav", 1.0f);
		}
		else
		{
			SoundOneShotPlay(L"Assets/sound/gameover.wav", 1.0f);
		}*/
		return true;
	}
	void ResultScene::Update()
	{		
		NextScene();
	}
	void ResultScene::CreateSprite()
	{		
		m_title = NewGO<SpriteRender>(2);
		if (m_isClear) {
			m_title->Init("Assets/Image/result/Clear.dds", UINT(nsResultSceneConstant::SPRITE_SCALE.x), UINT(nsResultSceneConstant::SPRITE_SCALE.y));
		}
		else {
			m_title->Init("Assets/Image/result/Fail.dds", UINT(nsResultSceneConstant::SPRITE_SCALE.x), UINT(nsResultSceneConstant::SPRITE_SCALE.y));
		}
	}
	void ResultScene::NextScene()
	{
		if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[0]->IsTrigger(enButtonUp)) {
			if (m_isTitle)
			{
				m_isTitle = false;
				m_toTitle->SetColor(Vector4::White);
				m_toReStart->SetColor(Vector4::Red);
			}
			else
			{
				m_isTitle = true;
				m_toTitle->SetColor(Vector4::Red);
				m_toReStart->SetColor(Vector4::White);
			}
		}
		if (g_pad[0]->IsPress(enButtonA))
		{
			BackGround* backGround = FindGO<BackGround>("backGround");
			DeleteGO(backGround);
			DeleteGO(m_title);
			DeleteGO(m_toTitle);
			DeleteGO(m_toReStart);
			DeleteGO(m_gameScene);
			DeleteGO(this);
		}
	}
}