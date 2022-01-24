#pragma once
#include "Path.h"
#include "PathFinding.h"
namespace nsMuscle {
	namespace nsEnemyConstant {
		//const Vector3 INI_POSITION = { 0.0f,0.0f,-500.0f };
	}
	class GameScene;
	class TestEnemy :public IGameObject
	{
	public:
		~TestEnemy();
		bool Start();
		void Update();
		void EnemyMove();//移動処理
		void SetEnemyNum(int num) { m_enemyNum = num; }//プレイヤー番号を設定する
		SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }
		Vector3 GetMoveSpeed() { return m_moveSpeed; }
		Vector3 GetPosition() { return m_position; }
		void SetPosition(Vector3 pos) { m_position = pos; }

		void PositionUpdate();

	private:
		int m_enemyNum = 0;
		//CharacterController m_charaCon;//プレイヤーの物理カプセル

		CCapsuleCollider m_CapsuleCollider;
		RigidBody m_rigidBody;
		float m_radius = 10.0f;				//!<カプセルコライダーの半径。
		float m_height = 100.0f;				//!<カプセルコライダーの高さ。
		
		SkinModelRender* m_skinModelRender = nullptr;
		Vector3 m_position = { 0.0f,0.0f,-500.0f };
		Vector3 m_scale = { 1.5f,1.5f, 1.5f };
		Quaternion m_qrot = Quaternion::Identity;
		float m_hp = 100.0f;
		Vector3 m_moveSpeed = Vector3::Zero;
		Vector3 m_playerDir = Vector3::AxisZ;
		float m_velocity = 1.0f;//移動スピード倍率
		Vector3 m_playerPos = Vector3::Zero;
		
		Vector3 m_targetPointPosition;

		Path m_path;
		PathFinding m_pathFiding;

		GameScene* m_gameScene = nullptr;
	};
}
