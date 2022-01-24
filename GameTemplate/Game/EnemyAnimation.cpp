#include "stdafx.h"
#include "EnemyAnimation.h"
#include "Enemy.h"
#include "EnemyVoice.h"
namespace nsMuscle {
	namespace {
		const float ENEMY_ATTACK_TIMING = 25.0f;
	}
	EnemyAnimation::~EnemyAnimation()
	{		
	}
	bool EnemyAnimation::Start()
	{		
		m_animationClips[enAnimClip_Idle].Load("Assets/animData/Enemy/Idle.tka");
		m_animationClips[enAnimClip_Idle].SetLoopFlag(true);	//ループモーション
		m_animationClips[enAnimClip_Walk].Load("Assets/animData/Enemy/Run.tka");
		m_animationClips[enAnimClip_Walk].SetLoopFlag(true);	//ループモーション
		m_animationClips[enAnimClip_Run].Load("Assets/animData/Enemy/Run.tka");
		m_animationClips[enAnimClip_Run].SetLoopFlag(true);	//ループモーション
		m_animationClips[enAnimClip_Attack].Load("Assets/animData/Enemy/Attack.tka");
		m_animationClips[enAnimClip_Attack].SetLoopFlag(false);	//ループモーション

		m_animationClips[enAnimClip_Damage].Load("Assets/animData/Enemy/HitDamage.tka");
		m_animationClips[enAnimClip_Damage].SetLoopFlag(false);	//ループモーション
		m_animationClips[enAnimClip_KnockBack].Load("Assets/animData/Enemy/KnockBack.tka");
		m_animationClips[enAnimClip_KnockBack].SetLoopFlag(false);
		m_animationClips[enAnimClip_StandUp].Load("Assets/animData/Enemy/StandUp.tka");
		m_animationClips[enAnimClip_StandUp].SetLoopFlag(false);
		m_animationClips[enAnimClip_Die].Load("Assets/animData/Enemy/Dying.tka");
		m_animationClips[enAnimClip_Die].SetLoopFlag(false);	//ループモーション
		return true;
	}
	void EnemyAnimation::Update()
	{
		AnimationTrans();
		AnimationSelect();
	}
	//アニメーションを再生
	void EnemyAnimation::AnimationSelect()
	{
		float interpolateTime = 0.4f;//アニメーション間の補間
		auto onPostCalcLocalMatrix = [&](int boneNo, Bone* bone)
		{
			for (int i = 0; i < m_parent->GetSkinModelRender()->GetSkeleton().GetNumBones(); i++)
			{
				if (boneNo == i) {
					Matrix MatScale;
					MatScale.MakeScaling(bone->GetScale());//行列内の拡大率を設定する
					Matrix localMatrix = bone->GetLocalMatrix();//ローカル行列のデータをコピー
					localMatrix.Multiply(MatScale, localMatrix);//行列に拡大率をかける
					bone->SetLocalMatrix(localMatrix);//ローカル行列を上書きする				
				}
			}
		};
		switch (m_animStatus)
		{
		case nsMuscle::EnemyAnimation::enStatus_Idle:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Idle, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::EnemyAnimation::enStatus_Walk:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Walk, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::EnemyAnimation::enStatus_Run:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Run, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::EnemyAnimation::enStatus_Attack:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Attack, interpolateTime, onPostCalcLocalMatrix);
			m_attackTimer++;
			if (m_attackTimer == ENEMY_ATTACK_TIMING) {//アニメーションが終了したらタイマーを0に戻す
				m_parent->SetIsPlayerDamage(true);
			}
			break;
		case nsMuscle::EnemyAnimation::enStatus_Damage:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Damage,m_parent->GetIsRePlay(), interpolateTime, onPostCalcLocalMatrix);
			m_parent->SetIsRePlay(false);
			break;
		case nsMuscle::EnemyAnimation::enStatus_KnockBack:			
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_KnockBack,1.0f, onPostCalcLocalMatrix);		
			
			break;
		case nsMuscle::EnemyAnimation::enStatus_StandUp:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_StandUp, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::EnemyAnimation::enStatus_Die:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Die, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::EnemyAnimation::enStatus_Num:
			break;
		default:
			break;
		}
	}
	//アニメーション遷移
	void EnemyAnimation::AnimationTrans()
	{
		switch (m_animStatus)
		{
		case nsMuscle::EnemyAnimation::enStatus_Idle:
			TryChangeStatusWalk();
			TryChangeStatusAttack();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			TryChangeStatusKnockBack();
			break;
		case nsMuscle::EnemyAnimation::enStatus_Walk:
			TryChangeStatusIdle();
			//TryChangeStatusRun();
			TryChangeStatusAttack();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			TryChangeStatusKnockBack();
			break;
		case nsMuscle::EnemyAnimation::enStatus_Run:
			TryChangeStatusWalk();
			TryChangeStatusIdle();
			TryChangeStatusAttack();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			TryChangeStatusKnockBack();
			break;
		case nsMuscle::EnemyAnimation::enStatus_Attack:
			TryChangeStatusAttack();
			TryChangeStatusDamage();	
			TryChangeStatusKnockBack();
			break;
		case nsMuscle::EnemyAnimation::enStatus_Damage:
			TryChangeStatusDamage();
			TryChangeStatusKnockBack();
			break;
		case  nsMuscle::EnemyAnimation::enStatus_KnockBack:
			TryChangeStatusKnockBack();
			break;
		case nsMuscle::EnemyAnimation::enStatus_StandUp:
			TryChangeStatusStandUp();
			break;
		case nsMuscle::EnemyAnimation::enStatus_Die:
			TryChangeStatusDie();
			break;
		case nsMuscle::EnemyAnimation::enStatus_Num:
			break;
		default:
			break;
		}
	}
	//待機状態に遷移する
	void EnemyAnimation::TryChangeStatusIdle()
	{
		if (m_parent->GetMoveSpeed().Length() < 0.001f)
		{
			m_animStatus = nsMuscle::EnemyAnimation::enStatus_Idle;
		}
	}
	//歩き状態に遷移する
	void EnemyAnimation::TryChangeStatusWalk()
	{
		if (m_parent->GetMoveSpeed().Length() > 0.f&& m_parent->GetStatus()== Enemy::EnEnemyStatus::enEnemyStatus_Move)
		{
			m_animStatus = nsMuscle::EnemyAnimation::enStatus_Walk;
		}
	}
	void EnemyAnimation::TryChangeStatusRun()
	{
		if (m_parent->GetMoveSpeed().Length() > 0.5f)
		{
			m_animStatus = nsMuscle::EnemyAnimation::enStatus_Run;
		}
	}
	void EnemyAnimation::TryChangeStatusAttack()
	{
		if (m_parent->GetIsAttack())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.5f);
			m_animStatus = nsMuscle::EnemyAnimation::enStatus_Attack;
		}
		if (m_animStatus == EnemyAnimation::enStatus_Attack && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = EnemyAnimation::enStatus_Idle;
			m_attackTimer = 0;
			//m_parent->ReSetCoolTime();
			m_parent->SetStatus(Enemy::EnEnemyStatus::enEnemyStatus_Move);
			m_parent->SetIsAttack(false);
		}
	}
	void EnemyAnimation::TryChangeStatusDie()
	{
		if (m_parent->GetHP() <= 0.f)
		{
			m_animStatus= nsMuscle::EnemyAnimation::enStatus_Die;
		}
	}
	void EnemyAnimation::TryChangeStatusDamage()
	{
		if (m_parent->GetIsDamage())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(5.0f);
			m_animStatus= EnemyAnimation::enStatus_Damage;
		}
		if (m_animStatus == EnemyAnimation::enStatus_Damage && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->SetIsDamage(false);
			m_animStatus = EnemyAnimation::enStatus_Idle;
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_parent->SetStatus(Enemy::EnEnemyStatus::enEnemyStatus_Move);
		}
	}
	void EnemyAnimation::TryChangeStatusKnockBack()
	{
		if (m_parent->GetIsKnockBack())
		{
			m_knockBackTimer++;//音を再生させるタイミングのためのタイマー
			m_parent->GetSkinModelRender()->SetAnimationSpeed(5.0f);
			m_animStatus = EnemyAnimation::enStatus_KnockBack;
		}
		else if (m_animStatus == EnemyAnimation::enStatus_KnockBack && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->SetIsDamage(false);	
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			//if (m_animStatus == EnemyAnimation::enStatus_KnockBack){
				if (m_parent->GetHP() < 0.f)//HPが０だったらそのまま死
				{
				}
				else {
					m_animStatus = EnemyAnimation::enStatus_StandUp;	
					m_timer = 0.0f;//ノックバックアニメーションの遅延につかったタイマーをリセット
				}
			//}
		}
	}
	void EnemyAnimation::TryChangeStatusStandUp()
	{
		if (m_animStatus == EnemyAnimation::enStatus_KnockBack && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_animStatus = EnemyAnimation::enStatus_StandUp;
		}
		else if (m_animStatus == EnemyAnimation::enStatus_StandUp && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_animStatus = EnemyAnimation::enStatus_Idle;
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
		}
	}
}