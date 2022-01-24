#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAnimation.h"
#include "EnemyVoice.h"
#include "GameScene.h"
#include "NaviMesh.h"
#include "BackGround.h"
#include "GoldGym.h"
#include "Player.h"
#include "HUD.h"
#include "Map.h"

namespace nsMuscle {
	Enemy::~Enemy()
	{				
		DeleteGO(m_skinModelRender);
		DeleteGO(m_enemyAnimation);
		DeleteGO(m_enemyVoice);
		if (m_isWaveEnemy == false&& m_isMapDisplay) {
			m_map->DeleteEnemySprite(m_enemyNum);
		}
		//PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
	}
	bool Enemy::Start()
	{
		m_gameScene = FindGO<GameScene>("gameScene");
		m_enemyAnimation = NewGO<EnemyAnimation>(0,"enemyAnimation");
		m_enemyAnimation->SetParent(*this);
		m_enemyVoice = NewGO<EnemyVoice>(0, "enemyVoice");
		m_enemyVoice->SetParent(*m_enemyAnimation);
		m_enemyAnimation->SetEnemyVoice(*m_enemyVoice);
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->InitSkeleton("Assets/modelData/Enemy/Enemy.tks", m_enemyAnimation->GetAnimationClips(), m_enemyAnimation->GetAnimationClipNum());
		if (m_isWaveEnemy) {
			m_skinModelRender->Init("Assets/modelData/Enemy/WaveEnemy.tkm");
		}
		else {
			m_skinModelRender->Init("Assets/modelData/Enemy/EnemyTest.tkm");
		}
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetRotation(m_qrot);
		// m_skinModelRender->Deactivate();
		//キャラコンの初期化
		m_charaCon.Init(m_radius, m_position);

		m_backGround = FindGO<BackGround>("backGround");
		//コリジョン作成。		
		//m_CapsuleCollider.Init(m_radius, m_height);

		//剛体を初期化。
		//RigidBodyInitData rbInfo;
		//rbInfo.collider = &m_CapsuleCollider;
		//rbInfo.mass = 0.0f;
		//m_rigidBody.Init(rbInfo);

		PositionUpdate();
		for (int i = 0; i++; i < 4) {
			m_skinModelRender->SetIsWipeModel(false, i);
		}
		m_skinModelRender->SetShadowCasterFlag(true);//影の生成
		m_skinModelRender->SetScale({ m_scale });
		if (m_isWaveEnemy==false&& m_isMapDisplay) {
			m_map = FindGO<Map>("map");
			m_map->CreateEnemySprite();
			m_map->UpdateEnemySpritePos(m_position, m_enemyNum);
		}
		
		return true;
	}
	void Enemy::Update()
	{		
		notDisplay();//カリング
		if (m_isDamage|| m_enemyAnimation->GetAnimationStatus() == m_enemyAnimation->GetEnStatusStandUp())
		{
			m_enemyStatus = enEnemyStatus_Damage;
		}
		if (m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave1
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave2
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave3)
		{
			m_enemyVoice->SetIsVoice(true);
			if (m_diffPlayerPos.Length() > 1000.0f)
			{
				m_enemyVoice->SetIsVoice(false);
			}
			switch (m_enemyStatus)
			{
			case nsMuscle::Enemy::enEnemyStatus_Idle:
				break;
			case nsMuscle::Enemy::enEnemyStatus_War:
				EnemyAttack();
				break;
			case nsMuscle::Enemy::enEnemyStatus_Move:
				EnemyMove();
				break;
			case nsMuscle::Enemy::enEnemyStatus_Damage:
				KnockBack();
				break;
			default:
				break;
			}
		}
		else
		{
			m_enemyVoice->SetIsVoice(false);
		}		
		if (m_hp <= 0 && !m_skinModelRender->IsPlayingAnimation()//HPが0以下でアニメーションが止まっている
			//かつ、死亡アニメーションかノックバックアニメーションの後であること
			 && (m_enemyAnimation->GetAnimationStatus() == m_enemyAnimation->GetEnStatusDie() || m_enemyAnimation->GetAnimationStatus() == m_enemyAnimation->GetEnStatusKnockBack()))//
		{
			m_dieTimer++;
			m_position.y -= 0.8f;
			m_skinModelRender->SetPosition(m_position);
			if(m_dieTimer>50)
			DeleteGO(this);
		}
		if (m_hp <= 0&& m_isKillNumUpdate) {
			if (m_gameScene->GetEnGameStatus() != GameScene::enGameStatus_WaveWin)
			{
				m_isKillNumUpdate = false;
				if (m_isWaveEnemy) {
					m_gameScene->AddRedWaveEnemyDethNum();
				}
				else {
					m_gameScene->AddWaveEnemyDethNum();//敵死亡数をインクリメント
				}
				m_gameScene->SetEnemyTotalNumber(-1);
				HUD* hUD = FindGO<HUD>("hUD");
				if (hUD != nullptr) {
					hUD->SetIsKillNumUpdate();
				}
			}
			m_enemyVoice->SetIsVoice(false);//敵は声を出さない
			m_charaCon.RemoveRigidBoby();
		}
		if (m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_WaveWin)
		{
			m_hp = -1;
		}
		if (m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Confirmation|| 
			m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Clear)
		{
			DeleteGO(this);
		}
		
		//ProteinTreatment();//プロテインメーターが貯まると味方になる
	}
	void Enemy::EnemyMove()
	{
		m_moveSpeed = Vector3::Zero;
		
		EnemyMoveToPlayer();
		if (m_isGymAttack) {
			EnemyMoveToGym();
		}

		//進行方向に向きを合わせる
		Vector3 Dir = m_playerDir;
		Dir.y = 0.f;
		Dir.Normalize();//大きさを位置にする
		float x = Dir.Dot(Vector3::AxisX);//X軸から何度ずれているかを入れる
		Dir.z *= -1.f;
		float angleX = acosf(x);//アークコサイン
		if (Dir.z < 0.f) {
			angleX *= -1.f;
		}
		angleX += 0.5f * 3.14159;//90度だけまわす

		m_qrot.SetRotationY(angleX);//ｘ度だけY軸を回す
		m_skinModelRender->SetRotation(m_qrot);//角度を設定する

		PositionUpdate();//剛体の位置を更新
		//座標を設定。
		m_skinModelRender->SetPosition(m_position);
		if (m_isWaveEnemy == false&& m_isMapDisplay) {
			m_map->UpdateEnemySpritePos(m_position, m_enemyNum);
		}
	}
	void Enemy::EnemyAttack()
	{	
		if (m_isGymAttack&& m_isPlayerDamage)
		{
			if (m_isWaveEnemy) {
				GymAnyDamage(1.0f);
			}
			else{
				GymAnyDamage(3.0f);
			}
			m_isPlayerDamage = false;
		}
		if (m_isPlayerDamage) {
			PlayerAnyDamage(0.05f);
			m_isPlayerDamage = false;
		}		
	}
	
	void Enemy::ProteinTreatment()
	{
		if (m_proteinMeter > 100)
		{
			DeleteGO(this);
		}
	}
	void Enemy::KnockBack()
	{
		//３回目の攻撃を受けると少し浮く
		if (m_isKnockBack)
		{
			Vector3 diff = m_playerPos - m_position;
			diff.Normalize();
			m_moveSpeed = diff * -5.0f;	//うしろ方向に速度を設定して、
			//m_moveSpeed.y = 0.0f;
			m_position += m_moveSpeed;
			PositionUpdate();
			//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			float x = diff.Dot(Vector3::AxisX);//X軸から何度ずれているかを入れる
			diff.z *= -1.f;
			float angleX = acosf(x);//アークコサイン
			if (diff.z < 0.f) {
				angleX *= -1.f;
			}
			angleX += 0.5f * 3.14159;//90度だけまわす

			m_qrot.SetRotationY(angleX);//ｘ度だけY軸を回す
			m_skinModelRender->SetRotation(m_qrot);//角度を設定する

			m_skinModelRender->SetPosition(m_position);

			m_knockBackLoopCount++;
			if (m_knockBackLoopCount > 15) {
				m_isKnockBack = false;
				m_knockBackLoopCount = 0;
			}
		}
	}
	void Enemy::PositionUpdate()
	{
		//btRigidBody* btBody = m_rigidBody.GetBody();
		////剛体を動かす。
		//btBody->setActivationState(DISABLE_DEACTIVATION);
		//btTransform& trans = btBody->getWorldTransform();
		////剛体の位置を更新。
		//trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
		m_position=m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	}
	void Enemy::EnemyMoveToPlayer()
	{
		//NotDisplayでm_diffPlayerPosは計算済み
		if (m_diffPlayerPos.Length() < 1000.0f)//プレイヤーが近くにいれば
		{
			m_isGymAttack = false;//ジムが近くにあってもプレイヤーに来る（本当は逆のやつもほしい）
			m_playerDir = m_diffPlayerPos;
			m_playerDir.Normalize();
			m_moveSpeed += m_playerDir * m_velocity;
			//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			//m_position += m_moveSpeed;
			//m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
			if (m_diffPlayerPos.Length() < nsEnemyConstant::PLATER_ATTACK_LENGE) {
				m_enemyStatus = enEnemyStatus_War;
				m_isAttack = true;
			}
			m_enemyVoice->SetIsVoice(true);//敵は声を出す
		}
		else
		{
			m_enemyVoice->SetIsVoice(false);//敵は声を出さない
			m_isGymAttack = true;
		}
	}
	void Enemy::EnemyMoveToGym()
	{		
		for (int i = 0; i < m_backGround->GetGoldGymTotalNum(); i++)
		{
			if (m_gameScene->GetIsBreakGym(i))
			{
				return;
			}
			m_diffGoldGym = m_backGround->GetGoldGym(i).GetPosition() - m_position;
			if (m_diffGoldGym.Length() < 1000.0f)//ジムが近くにあれば
			{
				m_playerDir = m_diffGoldGym;
				m_playerDir.Normalize();
				m_moveSpeed += m_playerDir * m_velocity;
				//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
				//m_position += m_moveSpeed;
				//m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

				if (m_diffGoldGym.Length() < 400) {
					m_enemyVoice->SetIsVoice(false);
					m_enemyStatus = enEnemyStatus_War;
					m_nearGymNumber = m_backGround->GetGoldGym(i).GetGoldGymSpawnNum();
					m_isAttack = true;
				}
			}
		}		
	}
	void Enemy::PlayerAnyDamage(float damage)
	{
		Vector3 diff=m_position-m_gameScene->GetNowWarPlayer().GetPosition();
		if (diff.Length() < nsEnemyConstant::PLATER_ATTACK_LENGE)
		{
			m_gameScene->GetNowWarPlayer().AnyDamage(damage);
			m_gameScene->GetNowWarPlayer().SetIsRePlay(true);//リアクションを最初から再生させる
		}
		//m_isAttackOn = false;//敵は一定時間無敵
	}
	void Enemy::GymAnyDamage(float damage)
	{		
		m_backGround->GetGoldGym(m_nearGymNumber).AnyDamage(damage);
	}
	void Enemy::notDisplay()
	{
		m_diffPlayerPos = m_gameScene->GetNowWarPlayer().GetPosition() - m_position;
		Vector4 _screenPos;
		_screenPos.Set(m_position.x, m_position.y, m_position.z, 1.0f);
		g_camera3D[0]->GetViewProjectionMatrix().Apply(_screenPos);
		m_screenPos.x = (_screenPos.x / _screenPos.w);
		m_screenPos.y = (_screenPos.y / _screenPos.w);
		m_screenPos.z = (_screenPos.z / _screenPos.w);		
		if (m_screenPos.x > 1.0f || m_screenPos.x < -1.0f||//画面外にいれば
			m_screenPos.y>1.0f || m_screenPos.y < -1.0f||
			m_screenPos.z > 1.0f || m_screenPos.z < 0.0f||
			m_diffPlayerPos.Length() > 2000.0f)//プレイヤーが遠くにいれば
		{
			m_skinModelRender->Deactivate();//敵を非表示
		}
		else
		{
			m_skinModelRender->Activate();
		}
		if (m_gameScene->GetEnGameStatus()==GameScene::enGameStatus_WaveEnemyPop&&m_isWaveEnemy) {//強制表示
			m_skinModelRender->Activate();//表示する
		}
	}
}