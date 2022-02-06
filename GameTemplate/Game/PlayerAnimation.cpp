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
		m_animationClips[enAnimClip_Idle].SetLoopFlag(true);	//ループモーション
		m_animationClips[enAnimClip_Walk].Load("Assets/animData/Player/test/Walk.tka");
		m_animationClips[enAnimClip_Walk].SetLoopFlag(true);	//ループモーション
		m_animationClips[enAnimClip_Run].Load("Assets/animData/Player/test/Run.tka");
		m_animationClips[enAnimClip_Run].SetLoopFlag(true);	//ループモーション
		m_animationClips[enAnimClip_Punchi0].Load("Assets/animData/Player/test/Punchi0.tka");
		m_animationClips[enAnimClip_Punchi0].SetLoopFlag(false);	//ループモーション
		m_animationClips[enAnimClip_Punchi1].Load("Assets/animData/Player/test/Punchi1.tka");
		m_animationClips[enAnimClip_Punchi1].SetLoopFlag(false);	//ループモーション
		m_animationClips[enAnimClip_Punchi2].Load("Assets/animData/Player/test/Punchi2.tka");
		m_animationClips[enAnimClip_Punchi2].SetLoopFlag(false);	//ループモーション
		//m_animationClips[enAnimClip_Kick0].Load("Assets/animData/Player/Kick0.tka");
		//m_animationClips[enAnimClip_Kick0].SetLoopFlag(false);	//ループモーション
		//m_animationClips[enAnimClip_Kick1].Load("Assets/animData/Player/Kick1.tka");
		//m_animationClips[enAnimClip_Kick1].SetLoopFlag(false);	//ループモーション
		//m_animationClips[enAnimClip_Kick2].Load("Assets/animData/Player/Kick2.tka");
		//m_animationClips[enAnimClip_Kick2].SetLoopFlag(false);	//ループモーション
		//m_animationClips[enAnimClip_JumpAttack].Load("Assets/animData/Player/test/Hadoken.tka");
		//m_animationClips[enAnimClip_JumpAttack].SetLoopFlag(false);	//ループモーション
		m_animationClips[enAnimClip_Damage].Load("Assets/animData/Player/test/HitDamage.tka");
		m_animationClips[enAnimClip_Damage].SetLoopFlag(false);	//ループモーション
		m_animationClips[enAnimClip_Die].Load("Assets/animData/Player/test/Die.tka");
		m_animationClips[enAnimClip_Die].SetLoopFlag(false);	//ループモーション

		m_animationClips[enAnimClip_TrainingIdle].Load("Assets/animData/Player/training/Idle.tka");
		m_animationClips[enAnimClip_TrainingIdle].SetLoopFlag(false);	//ループモーションにする。
		m_animationClips[enAnimClip_PushUp].Load("Assets/animData/Player/training/PushUp.tka");//PushUp
		m_animationClips[enAnimClip_PushUp].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimClip_SitUp].Load("Assets/animData/Player/training/Situps.tka");
		m_animationClips[enAnimClip_SitUp].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimClip_Squat].Load("Assets/animData/Player/training/Squat.tka");
		m_animationClips[enAnimClip_Squat].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimClip_LevelUp].Load("Assets/animData/Player/training/LevelUp.tka");
		m_animationClips[enAnimClip_LevelUp].SetLoopFlag(false);	//ループモーションにする。

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
	auto PlayerAnimation::PartScale()//骨の名前の拡大率を設定する
	{
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
		return onPostCalcLocalMatrix;
	}
	//アニメーションを再生
	void PlayerAnimation::AnimationSelect()
	{		
		float interpolateTime = 0.4f;//アニメーション間の補間
		
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
	//アニメーション遷移
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
			TryChangeStatusRun();//歩き状態に遷移できる
			TryChangeStatusPunchi0();
			
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Run:
			TryChangeStatusIdle();//待機状態に遷移できる			
			TryChangeStatusPunchi0();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;		
		case nsMuscle::PlayerAnimation::enStatus_Punchi0:						
			//攻撃状態に遷移できる
			TryChangeStatusPunchi0();
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi1:
			TryChangeStatusPunchi1();//攻撃1状態に遷移できる
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		case nsMuscle::PlayerAnimation::enStatus_Punchi2:
			TryChangeStatusPunchi2();//攻撃1状態に遷移できる
			TryChangeStatusDamage();
			TryChangeStatusDie();
			break;
		//case nsMuscle::PlayerAnimation::enStatus_Kick0:
		//	//攻撃状態に遷移できる
		//	TryChangeStatusKick0();
		//	TryChangeStatusDamage();
		//	TryChangeStatusDie();
		//	break;
		//case nsMuscle::PlayerAnimation::enStatus_Kick1:
		//	TryChangeStatusKick1();//攻撃1状態に遷移できる
		//	TryChangeStatusDamage();
		//	TryChangeStatusDie();
		//	break;
		//case nsMuscle::PlayerAnimation::enStatus_Kick2:
		//	TryChangeStatusKick2();//攻撃1状態に遷移できる		
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
			TryChangeStatusPushUp();//腕立て伏せ状態に遷移できる
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


	//待機状態に遷移する
	void PlayerAnimation::TryChangeStatusIdle()
	{
		if (m_parent->GetMoveSpeed().Length() <= 0.5f)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Idle;
		}
	}
	//歩き状態に遷移する
	void PlayerAnimation::TryChangeStatusRun()
	{
		if (m_parent->GetMoveSpeed().Length() > 0.5f)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_Run;
		}
	}
	
	//攻撃状態に遷移する
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
			m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
		}
		else if (m_parent->GetComboNum() == 0 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->ReSetComboNum();//コンボ数をリセット
			m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
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
			m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
		}
		else if (m_parent->GetComboNum() == 1 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->ReSetComboNum();//コンボ数をリセット
			m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
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
		//	m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
		//}
		if (!m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
			m_animStatus = PlayerAnimation::enStatus_Idle;
			m_parent->ReSetComboNum();//コンボ数をリセット
			m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
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
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
	//	}
	//	else if (m_parent->GetComboNum() == 0 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//コンボ数をリセット
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
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
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
	//	}
	//	else if (m_parent->GetComboNum() == 1 && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//コンボ数をリセット
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
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
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
	//	}
	//	else if (!m_parent->GetSkinModelRender()->IsPlayingAnimation())
	//	{
	//		m_parent->GetSkinModelRender()->SetAnimationSpeed(1.0f);
	//		m_animStatus = PlayerAnimation::enStatus_Idle;
	//		m_parent->ReSetComboNum();//コンボ数をリセット
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
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
	//		m_parent->ReSetComboNum();//コンボ数をリセット
	//		m_parent->ReSetIsAttackOn();//敵にダメージを与えられるようにする
	//	}
	//}
	void PlayerAnimation::TryChangeStatusDamage()
	{
		if (m_parent->GetIsDamage())
		{
			m_parent->GetSkinModelRender()->SetAnimationSpeed(2.0f* m_parent->GetAbdominalLevel());//腹筋レベルによってダメージアニメーションのスピードを調整
			m_animStatus = PlayerAnimation::enStatus_Damage;
		}
		if (m_animStatus == PlayerAnimation::enStatus_Damage && !m_parent->GetSkinModelRender()->IsPlayingAnimation())
		{	
			m_parent->ReSetComboNum();//コンボ数をリセット	
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
	//トレーニング時待機状態に遷移する
	void PlayerAnimation::TryChangeStatusTrainingIdle()
	{
		if (m_parent->GetPlayerStatus() == Player::EnPlayerStatus::enPlayerStatus_Training&&m_parent->GetTrainingStatus() == Player::EnTrainingStatus::enTrainingStatus_Idle)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_TrainingIdle;
		}
	}
	//腕立て伏せ状態に遷移する	
	void PlayerAnimation::TryChangeStatusPushUp()
	{
		if (m_parent->GetTrainingStatus()==Player::EnTrainingStatus::enTrainingStatus_PushUp)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_PushUp;
		}
	}
	//上体起こし状態にする
	void PlayerAnimation::TryChangeStatusSitUp()
	{
		if (m_parent->GetTrainingStatus() == Player::EnTrainingStatus::enTrainingStatus_SitUp)
		{
			m_animStatus = nsMuscle::PlayerAnimation::enStatus_SitUp;
		}
	}
	//スクワット状態にする
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