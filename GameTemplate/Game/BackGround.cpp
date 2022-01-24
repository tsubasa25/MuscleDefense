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
			m_gymModelRender[i]->SetIsWipeModel(true,i);//���C�v�J�����Ɏʂ�
			m_gymModelRender[i]->SetIsMainModel(false);//���C���J�����Ɏʂ��Ȃ�

			m_gymModelRender[i]->Init("Assets/modelData/Gym/GymStage.tkm");			
		}
		m_skinModelRender->SetShadowCasterFlag(false);
	//	m_skinModelRenderBuilding->SetShadowCasterFlag(true);
		//�X�e�[�W�̃��f���̐ÓI�������f�����쐬
		m_physicsStaticObject.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());
		m_physicsStaticObjectBuilding.CreateFromModel(m_skinModelRenderBuildingCollider->GetModel(), m_skinModelRenderBuildingCollider->GetModel().GetWorldMatrix());
		//���x�����\�z����B		
		m_level3D.Init("Assets/levelData/level00.tkl", [&](LevelObjectData& objData) {
			if (objData.EqualObjectName(L"GoldGym") == true) {
				m_goldGym.push_back(NewGO<GoldGym>(0, "goldGym"));
				//�z�u���W�A�X�P�[���A��]���擾����B
				m_goldGym[m_goldGymTotalNum]->SetPosition(objData.position);
				m_goldGym[m_goldGymTotalNum]->SetScale(objData.scale);
				m_goldGym[m_goldGymTotalNum]->SetRotation(objData.rotation);
				m_goldGym[m_goldGymTotalNum]->SetGoldGymSpawnNum(m_goldGymTotalNum);
				m_goldGym[m_goldGymTotalNum]->SetLivePlayerNum(m_goldGymTotalNum);//�v���C���[�ԍ�
				m_gymModelRender[m_goldGymTotalNum]->SetPosition(objData.position);//�W���w�i�̈ʒu��ݒ�
				if (m_goldGymTotalNum < nsBGConstant::MAX_GYM_NUM)
				{
					m_goldGymTotalNum++;
				}
				//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
				return true;
			}
			return false;
		});		
		// �i�r���b�V�����\�z�B
		m_nvmMesh.Init("Assets/naviData/StageNavi.tkn");


		m_directionLight = NewGO<DirectionLight>(0);
		// �f�B���N�V�������C�g�̃f�[�^��ݒ肷��
		// ���C�g�͉E�����瓖�����Ă���
		m_directionLight->SetDirection({ m_dir });
		// ���C�g�̃J���[�͔�
		m_directionLight->SetColor({ 3.0f,3.0f,3.0f });

		//�n�ʐF�A�V���F�A�n�ʂ̖@���f�[�^��ݒ�
		// �n�ʐF��ݒ�B
		//LightManager::GetInstance()->SetGroundColor({ 0.5f,0.3f,0.3f });

		//�V���F��ݒ�B
		//LightManager::GetInstance()->SetSkyColor({ 0.1f, 0.1f, 0.1f });

		//�n�ʂ̖@����ݒ�B
		//LightManager::GetInstance()->SetGroundNormal({ 0.0f,1.0f,0.0f });

		LightManager::GetInstance()->SetAmbientLight({ 0.3f, 0.4f, 0.3f });
		m_gameScene = FindGO<GameScene>("gameScene");

		m_wipeCamera=NewGO<nsMuscle::WipeCamera>(0, "wipeCamera");//���C�v�J�����p�N���X

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
			if (m_gameScene->GetIsBreakGym(i))//��ꂽ�W����T��
			{
				if (m_gameScene->GetNowWarNum() != m_goldGym[i]->GetLivePlayerNum()) {//�W���ԍ��ƌ��ݐ���Ă���v���C���[�ԍ��������Ȃ�v���C���[�͎��ȂȂ�
					m_gameScene->GetPlayer(m_goldGym[i]->GetLivePlayerNum()).SetIsDeath(true);
					m_gameScene->DeathPlayer(m_goldGym[i]->GetLivePlayerNum());
				}
				DeleteGO(m_goldGym[i]);//�Ō�̃f�[�^���폜
				//m_goldGym.erase(m_goldGym.begin() + i);
				//m_goldGymTotalNum--;//�W���̐������炷				
				return;
			}
		}
	}
}