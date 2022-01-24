#include "stdafx.h"
#include "SpriteRender.h"
namespace nsMuscle {
	void SpriteRender::Init(const char* spritePath, UINT width, UINT height)
	{
		//スプライトの初期化データ
		SpriteInitData initData;

		//スプライトのテクスチャパス
		initData.m_ddsFilePath[0] = spritePath;

		//スプライトの頂点シェーダーのエントリー関数名の指定
		initData.m_vsEntryPointFunc = "VSMain";

		//スプライトのモードに応じて使用するピクセルシェーダーのエントリー関数を変更
		switch (m_spriteMode)
		{
		case SpriteRender::Normal:
			initData.m_psEntryPoinFunc = "PSMain";
			break;
		//case SpriteRender::Transition:
		//	initData.m_psEntryPoinFunc = "PS_Transition";
		//	//m_sprite.SetAlpha(0.0f);
		//	break;
		default:
			initData.m_psEntryPoinFunc = "PSMain";
			break;
		}

		//シェーダーファイルパスの指定
		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		//スプライトの幅を指定
		initData.m_width = width;

		//スプライトの高さを指定
		initData.m_height = height;

		//スプライトのアルファブレンドモードを透過に指定
		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//スプライトのカラーバッファのフォーマットを指定
		initData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		//スプライトの初期化
		m_sprite.Init(initData);

		//スプライトサポーターに自分の存在を伝える
		//m_spriteSupporter.SpriteRenderSetting(this);
	}

	void SpriteRender::Update()
	{
		//スプライトサポーターの更新
		//m_spriteSupporter.SpriteSupporter_Update();
	}

	/*void SpriteRender::Render(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == RenderContext::eStep_Render) {
			m_sprite.Draw(rc);
		}
	}*/
	void SpriteRender::PostRender(RenderContext& rc)
	{
		m_sprite.Draw(rc);
	}

	void SpriteRender::SetPosition(Vector3 pos)
	{
		m_position = pos;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void SpriteRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void SpriteRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void SpriteRender::SetPivot(Vector2 pivot)
	{
		m_pivot = pivot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}
}