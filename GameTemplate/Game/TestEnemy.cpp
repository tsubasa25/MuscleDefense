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
		
		//�R���W�����쐬�B
		m_CapsuleCollider.Init(m_radius, m_height);

		//���̂��������B
		RigidBodyInitData rbInfo;
		rbInfo.collider = &m_CapsuleCollider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Init(rbInfo);

		PositionUpdate();

		m_skinModelRender->SetShadowCasterFlag(true);//�e�̐���
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
			// �p�X����
			m_pathFiding.Execute(
				m_path,							// �\�z���ꂽ�p�X�̊i�[��
				backGround->GetNaviMesh(),		// �i�r���b�V��
				m_position,						// �J�n���W
				m_targetPointPosition,			// �ړ��ڕW���W
				PhysicsWorld::GetInstance(),	// �����G���W��	
				50.0f,							// AI�G�[�W�F���g�̔��a
				200.0f							// AI�G�[�W�F���g�̍����B
			);
		}
		// �p�X����ړ�����B
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
		//���̂𓮂����B
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));

	}
}