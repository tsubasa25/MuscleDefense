#include "stdafx.h"
#include "TestScene.h"
#include "Sky.h"
namespace nsMuscle {
	TestScene::~TestScene()
	{

	}
	bool TestScene::Start()
	{
		player[0] = NewGO<Player>(0);
		player[0]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_War);
		player[0]->SetPosition({ -200.0f,0.0f,0.0f, });
		player[1] = NewGO<Player>(0);
		player[1]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_War);
		player[1]->SetPosition({ -60.0f,0.0f,0.0f });		
		g_camera3D[0]->SetPosition({ -130.0f,100.0f,-200.0f });
		g_camera3D[0]->SetTarget({ -130.0f,50.0f,0.0f });

		m_sky = NewGO<Sky>(0);
		m_sky->Init(L"Assets/Image/skyMap/sky2.dds");
		m_sky->SetPosition({ 0,50,0 });

		backGround = NewGO<SkinModelRender>(0);
		backGround->Init("Assets/modelData/Stage/Road.tkm");

		m_skinModelRenderBuilding = NewGO<SkinModelRender>(0);
		m_skinModelRenderBuilding->Init("Assets/modelData/Stage/building.tkm");
		
		//ステージのモデルの静的物理モデルを作成
		m_physicsStaticObject.CreateFromModel(backGround->GetModel(), backGround->GetModel().GetWorldMatrix());

		light = NewGO<DirectionLight>(0);
		light->SetColor({ 3.0f,3.0f,3.0f });
		light->SetDirection({ 0.0f, -1.0f, 1.0f });
		LightManager::GetInstance()->SetAmbientLight({ 0.3f, 0.4f, 0.3f });
		PostEffectManager::GetInstance()->SetBlurMode(false);
		return true;
	}
	void TestScene::Update()
	{
		std::vector<int>boneId;
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:RightArm"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:LeftArm"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:RightShoulder"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:LeftShoulder"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:Spine2"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:LeftForeArm"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:RightForeArm"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:Hips"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:Spine"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:Spine1"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:RightUpLeg"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:RightLeg"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:RightFoot"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:LeftUpLeg"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:LeftLeg"));
		boneId.push_back(player[0]->GetSkinModelRender()->GetSkeleton().FindBoneID(L"mixamorig:LeftFoot"));

		for (int i = 0; i < boneId.size(); i++) {
			player[0]->SetMuscleRateArray(boneId[i], 0.0f);//補間率を代入
			player[1]->SetMuscleRateArray(boneId[i], scale);//補間率を代入			
			//player[1]->GetSkinModelRender()->GetSkeleton().GetBone(boneId[i])->SetScale({ scale+0.6f,scale + 0.6f, scale + 0.6f });
		}
		scale += 0.005f;
		scale = min(1.0f, scale);
	}}