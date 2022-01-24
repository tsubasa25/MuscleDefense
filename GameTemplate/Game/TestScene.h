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
		PhysicsStaticObject m_physicsStaticObject;				//CharaCon�Ƃ̓����蔻��Ɏg�p����ÓI�����I�u�W�F�N�g
		DirectionLight* light = nullptr;
		Player* player[2] = { nullptr };

		float scale = 0.0f;
	};
}
