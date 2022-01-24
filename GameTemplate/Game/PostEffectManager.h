#pragma once
#include "GaussianBlur.h"
namespace nsMuscle {
	namespace nsPostEffectConstant {
		const int MAX_WIPE_NUM = 3;
	}
class PostEffectManager
{
private:
	PostEffectManager() = default;
	~PostEffectManager() = default;

	static PostEffectManager* m_instance;
public:
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new PostEffectManager;
		}
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static PostEffectManager* GetInstance() { return m_instance; }

	/// <summary>
	/// 各種設定をオンにするかどうかを入力、現在はブルームのみ。
	/// </summary>
	/// <param name="bloomMode">ブルームをオンにするか?</param>
	void Init(bool bloomMode, bool shadowMode);

	bool GetShadowFlag()
	{
		return m_shadowMode;
	}

	Texture& GetShadowMap()
	{
		return m_shadowMap.GetRenderTargetTexture();
	}
	Texture& GetMainRender()
	{
		return m_mainRenderTarget.GetRenderTargetTexture();
	}
	Texture& GetBlurShadowMap()
	{
		return m_shadowBlur.GetBokeTexture();
	}
	Texture& GetWipeRender(int num)
	{
		return m_wipeRenderTarget[num].GetRenderTargetTexture();
	}
	RenderTarget& GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}
	RenderTarget& GetWipeRenderTarget(int num)
	{
		return m_wipeRenderTarget[num];
	}

	void WipeRender(RenderContext& rc, RenderTarget& rt);

	/// <summary>
	/// レンダリング前の処理
	/// </summary>
	void BeforeRender(RenderContext& rc, RenderTarget& rt);
	/// <summary>
	/// レンダリング前の処理
	/// </summary>
	//void BeforeWipeRender(RenderContext& rc, RenderTarget& rt);

	/// <summary>
	/// レンダリング後の処理
	/// </summary>
	/// <param name="rc"></param>
	void AfterRender(RenderContext& rc, RenderTarget& rt);
	/// <summary>
	/// レンダリング後の処理
	/// </summary>
	/// <param name="rc"></param>
	void AfterWipeRender(RenderContext& rc, RenderTarget& rt,int num);

	void ShadowRender(RenderContext& rc);

	void EndShadowRender(RenderContext& rc);
	
	void EndWipeRender(RenderContext& rc, RenderTarget& rt);
	
	//ワイプを画面に表示するか
	void SetIsWipeRender(bool flag) { m_isWipeRender = flag; }
	bool GetIsWipeRender() { return m_isWipeRender; }
	//ワイプ画面を全画面表示にする
	void WipeToFull(float interpolate,int wipeNum);

	void SetBlurMode(bool flag) { m_blurMode = flag; }
	void SetBokeAlphaBlendMode(AlphaBlendMode alphaBlendMode) { m_bokeSpriteInitData.m_alphaBlendMode = alphaBlendMode; m_bokeSprite.Init(m_bokeSpriteInitData);
	}
private:
	RenderTarget m_mainRenderTarget;
	
	Sprite m_copyToFrameBufferSprite;//画面に表示する最終結果のスプライト。

	//全体ぼかし
	bool m_blurMode = true;//ぼかしが有効か
	RenderTarget m_bokeRenderTarget;
	SpriteInitData m_bokeSpriteInitData;
	Sprite m_bokeSprite;
	GaussianBlur m_bokeGaussianBlur[4];//ブラー。

	//ブルーム関連。
	bool m_bloomMode = true; //ブルームが有効か
	
	RenderTarget m_luminnceRenderTarget;
	Sprite m_luminanceSprite;//輝度抽出したスプライト。
	Sprite m_bokeLuminanceSprite;//輝度抽出したものにブラーをかけたスプライト。
	GaussianBlur m_gaussianBlur[4];//ブラー。

	bool m_shadowMode = true;//シャドウが有効か。
	RenderTarget m_shadowMap;
	GaussianBlur m_shadowBlur;

	bool m_isWipeRender = false;
	RenderTarget m_wipeRenderTarget[nsPostEffectConstant::MAX_WIPE_NUM];
	//RenderTarget m_wipeLuminnceRenderTarget;
	//Sprite m_wipeLuminanceSprite;//輝度抽出したスプライト。
	//Sprite m_wipeBokeLuminanceSprite;//輝度抽出したものにブラーをかけたスプライト。
	//GaussianBlur m_wipeGaussianBlur[4];//ブラー。
	Sprite m_wipeCopyToFrameBufferSprite[nsPostEffectConstant::MAX_WIPE_NUM];

	SpriteInitData m_wipeSpriteInitData[nsPostEffectConstant::MAX_WIPE_NUM];
	btVector3 m_wipe = { 200.0f,150.0f,0.0f };
	btVector3 m_full = { 1280.0f,720.0f,0.0f };
	btVector3 m_lerpData = { 0.0,0.0,0.0 };

};
}


