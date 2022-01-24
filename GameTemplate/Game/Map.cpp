#include "stdafx.h"
#include "Map.h"
#include "GameScene.h"
#include "BackGround.h"
#include "GoldGym.h"
#include "Player.h"
namespace nsMuscle {

	Map::~Map()
	{
		DeleteGO(m_mapSprite);
		for (int i = 0; i < WIPE_MAX_NUM; i++){
			DeleteGO(m_gymSprite[i]);
		}
		DeleteGO(m_playerSprite);
		for (int i = 0; i < m_enemySprite.size(); i++)
		{
			DeleteGO(m_enemySprite[i]);
		}
		m_enemySprite.clear();
	}
	bool Map::Start()
	{
		m_gameScene = FindGO<GameScene>("gameScene");
		m_backGround = FindGO<BackGround>("backGround");
		m_mapSprite = NewGO<SpriteRender>(1);
		m_mapSprite->Init("Assets/Image/Map/MapDot5.dds", 200, 200);
		m_mapSprite->SetPosition(m_mapPos);
		for (int i = 0; i < WIPE_MAX_NUM; i++) {
			m_gymSprite[i] = NewGO<SpriteRender>(3);
			switch (i)
			{
			case 0:
				m_gymSprite[i]->Init("Assets/Image/Map/ChestDot.dds", 40, 40);
				break;
			case 1:
				m_gymSprite[i]->Init("Assets/Image/Map/AbdominalDot.dds", 40, 40);
				break;
			case 2:
				m_gymSprite[i]->Init("Assets/Image/Map/legDot.dds", 40, 40);
				break;
			default:
				break;
			}			
			m_gymSprite[i]->SetPivot({ 0.5f,0.3f });
			m_gymSpritePos[i]=m_backGround->GetGoldGym(i).GetPosition();
			m_gymSpritePos[i].y = m_gymSpritePos[i].z;
			m_gymSpritePos[i].z = 0;
			//m_gymSpritePos[i].Normalize();
			m_gymSpritePos[i] /= 25.6;
			m_gymSpritePos[i] += m_mapPos;
			m_gymSprite[i]->SetPosition(m_gymSpritePos[i]);
		}
		m_playerSprite = NewGO<SpriteRender>(3,"playerSprite");
		m_playerSprite->Init("Assets/Image/Map/playerDot.dds", 16, 16);
		return true;
	}
	void Map::Update()
	{
		Vector3 pos=m_gameScene->GetPlayer(m_gameScene->GetNowWarNum()).GetPosition();
		pos.y = pos.z;
		pos.z = 0.0f;
		pos /= 25.6f;
		pos += m_mapPos;
		m_playerSprite->SetPosition(pos);
	}
	void Map::CreateEnemySprite()
	{
		m_enemySprite.push_back(NewGO<SpriteRender>(2));
		m_enemySprite[m_enemySpriteNum]->Init("Assets/Image/Map/enemyDot.dds", 4, 4);
		m_enemySpriteNum++;
	}
	void Map::UpdateEnemySpritePos(Vector3 pos, int enemyNum)
	{
		Vector3 spritePos = pos;
		spritePos.y = spritePos.z;
		spritePos.z = 0.0f;
		spritePos /= 25.6f;
		spritePos += m_mapPos;		
		m_enemySprite[enemyNum]->SetPosition(spritePos);
	}
	void Map::ClearEnemySprite() { 
		for (int i = 0; i < m_enemySprite.size(); i++)
		{
			DeleteGO(m_enemySprite[i]);
		}
		m_enemySprite.clear(); 
		m_enemySpriteNum = 0;
	}
}