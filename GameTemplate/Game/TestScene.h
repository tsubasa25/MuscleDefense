#pragma once
#include "Player.h"
#include "Sky.h"
namespace nsMuscle {
	class TestScene :public IGameObject
	{
	public:
		bool Start();
		void Update();
		~TestScene();
		Sky* m_sky = nullptr;
		SkinModelRender* backGround = nullptr;
		SkinModelRender* m_skinModelRenderBuilding = nullptr;
		PhysicsStaticObject m_physicsStaticObject;				//CharaConとの当たり判定に使用する静的物理オブジェクト
		DirectionLight* light = nullptr;
		Player* player[2] = { nullptr };

		float scale = 0.0f;
	};
}
