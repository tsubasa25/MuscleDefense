#pragma once
namespace nsMuscle {
	struct DirLigData {
		Vector3 direction;//ライトの方向
		float pad0;//何番目に作ったか
		Vector3 color;	 //ライトのカラー
		float pad1;
	};
	class DirectionLight :public IGameObject
	{
		bool Start();
		void Update();
		//データを作成する
		DirLigData* m_dirLigData;

	public:
		~DirectionLight();
		DirectionLight();
		//ディレクションライトのデータを取得する
		DirLigData* GetLigData() { return m_dirLigData; }
		//ディレクションライトのサイズを取得する
		int GetLigDataSize() { return sizeof(*m_dirLigData); }
		//向きを設定する
		void SetDirection(Vector3 dir) {
			m_dirLigData->direction = dir;
			m_dirLigData->direction.Normalize();
		}

		//向き情報を取得する
		Vector3 GetDirection() { return m_dirLigData->direction; }

		//色を設定する
		void SetColor(Vector3 Color) { m_dirLigData->color = Color; }

		//色情報を取得する
		Vector3 GetColor() { return m_dirLigData->color; }

		Quaternion qRotY;
	};
}