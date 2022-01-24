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
	/// �e��ݒ���I���ɂ��邩�ǂ�������́A���݂̓u���[���̂݁B
	/// </summary>
	/// <param name="bloomMode">�u���[�����I���ɂ��邩?</param>
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
	/// �����_�����O�O�̏���
	/// </summary>
	void BeforeRender(RenderContext& rc, RenderTarget& rt);
	/// <summary>
	/// �����_�����O�O�̏���
	/// </summary>
	//void BeforeWipeRender(RenderContext& rc, RenderTarget& rt);

	/// <summary>
	/// �����_�����O��̏���
	/// </summary>
	/// <param name="rc"></param>
	void AfterRender(RenderContext& rc, RenderTarget& rt);
	/// <summary>
	/// �����_�����O��̏���
	/// </summary>
	/// <param name="rc"></param>
	void AfterWipeRender(RenderContext& rc, RenderTarget& rt,int num);

	void ShadowRender(RenderContext& rc);

	void EndShadowRender(RenderContext& rc);
	
	void EndWipeRender(RenderContext& rc, RenderTarget& rt);
	
	//���C�v����ʂɕ\�����邩
	void SetIsWipeRender(bool flag) { m_isWipeRender = flag; }
	bool GetIsWipeRender() { return m_isWipeRender; }
	//���C�v��ʂ�S��ʕ\���ɂ���
	void WipeToFull(float interpolate,int wipeNum);

	void SetBlurMode(bool flag) { m_blurMode = flag; }
	void SetBokeAlphaBlendMode(AlphaBlendMode alphaBlendMode) { m_bokeSpriteInitData.m_alphaBlendMode = alphaBlendMode; m_bokeSprite.Init(m_bokeSpriteInitData);
	}
private:
	RenderTarget m_mainRenderTarget;
	
	Sprite m_copyToFrameBufferSprite;//��ʂɕ\������ŏI���ʂ̃X�v���C�g�B

	//�S�̂ڂ���
	bool m_blurMode = true;//�ڂ������L����
	RenderTarget m_bokeRenderTarget;
	SpriteInitData m_bokeSpriteInitData;
	Sprite m_bokeSprite;
	GaussianBlur m_bokeGaussianBlur[4];//�u���[�B

	//�u���[���֘A�B
	bool m_bloomMode = true; //�u���[�����L����
	
	RenderTarget m_luminnceRenderTarget;
	Sprite m_luminanceSprite;//�P�x���o�����X�v���C�g�B
	Sprite m_bokeLuminanceSprite;//�P�x���o�������̂Ƀu���[���������X�v���C�g�B
	GaussianBlur m_gaussianBlur[4];//�u���[�B

	bool m_shadowMode = true;//�V���h�E���L�����B
	RenderTarget m_shadowMap;
	GaussianBlur m_shadowBlur;

	bool m_isWipeRender = false;
	RenderTarget m_wipeRenderTarget[nsPostEffectConstant::MAX_WIPE_NUM];
	//RenderTarget m_wipeLuminnceRenderTarget;
	//Sprite m_wipeLuminanceSprite;//�P�x���o�����X�v���C�g�B
	//Sprite m_wipeBokeLuminanceSprite;//�P�x���o�������̂Ƀu���[���������X�v���C�g�B
	//GaussianBlur m_wipeGaussianBlur[4];//�u���[�B
	Sprite m_wipeCopyToFrameBufferSprite[nsPostEffectConstant::MAX_WIPE_NUM];

	SpriteInitData m_wipeSpriteInitData[nsPostEffectConstant::MAX_WIPE_NUM];
	btVector3 m_wipe = { 200.0f,150.0f,0.0f };
	btVector3 m_full = { 1280.0f,720.0f,0.0f };
	btVector3 m_lerpData = { 0.0,0.0,0.0 };

};
}


