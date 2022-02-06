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
		SkinModelRender*		m_modelRender=nullptr;				//モデルレンダラー。
		Vector3					m_position=Vector3::One;					//座標。
		Vector3					m_scale=Vector3::One;					//大きさ。
		Quaternion				m_rotation=Quaternion::Identity;					//回転。
		PhysicsStaticObject		m_physicsStaticObject;		//静的物理オブジェクト。
		int m_goldGymSpawnNum = 0;
		int m_goldGymHP = 1000;
		int m_goldGymOldHP = 1000;
		int m_numberSameCount = 0;//数が同じであればインクリメント
		bool m_isBreak = false;//体力が０になったらtrue
		BackGround* m_backGround = nullptr;
		WipeCamera*m_wipeCamera=nullptr;
		HUD* m_hud = nullptr;
		int m_livePlayerNum = 0;//ジムに入るプレイヤーの番号
		bool m_isLivePlayer = true;//ジムの中にプレイヤーが居るかどうか
	};
}

