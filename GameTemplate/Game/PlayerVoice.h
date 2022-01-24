#pragma once
namespace nsMuscle {
	class PlayerAnimation;
	class PlayerVoice:public IGameObject
	{
	public:
		~PlayerVoice();
		bool Start();
		void Update();
		void SetParent(PlayerAnimation& data) { m_playerAnimation = &data; }
		void SetIsVoice(bool flag) { m_isVoice = flag; }
		void RunVoice();
		void Punch0Voice();
		void Punch1Voice();
		void UpperVoice();
		void DamageVoice();		
		void DieVoice();
		void NotRunVoice();
		void NotPunch0Voice();
		void NotPunch1Voice();
		void NotUpperVoice();
		void NotDamageVoice();		
		void NotDieVoice();
	private:
		PlayerAnimation* m_playerAnimation = nullptr;

		SoundSource* m_ssRun = nullptr;
		bool m_isRunCreate = false;
		SoundSource* m_ssPunch0 = nullptr;
		bool m_isPunch0Create = false;
		SoundSource* m_ssPunch1 = nullptr;
		bool m_isPunch1Create = false;
		SoundSource* m_ssUpper = nullptr;
		bool m_isUpperCreate = false;
		SoundSource* m_ssDamage = nullptr;
		bool m_isDamageCreate = false;		
		SoundSource* m_ssDie = nullptr;
		bool m_isDieCreate = false;

		bool m_isVoice = true;

		int m_upperTimer = 0;
		
	};

}