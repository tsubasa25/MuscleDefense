#include "stdafx.h"
#include "TestEnemy.h"
#include "GameScene.h"
#include "BackGround.h"
namespace nsMuscle {
	TestEnemy::~TestEnemy()
	{
		m_gameScene->SetEnemyTotalNumber(-1);
		DeleteGO(m_skinModelRender);
		PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
	}
	bool TestEnemy::Start()
	{
		m_gameScene = FindGO<GameScene>("gameScene");
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init("Assets/modelData/Stage/GoldGym.tkm");
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetScale({ 0.1f,0.1f,0.1f });
		
		//コリジョン作成。
		m_CapsuleCollider.Init(m_radius, m_height);

		//剛体を初期化。
		RigidBodyInitData rbInfo;
		rbInfo.collider = &m_CapsuleCollider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Init(rbInfo);

		PositionUpdate();

		m_skinModelRender->SetShadowCasterFlag(true);//影の生成
		//m_skinModelRender->SetScale({ m_scale });


		return true;
	}
	void TestEnemy::Update()
	{
		EnemyMove();
	}
	void TestEnemy::EnemyMove()
	{
		bool isEnd;
		if (g_pad[0]->IsTrigger(enButtonA)) {
			BackGround* backGround = FindGO<BackGround>("backGround");
			m_targetPointPosition = m_gameScene->GetTargetPosition();
			// パス検索
			m_pathFiding.Execute(
				m_path,							// 構築されたパスの格納先
				backGround->GetNaviMesh(),		// ナビメッシュ
				m_position,						// 開始座標
				m_targetPointPosition,			// 移動目標座標
				PhysicsWorld::GetInstance(),	// 物理エンジン	
				50.0f,							// AIエージェントの半径
				200.0f							// AIエージェントの高さ。
			);
		}
		// パス上を移動する。
		m_position = m_path.Move(
			m_position,
			10.0f,
			isEnd
		);
		m_skinModelRender->SetPosition(m_position);
		PositionUpdate();
	}
	void TestEnemy::PositionUpdate()
	{
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));

	}
}