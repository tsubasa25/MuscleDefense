#pragma once
namespace nsMuscle {
	class EnemyAnimation;
	class EnemyVoice:public IGameObject
	{
	public:
		~EnemyVoice();
		bool Start();
		void Update();
		void SetParent(EnemyAnimation& data) { m_enemyAnimation = &data; }
		void SetIsVoice(bool flag) { m_isVoice = flag; }

		void RunVoice();
		void PunchVoice();
		void DamageVoice();
		void KnockBackVoice();
		void DieVoice();
		void NotRunVoice();
		void NotAttackVoice();
		void NotDamageVoice();
		void NotKnockBackVoice();
		void NotDieVoice();
	private:
		EnemyAnimation* m_enemyAnimation = nullptr;
		SoundSource* m_ssRun = nullptr;
		bool m_isRunPlay = false;
		bool m_isRunCreate = false;
		SoundSource* m_ssAttack = nullptr;
		bool m_isAttackPlay = false;
		bool m_isAttackCreate = false;
		SoundSource* m_ssDamage = nullptr;
		bool m_isDamagePlay = false;
		bool m_isDamageCreate = false;
		SoundSource* m_ssKnockBack = nullptr;
		bool m_isKnockBackPlay = false;
		bool m_isKnockBackCreate = false;
		SoundSource* m_ssDie = nullptr;
		bool m_isDiePlay = false;
		bool m_isDieCreate = false;

		bool m_isVoice = false;

		int m_attackSETimer = 0;
		//ÇªÇÍÇºÇÍÇÃSEÇ™ìØéûÇ…ó¨ÇÍÇÈç≈ëÂêî
		int m_runSENum = 1;
		int m_attackSENum = 1;
		int m_damageSENum = 2;
		int m_knockBackSENum = 3;
		int m_dieSENum = 1;
	};

}