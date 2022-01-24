#include "stdafx.h"
#include "BackGround.h"
#include "Sky.h"
#include "GoldGym.h"
#include "GameScene.h"
#include "WipeCamera.h"
namespace nsMuscle {	
	BackGround::~BackGround()
	{
		DeleteGO(m_sky);
		DeleteGO(m_skinModelRender);
		for (int i = 0; i < 4; i++){
			DeleteGO(m_gymModelRender[i]);
		}
		DeleteGO(m_skinModelRenderBuilding);
		DeleteGO(m_directionLight);
		for (int i = 0; i < m_goldGym.size(); i++)
		{
			DeleteGO(m_goldGym[i]);
		}
		m_goldGym.clear();
		DeleteGO(m_wipeCamera);
	}
	bool BackGround::Start()
	{
		m_sky = NewGO<Sky>(0);
		m_sky->Init(L"Assets/Image/skyMap/sky2.dds");
		m_sky->SetPosition({ 0,50,0 });

		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init("Assets/modelData/Stage/Road.tkm");
		
		m_skinModelRenderBuilding = NewGO<SkinModelRender>(0);
		m_skinModelRenderBuilding->Init("Assets/modelData/Stage/building.tkm");
		
		m_skinModelRenderBuildingCollider = NewGO<SkinModelRender>(0);
		m_skinModelRenderBuildingCollider->Init("Assets/modelData/Stage/buildingCollider.tkm");
		m_skinModelRenderBuildingCollider->Deactivate();
		for (int i = 0; i < 3; i++)
		{
			m_gymModelRender[i] = NewGO<SkinModelRender>(0);
			m_gymModelRender[i]->SetIsWipeModel(true,i);//ワイプカメラに写す
			m_gymModelRender[i]->SetIsMainModel(false);//メインカメラに写さない

			m_gymModelRender[i]->Init("Assets/modelData/Gym/GymStage.tkm");			
		}
		m_skinModelRender->SetShadowCasterFlag(false);
	//	m_skinModelRenderBuilding->SetShadowCasterFlag(true);
		//ステージのモデルの静的物理モデルを作成
		m_physicsStaticObject.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());
		m_physicsStaticObjectBuilding.CreateFromModel(m_skinModelRenderBuildingCollider->GetModel(), m_skinModelRenderBuildingCollider->GetModel().GetWorldMatrix());
		//レベルを構築する。		
		m_level3D.Init("Assets/levelData/level00.tkl", [&](LevelObjectData& objData) {
			if (objData.EqualObjectName(L"GoldGym") == true) {
				m_goldGym.push_back(NewGO<GoldGym>(0, "goldGym"));
				//配置座標、スケール、回転を取得する。
				m_goldGym[m_goldGymTotalNum]->SetPosition(objData.position);
				m_goldGym[m_goldGymTotalNum]->SetScale(objData.scale);
				m_goldGym[m_goldGymTotalNum]->SetRotation(objData.rotation);
				m_goldGym[m_goldGymTotalNum]->SetGoldGymSpawnNum(m_goldGymTotalNum);
				m_goldGym[m_goldGymTotalNum]->SetLivePlayerNum(m_goldGymTotalNum);//プレイヤー番号
				m_gymModelRender[m_goldGymTotalNum]->SetPosition(objData.position);//ジム背景の位置を設定
				if (m_goldGymTotalNum < nsBGConstant::MAX_GYM_NUM)
				{
					m_goldGymTotalNum++;
				}
				//trueにすると、レベルの方でモデルが読み込まれない。
				return true;
			}
			return false;
		});		
		// ナビメッシュを構築。
		m_nvmMesh.Init("Assets/naviData/StageNavi.tkn");


		m_directionLight = NewGO<DirectionLight>(0);
		// ディレクションライトのデータを設定する
		// ライトは右側から当たっている
		m_directionLight->SetDirection({ m_dir });
		// ライトのカラーは白
		m_directionLight->SetColor({ 3.0f,3.0f,3.0f });

		//地面色、天球色、地面の法線データを設定
		// 地面色を設定。
		//LightManager::GetInstance()->SetGroundColor({ 0.5f,0.3f,0.3f });

		//天球色を設定。
		//LightManager::GetInstance()->SetSkyColor({ 0.1f, 0.1f, 0.1f });

		//地面の法線を設定。
		//LightManager::GetInstance()->SetGroundNormal({ 0.0f,1.0f,0.0f });

		LightManager::GetInstance()->SetAmbientLight({ 0.3f, 0.4f, 0.3f });
		m_gameScene = FindGO<GameScene>("gameScene");

		m_wipeCamera=NewGO<nsMuscle::WipeCamera>(0, "wipeCamera");//ワイプカメラ用クラス

		return true;
	}
	void BackGround::Update()
	{
		if (m_isGameEnd ==true)//
		{
			//DeleteGO(this);
		}
		if (m_isBreakOnce == true)
		{
			BreakGym();
			m_isBreakOnce = false;
		}
	}
	void BackGround::BreakGym()
	{
		for (int i = 0; i < WIPE_MAX_NUM; i++)
		{
			if (m_gameScene->GetIsBreakGym(i))//壊れたジムを探す
			{
				if (m_gameScene->GetNowWarNum() != m_goldGym[i]->GetLivePlayerNum()) {//ジム番号と現在戦っているプレイヤー番号が同じならプレイヤーは死なない
					m_gameScene->GetPlayer(m_goldGym[i]->GetLivePlayerNum()).SetIsDeath(true);
					m_gameScene->DeathPlayer(m_goldGym[i]->GetLivePlayerNum());
				}
				DeleteGO(m_goldGym[i]);//最後のデータを削除
				//m_goldGym.erase(m_goldGym.begin() + i);
				//m_goldGymTotalNum--;//ジムの数を減らす				
				return;
			}
		}
	}
}