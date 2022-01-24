#pragma once
namespace nsMuscle {
	class EnemySEManager
	{
	private:
		EnemySEManager() = default;
		~EnemySEManager() = default;		
	public:
		static EnemySEManager* m_instance;
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new EnemySEManager;
			}
		}

		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}
		static EnemySEManager* GetInstance() { return m_instance; }

		void PlusWalkSENum() { m_walkSENum++; }
		void MinusWalkSENum() { m_walkSENum--; }
		int GetWalkSENum() { return m_walkSENum; }

		void PlusAttackSENum() { m_attackSENum++; }
		void MinusAttackSENum() { m_attackSENum--; }
		int GetAttackSENum() { return m_attackSENum; }

		void PlusDamageSENum() { m_damageSENum++; }
		void MinusDamageSENum() { m_damageSENum--; }
		int GetDamageSENum() { return m_damageSENum; }

		void PlusKnockBackSENum() { m_knockBackSENum++; }
		void MinusKnockBackSENum() { m_knockBackSENum--; }
		int GetKnockBackSENum() { return m_knockBackSENum; }

		void PlusDieSENum() { m_dieSENum++; }
		void MinusDieSENum() { m_dieSENum--; }
		int GetDieSENum() { return m_dieSENum; }
	private:
		int m_walkSENum = 0;
		int m_attackSENum = 0;
		int m_damageSENum = 0;
		int m_knockBackSENum = 0;
		int m_dieSENum = 0;
	};

}