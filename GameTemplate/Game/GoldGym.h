#pragma once
namespace nsMuscle {
	class BackGround;
	class WipeCamera;
	class HUD;
	class GoldGym : public IGameObject
	{
	public:	
		~GoldGym();
		bool Start();
		void Update();

		void SetPosition(Vector3 pos) { m_position = pos; }
		Vector3 GetPosition() { return m_position; }
		void SetScale(Vector3 scale) { m_scale = scale; }
		void SetRotation(Quaternion rot) { m_rotation = rot; }

		void SetGoldGymSpawnNum(int num) { m_goldGymSpawnNum = num; }
		int GetGoldGymSpawnNum() { return m_goldGymSpawnNum; }
		void AnyDamage(int num);
		bool GetIsBreak() { return m_isBreak; }

		void SetLivePlayerNum(int num) { m_livePlayerNum = num; }
		int GetLivePlayerNum() { return m_livePlayerNum; }

		void SetIsLivePlayer(bool flag) { m_isLivePlayer = flag; }
		bool GetIsLivePlayer() { return m_isLivePlayer; }

		int GetHP() { return m_goldGymHP; }
	private:
		SkinModelRender*		m_modelRender=nullptr;				//���f�������_���[�B
		Vector3					m_position=Vector3::One;					//���W�B
		Vector3					m_scale=Vector3::One;					//�傫���B
		Quaternion				m_rotation=Quaternion::Identity;					//��]�B
		PhysicsStaticObject		m_physicsStaticObject;		//�ÓI�����I�u�W�F�N�g�B
		int m_goldGymSpawnNum = 0;
		int m_goldGymHP = 1000;
		int m_goldGymOldHP = 1000;
		int m_numberSameCount = 0;//���������ł���΃C���N�������g
		bool m_isBreak = false;//�̗͂��O�ɂȂ�����true
		BackGround* m_backGround = nullptr;
		WipeCamera*m_wipeCamera=nullptr;
		HUD* m_hud = nullptr;
		int m_livePlayerNum = 0;//�W���ɓ���v���C���[�̔ԍ�
		bool m_isLivePlayer = true;//�W���̒��Ƀv���C���[�����邩�ǂ���
	};
}

