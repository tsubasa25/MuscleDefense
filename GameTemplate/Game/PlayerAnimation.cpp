#include "stdafx.h"
#include "PlayerAnimation.h"
#include "Player.h"
namespace nsMuscle {
	namespace {
		const float PUNCH_SPEED = 1.5f;
		const float UPPER_SPEED = 1.0f;
	}
	PlayerAnimation::~PlayerAnimation()
	{

	}
	bool PlayerAnimation::Start()
	{
		m_animationClips[enAnimClip_Idle].Load("Assets/animData/Player/test/Idle.tka");
		m_animationClips[enAnimClip_Idle].SetLoopFlag(true);	//���[�v���[�V����
		m_animationClips[enAnimClip_Walk].Load("Assets/animData/Player/test/Walk.tka");
		m_animationClips[enAnimClip_Walk].SetLoopFlag(true);	//���[�v���[�V����
		m_animationClips[enAnimClip_Run].Load("Assets/animData/Player/test/Run.tka");
		m_animationClips[enAnimClip_Run].SetLoopFlag(true);	//���[�v���[�V����
		m_animationClips[enAnimClip_Punchi0].Load("Assets/animData/Player/test/Punchi0.tka");
		m_animationClips[enAnimClip_Punchi0].SetLoopFlag(false);	//���[�v���[�V����
		m_animationClips[enAnimClip_Punchi1].Load("Assets/animData/Player/test/Punchi1.tka");
		m_animationClips[enAnimClip_Punchi1].SetLoopFlag(false);	//���[�v���[�V����
		m_animationClips[enAnimClip_Punchi2].Load("Assets/animData/Player/test/Punchi2.tka");
		m_animationClips[enAnimClip_Punchi2].SetLoopFlag(false);	//���[�v���[�V����
		//m_animationClips[enAnimClip_Kick0].Load("Assets/animData/Player/Kick0.tka");
		//m_animationClips[enAnimClip_Kick0].SetLoopFlag(false);	//���[�v���[�V����
		//m_animationClips[enAnimClip_Kick1].Load("Assets/animData/Player/Kick1.tka");
		//m_animationClips[enAnimClip_Kick1].SetLoopFlag(false);	//���[�v���[�V����
		//m_animationClips[enAnimClip_Kick2].Load("Assets/animData/Player/Kick2.tka");
		//m_animationClips[enAnimClip_Kick2].SetLoopFlag(false);	//���[�v���[�V����
		//m_animationClips[enAnimClip_JumpAttack].Load("Assets/animData/Player/test/Hadoken.tka");
		//m_animationClips[enAnimClip_JumpAttack].SetLoopFlag(false);	//���[�v���[�V����
		m_animationClips[enAnimClip_Damage].Load("Assets/animData/Player/test/HitDamage.tka");
		m_animationClips[enAnimClip_Damage].SetLoopFlag(false);	//���[�v���[�V����
		m_animationClips[enAnimClip_Die].Load("Assets/animData/Player/test/Die.tka");
		m_animationClips[enAnimClip_Die].SetLoopFlag(false);	//���[�v���[�V����

		m_animationClips[enAnimClip_TrainingIdle].Load("Assets/animData/Player/training/Idle.tka");
		m_animationClips[enAnimClip_TrainingIdle].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimClip_PushUp].Load("Assets/animData/Player/training/PushUp.tka");//PushUp
		m_animationClips[enAnimClip_PushUp].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimClip_SitUp].Load("Assets/animData/Player/training/Situps.tka");
		m_animationClips[enAnimClip_SitUp].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimClip_Squat].Load("Assets/animData/Player/training/Squat.tka");
		m_animationClips[enAnimClip_Squat].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimClip_LevelUp].Load("Assets/animData/Player/training/LevelUp.tka");
		m_animationClips[enAnimClip_LevelUp].SetLoopFlag(false);	//���[�v���[�V�����ɂ���B

		m_animationClips[enAnimClip_Lose].Load("Assets/animData/Player/result/pose.tka");
		m_animationClips[enAnimClip_Lose].SetLoopFlag(false);
		m_animationClips[enAnimClip_Clear0].Load("Assets/animData/Player/result/pose0.tka");
		m_animationClips[enAnimClip_Clear0].SetLoopFlag(false);
		m_animationClips[enAnimClip_Clear1].Load("Assets/animData/Player/result/pose1.tka");
		m_animationClips[enAnimClip_Clear1].SetLoopFlag(false);
		m_animationClips[enAnimClip_Clear2].Load("Assets/animData/Player/result/pose2.tka");
		m_animationClips[enAnimClip_Clear2].SetLoopFlag(false);
		m_animationClips[enAnimClip_Clear3].Load("Assets/animData/Player/result/pose3.tka");
		m_animationClips[enAnimClip_Clear3].SetLoopFlag(false);
		return true;
	}
	void PlayerAnimation::Update()
	{
		AnimationTrans();
		AnimationSelect();
	}
	auto PlayerAnimation::PartScale()//���̖��O�̊g�嗦��ݒ肷��
	{
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
		return onPostCalcLocalMatrix;
	}
	//�A�j���[�V�������Đ�
	void PlayerAnimation::AnimationSelect()
	{		
		float interpolateTime = 0.4f;//�A�j���[�V�����Ԃ̕��
		
		auto onPostCalcLocalMatrix = PartScale();
		switch (m_animStatus)
		{
		case nsMuscle::PlayerAnimation::enStatus_Idle:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Idle, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Walk:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Walk,0.1f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Run:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Run,interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi0:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Punchi0, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi1:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Punchi1, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi2:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Punchi2, interpolateTime, onPostCalcLocalMatrix);
			break;
		/*case nsMuscle::PlayerAnimation::enStatus_Kick0:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Kick0, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Kick1:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Kick1, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Kick2:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Kick2, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_JumpAttack:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_JumpAttack, interpolateTime, onPostCalcLocalMatrix);
			break;*/
		case nsMuscle::PlayerAnimation::enStatus_Damage:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Damage, m_parent->GetIsRePlay(), interpolateTime, onPostCalcLocalMatrix);
			m_parent->SetIsRePlay(false);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Die:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Die, interpolateTime, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_TrainingIdle:
			m_parent->GetSkinModelRender()->PlayAnimation(enStatus_TrainingIdle, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_PushUp:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_PushUp, 0.5f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_SitUp:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_SitUp, 0.5f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Squat:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Squat, 0.5f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_LevelUp:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_LevelUp, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Lose:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Lose, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Clear0:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Clear0, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Clear1:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Clear1, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Clear2:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Clear2, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Clear3:
			m_parent->GetSkinModelRender()->PlayAnimation(enAnimClip_Clear3, 0.0f, onPostCalcLocalMatrix);
			break;
		case nsMuscle::PlayerAnimation::enStatus_Num:
			break;
		default:
			break;
		}
	}
	//�A�j���[�V�����J��
	void PlayerAnimation::AnimationTrans()
	{
		if (m_parent->GetPlayerStatus() == Player::EnPlayerStatus::enPlayerStatus_War && m_parent->GetIsOnce()==true) {
			m_parent->SetIsOnce(false);
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Idle;
		}
		else if(m_parent->GetPlayerStatus() == Player::EnPlayerStatus::enPlayerStatus_Training && m_parent->GetIsOnce() == true)
		{
			m_parent->SetIsOnce(false);
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_TrainingIdle;
		}
		switch (m_animStatus)
		{
		case nsMuscle::PlayerAnimation::enStatus_Idle:
			TryChangeStatusRun();//������ԂɑJ�ڂł���
			TryChangeStatusPunchi0();
			
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Run:
			TryChangeStatusIdle();//�ҋ@��ԂɑJ�ڂł���			
			TryChangeStatusPunchi0();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;		
		case nsMuscle::PlayerAnimation::enStatus_Punchi0:						
			//�U����ԂɑJ�ڂł���
			TryChangeStatusPunchi0();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi1:
			TryChangeStatusPunchi1();//�U��1��ԂɑJ�ڂł���
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi2:
			TryChangeStatusPunchi2();//�U��1��ԂɑJ�ڂł���
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		//case nsMuscle::PlayerAnimation::enStatus_Kick0:
		//	//�U����ԂɑJ�ڂł���
		//	TryChangeStatusKick0();
		//	TryChangeStatusDamage();
		//	TryChangeStatusDie();
		//	break;
		//case nsMuscle::PlayerAnimation::enStatus_Kick1:
		//	TryChangeStatusKick1();//�U��1��ԂɑJ�ڂł���
		//	TryChangeStatusDamage();
		//	TryChangeStatusDie();
		//	break;
		//case nsMuscle::PlayerAnimation::enStatus_Kick2:
		//	TryChangeStatusKick2();//�U��1��ԂɑJ�ڂł���		
		//	TryChangeStatusDamage();
		//	TryChangeStatusDie();
		//	break;
		//case nsMuscle::PlayerAnimation::enStatus_JumpAttack:
		//	TryChangeStatusJumpAttack();
		//	TryChangeStatusDamage();
		//	TryChangeStatusDie();
		//	break;
		case nsMuscle::PlayerAnimation::enStatus_Damage:
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Die:
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_TrainingIdle:
			TryChangeStatusTrainingIdle();
			TryChangeStatusPushUp();//�r���ĕ�����ԂɑJ�ڂł���
			TryChangeStatusSitUp();
			TryChangeStatusSquat();
			TryChangeStatusLevelUp();
			break;
		case nsMuscle::PlayerAnimation::enStatus_PushUp:
			TryChangeStatusTrainingIdle();
			TryChangeStatusPushUp();
			TryChangeStatusSitUp();
			TryChangeStatusSquat();
			TryChangeStatusLevelUp();
			break;
		case nsMuscle::PlayerAnimation::enStatus_SitUp :
			TryChangeStatusTrainingIdle();
			TryChangeStatusPushUp();
			TryChangeStatusSitUp();
			TryChangeStatusSquat();
			TryChangeStatusLevelUp();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Squat:
			TryChangeStatusTrainingIdle();
			TryChangeStatusPushUp();
			TryChangeStatusSitUp();
			TryChangeStatusSquat();
			TryChangeStatusLevelUp();
			break;
		case nsMuscle::PlayerAnimation::enStatus_LevelUp:
			TryChangeStatusTrainingIdle();
			TryChangeStatusPushUp();
			TryChangeStatusSitUp();
			TryChangeStatusSquat();
			TryChangeStatusLevelUp();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Lose:
			
			break;
		default:
			break;
		}
	}


	//�ҋ@��ԂɑJ�ڂ���
	void PlayerAnimation::TryChangeStatusIdle()
	{
		if (m_parent->GetMoveSpeed().Length() <= 0.5f)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Idle;
		}
	}
	//������ԂɑJ�ڂ���
	void PlayerAnimation::TryChangeStatusRun()
	{
		if (m_parent->GetMoveSpeed().Length() > 0.5f)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Run;
		}
	}
	
	//�U����ԂɑJ�ڂ���
	void PlayerAnimation::TryChangeStatusPunchi0()
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			if (m_parent->GetComboNum() == 0)
			{
				m_parent->GetSkinModelRender()->SetAnimationSpeed(PUNCH_SPEED);
				m_animStatus = PlayerAnimation::enStatus_Punchi0;
			}
		}
		if (m_parent->GetComboNum() >= 1 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_animStatus = PlayerAnimation::enStatus_Punchi1;
			m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
		}
		else if (m_parent->GetComboNum() == 0 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->ReSetComboNum();//�R���{�������Z�b�g
			m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
		}		
	}
	void PlayerAnimation::TryChangeStatusPunchi1()
	{
		if (m_parent->GetComboNum() >= 1)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Punchi1;			
		}
		if (m_parent->GetComboNum() >= 2 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_animStatus = PlayerAnimation::enStatus_Punchi2;
			m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
		}
		else if (m_parent->GetComboNum() == 1 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->ReSetComboNum();//�R���{�������Z�b�g
			m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
		}
	}
	void PlayerAnimation::TryChangeStatusPunchi2()
	{
		if (m_parent->GetComboNum() >= 2)
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(UPPER_SPEED);
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Punchi2;
		}
		//if (m_parent->GetComboNum() >= 3 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		//{
		//	m_animStatus = PlayerAnimation::enStatus_JumpAttack;
		//	m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
		//}
		if (!m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->ReSetComboNum();//�R���{�������Z�b�g
			m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
		}
	}
	//void PlayerAnimation::TryChangeStatusKick0()
	//{
	//	if (g_pad[0]->IsTrigger(enButtonY))
	//	{
	//		if (m_parent->GetComboNum() == 0)
	//		{
	//			m_parent->GetSkinModelRender()->SetAnimationSpeed(2.0f);
	//			m_animStatus = PlayerAnimation::enStatus_Kick0;
	//		}
	//	}
	//	if (m_parent->GetComboNum() >= 1 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_animStatus = PlayerAnimation::enStatus_Kick1;
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//	else if (m_parent->GetComboNum() == 0 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//�R���{�������Z�b�g
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//}
	//void PlayerAnimation::TryChangeStatusKick1()
	//{
	//	if (m_parent->GetComboNum() >= 1)
	//	{
	//		m_animStatus = nsMuscle::PlayerAnimation::enStatus_Kick1;
	//	}
	//	if (m_parent->GetComboNum() >= 2 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_animStatus = PlayerAnimation::enStatus_Kick2;
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//	else if (m_parent->GetComboNum() == 1 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//�R���{�������Z�b�g
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//}
	//void PlayerAnimation::TryChangeStatusKick2()
	//{
	//	if (m_parent->GetComboNum() >= 2)
	//	{
	//		m_animStatus = nsMuscle::PlayerAnimation::enStatus_Kick2;
	//	}
	//	if (m_parent->GetComboNum() >= 3 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_animStatus = PlayerAnimation::enStatus_JumpAttack;
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//	else if (!m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//�R���{�������Z�b�g
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//}
	//void PlayerAnimation::TryChangeStatusJumpAttack()
	//{
	//	if (m_parent->GetComboNum() >= 3)
	//	{
	//		m_animStatus = nsMuscle::PlayerAnimation::enStatus_JumpAttack;
	//	}
	//	if (!m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//�R���{�������Z�b�g
	//		m_parent->ReSetIsAttackOn();//�G�Ƀ_���[�W��^������悤�ɂ���
	//	}
	//}
	void PlayerAnimation::TryChangeStatusDamage()
	{
		if (m_parent->GetIsDamage())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(2.0f* m_parent->GetAbdominalLevel());//���؃��x���ɂ���ă_���[�W�A�j���[�V�����̃X�s�[�h�𒲐�
			m_animStatus = PlayerAnimation::enStatus_Damage;
		}
		if (m_animStatus == PlayerAnimation::enStatus_Damage && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{	
			m_parent->ReSetComboNum();//�R���{�������Z�b�g	
			m_parent->SetIsDamage(false);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
		}
	}
	void PlayerAnimation::TryChangeStatusDie()
	{
		if (m_parent->GetHP() <= 0.0f)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Die;
		}
	}
	//�g���[�j���O���ҋ@��ԂɑJ�ڂ���
	void PlayerAnimation::TryChangeStatusTrainingIdle()
	{
		if (m_parent->GetPlayerStatus() == Player::EnPlayerStatus::enPlayerStatus_Training&&m_parent->GetTrainingStatus() == Player::EnTrainingStatus::enTrainingStatus_Idle)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_TrainingIdle;
		}
	}
	//�r���ĕ�����ԂɑJ�ڂ���	
	void PlayerAnimation::TryChangeStatusPushUp()
	{
		if (m_parent->GetTrainingStatus()==Player::EnTrainingStatus::enTrainingStatus_PushUp)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_PushUp;
		}
	}
	//��̋N������Ԃɂ���
	void PlayerAnimation::TryChangeStatusSitUp()
	{
		if (m_parent->GetTrainingStatus() == Player::EnTrainingStatus::enTrainingStatus_SitUp)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_SitUp;
		}
	}
	//�X�N���b�g��Ԃɂ���
	void PlayerAnimation::TryChangeStatusSquat()
	{
		if (m_parent->GetTrainingStatus() == Player::EnTrainingStatus::enTrainingStatus_Squat)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Squat;
		}
	}
	void PlayerAnimation::TryChangeStatusLevelUp()
	{
		if (m_parent->GetLevelUpOnce())
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_LevelUp;
		}
		if (!m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->SetLevelUpOnce(false);
		}
	}
}