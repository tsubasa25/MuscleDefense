#pragma once
namespace nsMuscle {
	/// <summary>
	/// ジムの周りに待機して、敵が来ると、自動的に攻撃してくれる
	/// </summary>
	class Minion :public IGameObject
	{
	public :
		enum EnMinionStatus
		{
			enMinionStatus_Idle,//待機
			enMinionStatus_Move,//移動
			enMinionStatus_Attack,//攻撃
		};
	private:
		EnMinionStatus m_enMinionStatus = enMinionStatus_Idle;
	public:
		~Minion();
		bool Start();
		void Update();
		void MinionMove();
		void MinionAttack();
		void EnemyAnyDamage(float damage);//敵にダメージを与える
		
	private:
		float m_hp = 0.0f;
		bool m_isDamage = false;
		int m_playerNum = 0;
		bool m_isRePlay = false;//アニメーションを最初から再生させる

		SkinModelRender* m_skinModelRender = nullptr;//スキン
		Vector3 m_moveSpeed = Vector3::Zero;
		Vector3 m_scale = { 1.0f,1.0f, 1.0f };
		Vector3 m_position = Vector3::Zero;//位置
		CharacterController m_charaCon;//物理カプセル

		//PlayerAnimation* m_playerAnimation = nullptr;//アニメーションクラス

	};
}