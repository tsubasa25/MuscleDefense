#pragma once
namespace nsMuscle {
	struct PointLigData
	{
		Vector3 position;//位置
		float pointLigNum;//何番目に作ったか
		Vector3 color;//色
		float range;//半径
	};
	class PointLight :public IGameObject
	{

		bool Start();
		void Update();

		PointLigData* m_pointLigData;	//ポイントライトのデータ

	public:
		PointLight();
		~PointLight();
		int GetPointLigNum() { return int(m_pointLigData->pointLigNum); }
		void SetPointLigNum(int num) { m_pointLigData->pointLigNum = num; }
		/**
			 * @brief ポイントライトのデータを取得
			 * @return ポイントライトのデータ
			*/
		PointLigData* GetLigData() { return m_pointLigData; }

		/**
		 * @brief ポイントライトのデータのサイズを取得
		 * @return ポイントライトのデータのサイズ
		*/
		int GetLigDataSize() { return sizeof(*m_pointLigData); }

		/**
		 * @brief ポイントライトの座標を設定
		 * @param pos ポイントライトの座標
		*/
		void SetPosition(Vector3 pos) { m_pointLigData->position = pos; }

		/**
		 * @brief ポイントライトの座標を取得
		 * @return ポイントライトの座標
		*/
		Vector3 GetPosition() { return m_pointLigData->position; }

		/**
		 * @brief ポイントライトの色を設定
		 * @param color ポイントライトの色
		*/
		void SetColor(Vector3 color) { m_pointLigData->color = color; }

		/**
		 * @brief ポイントライトの色を取得
		 * @return ポイントライトの色
		*/
		Vector3 GetColor() { return m_pointLigData->color; }

		/**
		 * @brief ポイントライトの影響範囲を設定
		 * @param range ポイントライトの影響範囲
		*/
		void SetRange(float range) { m_pointLigData->range = range; }

		/**
		 * @brief ポイントライトの影響範囲を取得
		 * @return ポイントライトの影響範囲
		*/
		float GetRange() { return m_pointLigData->range; }

	};
}