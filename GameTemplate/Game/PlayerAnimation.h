#pragma once
namespace nsMuscle {
	class Player;
	class PlayerAnimation :public IGameObject
	{
	public:
		enum {
			enAnimClip_Idle,
			enAnimClip_Walk,
			enAnimClip_Run,
			enAnimClip_Punchi0,
			enAnimClip_Punchi1,
			enAnimClip_Punchi2,
			enAnimClip_Kick0,
			enAnimClip_Kick1,
			enAnimClip_Kick2,
			enAnimClip_JumpAttack,
			enAnimClip_Damage,
			enAnimClip_Die,
			
			enAnimClip_TrainingIdle,
			enAnimClip_PushUp,
			enAnimClip_SitUp,
			enAnimClip_Squat,
			enAnimClip_LevelUp,
			enAnimClip_num
		};
		//��Ԃ̐�
		enum EnPlayerAnimStatus {
			enStatus_Idle,
			enStatus_Walk,
			enStatus_Run,
			enStatus_Punchi0,
			enStatus_Punchi1,
			enStatus_Punchi2,
			enStatus_Kick0,
			enStatus_Kick1,
			enStatus_Kick2,
			enStatus_JumpAttack,
			enStatus_Damage,
			enStatus_Die,

			enStatus_TrainingIdle,
			enStatus_PushUp,
			enStatus_SitUp,
			enStatus_Squat,
			enStatus_LevelUp,
			enStatus_Num,			//��Ԃ̐��B
		};

	public:
		~PlayerAnimation();
		bool Start();
		void Update();
		auto PartScale();//���̖��O�̊g�嗦��ݒ肷��
		//�A�j���[�V�������Đ�
		void AnimationSelect();
		//�A�j���[�V�����J��
		void AnimationTrans();
		AnimationClip *GetAnimationClips() { return m_animationClips; }
		int GetAnimationStatus() { return m_animStatus; }
		int GetAnimationClipNum() { return enAnimClip_num; }
		//�ҋ@��ԂɑJ�ڂ���
		void TryChangeStatusIdle();		
		//�����ԂɑJ�ڂ���
		void TryChangeStatusRun();
		//�W�����v��ԂɑJ�ڂ���
		void TryChangeStatusJump();
		//���S��ԂɑJ�ڂ���
		void TryChangeStatusDie();
		//�U����ԂɑJ�ڂ���
		void TryChangeStatusPunchi0();
		void TryChangeStatusPunchi1();
		void TryChangeStatusPunchi2();
		void TryChangeStatusKick0();
		void TryChangeStatusKick1();
		void TryChangeStatusKick2();
		void TryChangeStatusJumpAttack();
		
		//��_���[�W��ԂɑJ�ڂ���
		void TryChangeStatusDamage();
		
		//�g���[�j���O���
		void TryChangeStatusTrainingIdle();
		void TryChangeStatusPushUp();
		void TryChangeStatusSitUp();
		void TryChangeStatusSquat();
		void TryChangeStatusLevelUp();
		/// <summary>
		/// �e��ݒ�
		/// </summary>
		/// <param name="player"></param>
		void SetParent(Player& player) { m_parent = &player; }
		int GetEnStatusPushUp() { return EnPlayerAnimStatus::enStatus_PushUp; }
		int GetEnStatusWalk(){return  EnPlayerAnimStatus::enStatus_Walk; }
		int GetEnStatusRun() { return  EnPlayerAnimStatus::enStatus_Run; }
		int GetEnStatusPunchi0(){return EnPlayerAnimStatus::enStatus_Punchi0;}
		int GetEnStatusPunchi1() { return EnPlayerAnimStatus::enStatus_Punchi1; }
		int GetEnStatusPunchi2() { return EnPlayerAnimStatus::enStatus_Punchi2; }
		int GetEnStatusKick0() { return EnPlayerAnimStatus::enStatus_Kick0; }
		int GetEnStatusKick1() { return EnPlayerAnimStatus::enStatus_Kick1; }
		int GetEnStatusKick2() { return EnPlayerAnimStatus::enStatus_Kick2; }
		int GetEnStatusJumpAttack() { return EnPlayerAnimStatus::enStatus_JumpAttack; }
		int GetEnStatusDie() { return EnPlayerAnimStatus::enStatus_Die; }
	private:

		EnPlayerAnimStatus m_animStatus = enStatus_TrainingIdle;
		//�A�j���[�V�����֘A
		AnimationClip m_animationClips[enAnimClip_num];			//�A�j���[�V�����N���b�v

		Player* m_parent = nullptr;
		
		bool m_isCombo = false;//�R���{���q���邩
		
	};
}