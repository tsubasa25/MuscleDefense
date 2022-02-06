#pragma once
#include "stdafx.h"
#include "Level3D.h"
#include "NaviMesh.h"
namespace nsMuscle {
	namespace nsBGConstant {
		const Vector3 GYMSTAGE_POSITION[4] = { { -4000.0f,0.0f,0.0f },{ -5000.0f,0.0f,0.0f },{ -6000.0f,0.0f,0.0f },{ -7000.0f,0.0f,0.0f } };
		const int MAX_GYM_NUM = 3;
	}
	class Sky;
	class GoldGym;
	class GameScene;
	class WipeCamera;
	class BackGround : public IGameObject
	{
	public:
		~BackGround();
		bool Start();
		void Update();
		void BreakGym();//ジムが破壊された

		GoldGym& GetGoldGym(int num) { return *m_goldGym[num]; }
		NaviMesh& GetNaviMesh() { return m_nvmMesh; }
		int GetGoldGymTotalNum() { return m_goldGymTotalNum; }
		void MinusGoldGymTotalNum() { m_goldGymTotalNum--; }
		DirectionLight &GetDirectionLight() { return *m_directionLight; }
		void SetIsGameEnd() { m_isGameEnd = true;}		
		void SetIsBreakOnce() { m_isBreakOnce = true; }
	
	private:
		DirectionLight* m_directionLight = nullptr;
		Vector3 m_dir = { 1.0f, -1.0f, -1.0f };

		SkinModelRender* m_skinModelRender = nullptr;
		SkinModelRender* m_skinModelRenderBuilding = nullptr;
		SkinModelRender* m_skinModelRenderBuildingCollider = nullptr;

		SkinModelRender* m_gymModelRender[3] = { nullptr };
		SkinModelRender* m_test = nullptr;
		Sky* m_sky = nullptr;
		PhysicsStaticObject m_physicsStaticObject;				//CharaConとの当たり判定に使用する静的物理オブジェクト
		PhysicsStaticObject m_physicsStaticObjectBuilding;				//CharaConとの当たり判定に使用する静的物理オブジェクト

		Level3D m_level3D;
		std::vector<GoldGym*> m_goldGym;
		int m_goldGymTotalNum = 0;

		NaviMesh m_nvmMesh;

		GameScene* m_gameScene=nullptr;
		bool m_isGameEnd = false;
		bool m_isBreakOnce = false;

		WipeCamera* m_wipeCamera = nullptr;
	};
}