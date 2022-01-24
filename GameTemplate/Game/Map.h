#pragma once
#include "stdafx.h"
namespace nsMuscle {
	class GameScene;
	class BackGround;
	class Map:public IGameObject
	{
	public:
		~Map();
		bool Start();
		void Update();

		//void SetSpritePos();

		void CreateEnemySprite();

		void UpdateEnemySpritePos(Vector3 pos,int enemyNum);

		void DeleteEnemySprite(int enemyNum) { m_enemySprite[enemyNum]->Deactivate();  }

		void ClearEnemySprite();

		void MinusEnemySpriteNum() { m_enemySpriteNum--; }
	private:
		SpriteRender* m_mapSprite = nullptr;
		SpriteRender* m_gymSprite[WIPE_MAX_NUM] = { nullptr };
		SpriteRender* m_playerSprite = nullptr;
		std::vector<SpriteRender*> m_enemySprite ;
		
		Vector3 m_mapPos = { 530.0f,-250,0.0f };//{640.0f,-360,0.0f};
		Vector3 m_gymSpritePos[WIPE_MAX_NUM] = { {Vector3::Zero},{Vector3::Zero} ,{Vector3::Zero} };
		Vector3 m_playerSpritePos = Vector3::Zero;
		std::vector<Vector3>* m_enemySpritePos;

		GameScene* m_gameScene = nullptr;
		BackGround* m_backGround = nullptr;

		int m_enemySpriteNum = 0;
	};
}