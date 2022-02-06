#include "stdafx.h"
#include "GoldGym.h"
#include "BackGround.h"
#include "GameScene.h"
#include "WipeCamera.h"
#include "HUD.h"
namespace nsMuscle {	
	GoldGym::~GoldGym()
	{
		DeleteGO(m_modelRender);
	}
	bool GoldGym::Start()
	{
		m_modelRender = NewGO<SkinModelRender>(0);
		m_modelRender->Init("Assets/modelData/Stage/GoldGym.tkm");
		m_modelRender->SetPosition(m_position);
		m_modelRender->SetRotation(m_rotation);
		m_modelRender->SetScale(m_scale);
		//m_modelRender->SetShadowCasterFlag(true);
		m_modelRender->Update();
		m_physicsStaticObject.CreateFromModel(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix());
		m_backGround = FindGO<BackGround>("backGround");
		m_wipeCamera = FindGO<WipeCamera>("wipeCamera");
		m_hud = FindGO<HUD>("hUD");
		return true;
	}
	void GoldGym::Update()
	{
		if (m_goldGymHP < 0)
		{
			m_isBreak = true;
			m_backGround->SetIsBreakOnce();
			GameScene*gameScene=FindGO<GameScene>("gameScene");
			gameScene->SetIsBreakGym(m_goldGymSpawnNum);
		}
		if (m_goldGymOldHP == m_goldGymHP)
		{	
			m_goldGymOldHP = m_goldGymHP;
			m_numberSameCount++;
			if (m_numberSameCount == 30)//ˆê’èŽžŠÔUŒ‚‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î
			{
				m_wipeCamera->SetIsSwing(false, m_goldGymSpawnNum);//ƒJƒƒ‰‚ð—h‚ç‚³‚È‚¢
				g_camera3D[m_goldGymSpawnNum + 1]->SetTarget(m_wipeCamera->GetTargetPos(m_goldGymSpawnNum));
			}
		}
		else
		{
			m_goldGymOldHP = m_goldGymHP;
			m_numberSameCount = 0;
		}
	}
	void GoldGym::AnyDamage(int num)
	{ 
		m_goldGymHP -= num; 
		m_wipeCamera->SetIsSwing(true, m_goldGymSpawnNum);
		m_hud->SetGymHPParameter(m_goldGymSpawnNum, m_goldGymHP);
	}
}