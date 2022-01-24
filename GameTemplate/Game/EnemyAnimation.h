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
		//状態の数
		enum EnEnemyAnimStatus {
			enStatus_Idle,
			enStatus_Walk,
			enStatus_Run,
			enStatus_Attack,
			enStatus_Damage,
			enStatus_KnockBack,
			enStatus_StandUp,
			enStatus_Die,
			enStatus_Num			//状態の数。
		};	
		~EnemyAnimation();
		bool Start();
		void Update();
		//アニメーションを再生
		void AnimationSelect();
		//アニメーション遷移
		void AnimationTrans();

		AnimationClip* GetAnimationClips() { return m_animationClips; }
		int GetAnimationStatus() { return m_animStatus; }
		int GetAnimationClipNum() { return enAnimClip_num; }
		void SetParent(Enemy& enemy) { m_parent = &enemy; }
		void SetEnemyVoice(EnemyVoice& enemyVoice) { m_enemyVoice = &enemyVoice; }

		//待機状態に遷移する
		void TryChangeStatusIdle();
		//歩き状態に遷移する
		void TryChangeStatusWalk();
		//走り状態に遷移する
		void TryChangeStatusRun();
		//攻撃状態に遷移する
		void TryChangeStatusAttack();
		//ダメージヒット状態に遷移する
		void TryChangeStatusDamage();
		//ノックバック状態に遷移する
		void TryChangeStatusKnockBack();
		//立ち上がり状態に遷移する
		void TryChangeStatusStandUp();
		//死亡状態に遷移する
		void TryChangeStatusDie();

		int GetEnStatusDie() { return EnEnemyAnimStatus::enStatus_Die; }
		int GetEnStatusKnockBack() { return EnEnemyAnimStatus::enStatus_KnockBack; }
		int GetEnStatusStandUp() { return EnEnemyAnimStatus::enStatus_StandUp; }		
		
		int GetKnockBackTimer() { return m_knockBackTimer; }
	private:
		EnemyVoice* m_enemyVoice = nullptr;

		EnEnemyAnimStatus m_animStatus = enStatus_Idle;
		//アニメーション関連
		AnimationClip m_animationClips[enAnimClip_num];			//アニメーションクリップ

		Enemy* m_parent = nullptr;
		bool m_isOnce = true;
		int m_timer = 0;
		int m_attackTimer = 0;
		int m_knockBackTimer = 0;
	};
}
