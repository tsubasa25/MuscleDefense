#pragma once
namespace nsMuscle {
	/// <summary>
	/// �W���̎���ɑҋ@���āA�G������ƁA�����I�ɍU�����Ă����
	/// </summary>
	class Minion :public IGameObject
	{
	public :
		enum EnMinionStatus
		{
			enMinionStatus_Idle,//�ҋ@
			enMinionStatus_Move,//�ړ�
			enMinionStatus_Attack,//�U��
		};
	private:
		EnMinionStatus m_enMinionStatus = enMinionStatus_Idle;
	public:
		~Minion();
		bool Start();
		void Update();
		void MinionMove();
		void MinionAttack();
		void EnemyAnyDamage(float damage);//�G�Ƀ_���[�W��^����
		
	private:
		float m_hp = 0.0f;
		bool m_isDamage = false;
		int m_playerNum = 0;
		bool m_isRePlay = false;//�A�j���[�V�������ŏ�����Đ�������

		SkinModelRender* m_skinModelRender = nullptr;//�X�L��
		Vector3 m_moveSpeed = Vector3::Zero;
		Vector3 m_scale = { 1.0f,1.0f, 1.0f };
		Vector3 m_position = Vector3::Zero;//�ʒu
		CharacterController m_charaCon;//�����J�v�Z��

		//PlayerAnimation* m_playerAnimation = nullptr;//�A�j���[�V�����N���X

	};
}