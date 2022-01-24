#pragma once
namespace nsMuscle {
	struct SpotLigData
	{
		Vector3 position = Vector3::Zero;
		float spotLigNum = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
		Vector3 direction = Vector3::Zero;
		float angle = 0.0f;
	};
	class SpotLight :public IGameObject
	{

		bool Start();
		void Update();
		SpotLigData* m_spotLigData;	//スポットライトのデータ

	public:
		SpotLight();
		~SpotLight();
		/**
			 * @brief スポットライトのデータを取得
			 * @return スポットライトのデータ
			*/
		SpotLigData* GetLigData() { return m_spotLigData; }

		/**
		 * @brief スポットライトのデータのサイズを取得
		 * @return スポットライトのデータのサイズ
		*/
		int GetLigDataSize() { return sizeof(SpotLigData); }

		/**
		 * @brief スポットライトの座標を設定
		 * @param pos スポットライトの座標
		*/
		void SetPosition(Vector3 pos) { m_spotLigData->position= pos; }

		/**
		 * @brief スポットライトの座標を取得
		 * @return スポットライトの座標
		*/
		Vector3 GetPosition() { return m_spotLigData->position; }

		/**
		 * @brief スポットライトの色を設定
		 * @param color スポットライトの色
		*/
		void SetColor(Vector3 color) { m_spotLigData->color = color; }

		/**
		 * @brief スポットライトの色を取得
		 * @return スポットライトの色
		*/
		Vector3 GetColor() { return m_spotLigData->color; }

		/**
		 * @brief スポットライトの影響範囲を設定
		 * @param range スポットライトの影響範囲
		*/
		void SetRange(float range) { m_spotLigData->range = range; }

		/**
		 * @brief スポットライトの影響範囲を取得
		 * @return スポットライトの影響範囲
		*/
		float GetRange() { return m_spotLigData->range; }

		/**
		 * @brief スポットライトの向きを設定
		 * @param dir スポットライトの向き
		*/
		void SetDirection(Vector3 dir) { m_spotLigData->direction = dir; }

		/**
		 * @brief スポットライトの向きを取得
		 * @return スポットライトの向き
		*/
		Vector3 GetDirection() { return m_spotLigData->direction; }

		/**
		 * @brief スポットライトの影響角度を設定(ラジアン単位)
		 * @param angle_rad スポットライトの影響角度
		*/
		void SetAngle(float angle_rad) { m_spotLigData->angle = angle_rad; }

		/**
		 * @brief スポットライトの影響角度を取得(ラジアン単位)
		 * @return スポットライトの影響角度
		*/
		float GetAngle() { return m_spotLigData->range; }

		/**
		 * @brief スポットライトの影響角度を設定(デグリー単位)
		 * @param angle_deg スポットライトの影響角度
		*/
		void SetAngleDeg(float angle_deg) { m_spotLigData->angle = Math::DegToRad(angle_deg); }

		/**
		 * @brief スポットライトの影響角度を取得(デグリー単位)
		 * @return スポットライトの影響角度
		*/
		float GetAngleDeg() { return Math::RadToDeg(m_spotLigData->angle); }

	};

}