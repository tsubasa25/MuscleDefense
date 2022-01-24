#pragma once

namespace nsMuscle {
	class FontRender : public IGameObject
	{
	public:
		/**
			 * @brief フォントの描画　PostRenderなのでモデル等が描き終わった後に描かれる
			 * @param rc レンダーコンテキスト
			 * @param camera 描画するカメラ
			*/
		void PostRender(RenderContext& rc)override;

		/**
		 * @brief フォントのテキストをセット
		 * @param text テキスト
		*/
		void SetText(const std::wstring& text) { m_text = text; }

		/**
		 * @brief フォントの座標をセット
		 * @param position 座標
		*/
		void SetPosition(const Vector2& position) { m_position = position; }

		/**
		 * @brief フォントの色をセット
		 * @param 色
		*/
		void SetColor(const Vector4& color) { m_color = color; }

		/**
		 * @brief フォントの回転角度をセット
		 * @param 回転角度
		*/
		void SetRotation(const float& rotation) { m_rotation = rotation;; }

		/**
		 * @brief フォントの拡大率をセット
		 * @param scale 拡大率
		*/
		void SetScale(const float& scale) { m_scale = scale; }

		/**
		 * @brief フォントのピボットをセット(動作していません)
		 * @param pivot ピボット
		*/
		void SetPivot(const Vector2& pivot) { m_pivot = pivot; }


		/**
		 * @brief フォントのアウトラインの描画フラグをセット
		 * @param flag 描画する:true 描画しない:false
		*/
		void SetShadowFlag(bool flag)
		{
			m_drawShadowFlag = flag;
			m_PosX_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		/**
		 * @brief フォントのアウトラインの太さをセット
		 * @param offset アウトラインの太さ
		*/
		void SetShadowOffset(float offset)
		{
			m_shadowOffset = offset;
			m_PosX_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		/**
		 * @brief フォントのアウトラインの色をセット
		 * @param color アウトラインの色
		*/
		void SetShadowColor(Vector4 color)
		{
			m_shadowColor = color;
			m_PosX_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}
		/// <summary>
		/// フォントレンダーをアクティブ化する。
		/// </summary>
		void Activate()
		{
			m_isActive = true;
		}
		/// <summary>
		/// フォントレンダーを非アクティブ化する。
		/// </summary>
		/// <remark>
		/// 非アクティブ化すると、モデルが描画されなくなります。
		/// 一時的に表示をオフにしたい場合などに使用してください。
		/// </remark>
		void Deactivate()
		{
			m_isActive = false;

		}
	private:
		Font m_PosX_font;

		std::wstring m_text = L"";							//表示するテキスト
		Vector2 m_position = { 0.0f,0.0f };					//表示位置
		Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };			//フォントの色
		float m_rotation = 0.0f;							//フォントの回転角度
		float m_scale = { 1.0f };					//フォントの拡大率
		Vector2 m_pivot = { 0.5f,0.5f };					//フォントのピボット(動いていない?)
		Vector2 m_aspect = { 1.0f,1.0f };					//フォントのアスペクト比(1画面,2画面の切り替えで使う)
		bool m_drawShadowFlag = false;						//フォントのアウトラインを描くかのフラグ
		float m_shadowOffset = 1.0f;						//フォントのアウトラインの太さ
		Vector4 m_shadowColor = { 1.0f,1.0f,1.0f,1.0f };	//フォントのアウトラインの

		bool m_isActive = true;						//アクティブフラグ
	};
}