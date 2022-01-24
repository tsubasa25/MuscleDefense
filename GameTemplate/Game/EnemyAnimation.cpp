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
		m_animationClips[enAnimClip_Idle].SetLoopFlag(true);	//���[�v���[�V����
		m_animationClips[enAnimClip_Walk].Load("Assets/animData/Enemy/Run.tka");
		m_animationClips[enAnimClip_Walk].SetLoopFlag(true);	//���[�v���[�V����
		m_animationClips[enAnimClip_Run].Load("Assets/animData/Enemy/Run.tka");
		m_animationClips[enAnimClip_Run].SetLoopFlag(true);	//���[�v���[�V����
		m_animationClips[enAnimClip_Attack].Load("Assets/animData/Enemy/Attack.tka");
		m_animationClips[enAnimClip_Attack].SetLoopFlag(false);	//���[�v���[�V����

		m_animationClips[enAnimClip_Damage].Load("Assets/animData/Enemy/HitDamage.tka");
		m_animationClips[enAnimClip_Damage].SetLoopFlag(false);	//���[�v���[�V����
		m_animationClips[enAnimClip_KnockBack].Load("Assets/animData/Enemy/KnockBack.tka");
		m_animationClips[enAnimClip_KnockBack].SetLoopFlag(false);
		m_animationClips[enAnimClip_StandUp].Load("Assets/animData/Enemy/StandUp.tka");
		m_animationClips[enAnimClip_StandUp].SetLoopFlag(false);
		m_animationClips[enAnimClip_Die].Load("Assets/animData/Enemy/Dying.tka");
		m_animationClips[enAnimClip_Die].SetLoopFlag(false);	//���[�v���[�V����
		return true;
	}
	void EnemyAnimation::Update()
	{
		AnimationTrans();
		AnimationSelect();
	}
	//�A�j���[�V�������Đ�
	void EnemyAnimation::AnimationSelect()
	{
		float interpolateTime = 0.4f;//�A�j���[�V�����Ԃ̕��
		auto onPostCalcLocalMatrix = [&](int boneNo, Bone* bone)
		{
			for (int i = 0; i < m_parent->GetSkinModelRender()->GetSkeleton().GetNumBones(); i++)
			{
				if (boneNo == i) {
					Matrix MatScale;
					MatScale.MakeScaling(bone->GetScale());//�s����̊g�嗦��ݒ肷��
					Matrix localMatrix = bone->GetLocalMatrix();//���[�J���s��̃f�[�^���R�s�[
					localMatrix.Multiply(MatScale, localMatrix);//�s��Ɋg�嗦��������
					bone->SetLocalMatrix(localMatrix);//���[�J���s����㏑������				
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
			if (m_attackTimer == ENEMY_ATTACK_TIMING) {//�A�j���[�V�������I��������^�C�}�[��0�ɖ߂�
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
	//�A�j���[�V�����J��
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
	//�ҋ@��ԂɑJ�ڂ���
	void EnemyAnimation::TryChangeStatusIdle()
	{
		if (m_parent->GetMoveSpeed().Length() < 0.001f)
		{
			m_animStatus = nsMuscle::EnemyAnimation::enStatus_Idle;
		}
	}
	//������ԂɑJ�ڂ���
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
			m_knockBackTimer++;//�����Đ�������^�C�~���O�̂��߂̃^�C�}�[
			m_parent->GetSkinModelRender()->SetAnimationSpeed(5.0f);
			m_animStatus = EnemyAnimation::enStatus_KnockBack;
		}
		else if (m_animStatus == EnemyAnimation::enStatus_KnockBack && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->SetIsDamage(false);	
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			//if (m_animStatus == EnemyAnimation::enStatus_KnockBack){
				if (m_parent->GetHP() < 0.f)//HP���O�������炻�̂܂܎�
				{
				}
				else {
					m_animStatus = EnemyAnimation::enStatus_StandUp;	
					m_timer = 0.0f;//�m�b�N�o�b�N�A�j���[�V�����̒x���ɂ������^�C�}�[�����Z�b�g
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