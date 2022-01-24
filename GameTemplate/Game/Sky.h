#pragma once
namespace nsMuscle {
	class Sky :public IGameObject
	{
	public:

		/**
		 * @brief モデルの初期化関数　アニメーションつき
		 * @param modelPath モデルファイルのパス(.tkm)
		 * @param skeletonPath スケルトンファイルのパス(.tks)
		 * @param animClips アニメーションクリップの配列
		 * @param animClipNum アニメーションクリップの数
		*/
		void Init(const wchar_t* filePath);
		void Render(RenderContext& rc, Camera* camera)override;
		void Update()override;
		void SetPosition(Vector3 pos) { m_position = pos; }
		void SetRotation(Quaternion rot) { m_qRot = rot; }
	private:
		Model m_model; //モデル
		Model m_modelWipe;
		Vector3 m_scale = { 300.0f, 300.0f, 300.0f };//空の拡大率。
		Quaternion m_qRot;
		Vector3 m_position = Vector3::Zero;//座標。
		Texture m_skyTexture;
		bool m_isDirty = false;//ダーティフラグ。
	};
}