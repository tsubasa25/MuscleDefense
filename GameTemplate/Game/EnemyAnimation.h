#pragma once
namespace nsMuscle {
	class Enemy;
	class EnemyVoice;
	class EnemyAnimation :public IGameObject
	{
	private:
		enum {
			enAnimClip_Idle,
			enAnimClip_Walk,
			enAnimClip_Run,
			enAnimClip_Attack,
			enAnimClip_Damage,
			enAnimClip_KnockBack,
			enAnimClip_StandUp,
			enAnimClip_Die,
			enAnimClip_num
		};
	public:
		//��Ԃ̐�
		enum EnEnemyAnimStatus {
			enStatus_Idle,
			enStatus_Walk,
			enStatus_Run,
			enStatus_Attack,
			enStatus_Damage,
			enStatus_KnockBack,
			enStatus_StandUp,
			enStatus_Die,
			enStatus_Num			//��Ԃ̐��B
		};	
		~EnemyAnimation();
		bool Start();
		void Update();
		//�A�j���[�V�������Đ�
		void AnimationSelect();
		//�A�j���[�V�����J��
		void AnimationTrans();

		AnimationClip* GetAnimationClips() { return m_animationClips; }
		int GetAnimationStatus() { return m_animStatus; }
		int GetAnimationClipNum() { return enAnimClip_num; }
		void SetParent(Enemy& enemy) { m_parent = &enemy; }
		void SetEnemyVoice(EnemyVoice& enemyVoice) { m_enemyVoice = &enemyVoice; }

		//�ҋ@��ԂɑJ�ڂ���
		void TryChangeStatusIdle();
		//������ԂɑJ�ڂ���
		void TryChangeStatusWalk();
		//�����ԂɑJ�ڂ���
		void TryChangeStatusRun();
		//�U����ԂɑJ�ڂ���
		void TryChangeStatusAttack();
		//�_���[�W�q�b�g��ԂɑJ�ڂ���
		void TryChangeStatusDamage();
		//�m�b�N�o�b�N��ԂɑJ�ڂ���
		void TryChangeStatusKnockBack();
		//�����オ���ԂɑJ�ڂ���
		void TryChangeStatusStandUp();
		//���S��ԂɑJ�ڂ���
		void TryChangeStatusDie();

		int GetEnStatusDie() { return EnEnemyAnimStatus::enStatus_Die; }
		int GetEnStatusKnockBack() { return EnEnemyAnimStatus::enStatus_KnockBack; }
		int GetEnStatusStandUp() { return EnEnemyAnimStatus::enStatus_StandUp; }		
		
		int GetKnockBackTimer() { return m_knockBackTimer; }
	private:
		EnemyVoice* m_enemyVoice = nullptr;

		EnEnemyAnimStatus m_animStatus = enStatus_Idle;
		//�A�j���[�V�����֘A
		AnimationClip m_animationClips[enAnimClip_num];			//�A�j���[�V�����N���b�v

		Enemy* m_parent = nullptr;
		bool m_isOnce = true;
		int m_timer = 0;
		int m_attackTimer = 0;
		int m_knockBackTimer = 0;
	};
}
