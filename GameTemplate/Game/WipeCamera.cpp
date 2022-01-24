#include "stdafx.h"
#include "WipeCamera.h"
#include "BackGround.h"
#include "GoldGym.h"
#include "Level3D.h"
#include "GameScene.h"
namespace nsMuscle {
	WipeCamera::~WipeCamera()
	{

	}
	bool WipeCamera::Start()
	{
		//ƒŒƒxƒ‹‚ð\’z‚·‚éB		
		Level3D level3D;

		level3D.Init("Assets/levelData/level00.tkl", [&](LevelObjectData& objData) {
			if (objData.EqualObjectName(L"GoldGym") == true) {
				Vector3 targetPos = objData.position;
				targetPos.x += 200.0f;
				targetPos.y += 50.0f;
				Vector3 cameraPos = objData.position;
				cameraPos.x -= 150.0f;
				cameraPos.y += 80.0f;
				m_position[m_wipeNum - 1] = cameraPos;
				m_target[m_wipeNum - 1] = targetPos;
				g_camera3D[m_wipeNum]->SetTarget(targetPos);
				g_camera3D[m_wipeNum]->SetPosition(cameraPos);
				m_wipeNum++;
				return true;
			}
			return false;
			});
		m_backGround = FindGO<BackGround>("backGround");
		m_gameScene = FindGO<GameScene>("gameScene");
		return true;
	}
	void WipeCamera::Update()
	{
		for (int i = 0; i < WIPE_MAX_NUM; i++)
		{
			if (m_gameScene->GetIsBreakGym(i) == true)
			{
				return;
			}
			else if (m_isSwing[i])
			{
				SwingCamera(i);
			}
		}
	}
	void WipeCamera::SwingCamera(int num)
	{
		int cameraNum = num + 1;
		m_swingTimer[num]++;
		if (m_swingTimer[num] > 2)
		{
			m_swingTimer[num] = 0;
			int hp=m_backGround->GetGoldGym(num).GetHP();
			float swingWidth = (100.0f - (int(hp / 10.0f)));//—h‚ê•‚ÍƒWƒ€‚ÌHP‚ª­‚È‚¢‚Ù‚Ç‘å‚«‚¢

			//Vector3 pos = m_position[num];
			//pos.z += (rand() % 30) - 15;
			//pos.y += (rand() % 30) - 15;
			//g_camera3D[cameraNum]->SetPosition(pos);
			Vector3 target = m_target[num];
			target.z += (rand() % int(swingWidth)) - (swingWidth/2);
			target.y += (rand() % int(swingWidth)) - (swingWidth/2);
			g_camera3D[cameraNum]->SetTarget(target);
		}
	}
}