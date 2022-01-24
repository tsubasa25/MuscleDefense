#include "stdafx.h"
#include "PostEffectManager.h"

namespace nsMuscle {

	PostEffectManager* PostEffectManager::m_instance = nullptr;

	void PostEffectManager::Init(bool bloomMode, bool shadowMode)
	{
		m_bloomMode = bloomMode;
		m_shadowMode = shadowMode;

		//�ŏI�I�ɉ�ʂɏo�͂���p�̃����_�[�^�[�Q�b�g
		m_mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
		for (int i = 0; i < nsPostEffectConstant::MAX_WIPE_NUM; i++) {
			m_wipeRenderTarget[i].Create(
				1280,
				720,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_D32_FLOAT
			);
		}
		if (m_bloomMode == true)
		{
			//�P�x�𒊏o�������̂��i�[����郌���_�[�^�[�Q�b�g
			m_luminnceRenderTarget.Create(
				1280,
				720,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_D32_FLOAT
			);

			//�s�N�Z���V�F�[�_�[PSSamplingLuminance�ŁA���C�������_�[�^�[�Q�b�g����P�x�𒊏o����p�̃X�v���C�g�B
			SpriteInitData luminanceSpriteInitData;
			luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
			luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
			luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
			luminanceSpriteInitData.m_width = 1280;
			luminanceSpriteInitData.m_height = 720;
			luminanceSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
			luminanceSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

			m_luminanceSprite.Init(luminanceSpriteInitData);

			//�P�x���o�����X�v���C�g�����ƂɃu���[������������悤�ɃZ�b�g�B
			m_gaussianBlur[0].Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
			m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
			m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
			m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

			//�P�x���o���ꂽ���̂Ƀu���[���������X�v���C�g�B
			SpriteInitData bokeLuminanceSpriteInitData;
			bokeLuminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
			bokeLuminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
			bokeLuminanceSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
			bokeLuminanceSpriteInitData.m_width = 1280;
			bokeLuminanceSpriteInitData.m_height = 720;
			bokeLuminanceSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
			bokeLuminanceSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
			bokeLuminanceSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
			bokeLuminanceSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
			bokeLuminanceSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
			bokeLuminanceSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

			m_bokeLuminanceSprite.Init(bokeLuminanceSpriteInitData);

			////////////////////////////////////////////////////////////////////////////////////
			//���C�v��ʂ���P�x�𒊏o����悤�̃X�v���C�g
			////�P�x�𒊏o�������̂��i�[����郌���_�[�^�[�Q�b�g
			//m_wipeLuminnceRenderTarget.Create(
			//	1280,
			//	720,
			//	1,
			//	1,
			//	DXGI_FORMAT_R32G32B32A32_FLOAT,
			//	DXGI_FORMAT_D32_FLOAT
			//);

			//SpriteInitData wipeLuminanceSpriteInitData = luminanceSpriteInitData;
			//wipeLuminanceSpriteInitData.m_textures[0] = &m_wipeRenderTarget[0].GetRenderTargetTexture();
			//m_wipeLuminanceSprite.Init(wipeLuminanceSpriteInitData);
			//
			////�P�x���o�����X�v���C�g�����ƂɃu���[������������悤�ɃZ�b�g�B
			//m_wipeGaussianBlur[0].Init(&m_wipeLuminnceRenderTarget.GetRenderTargetTexture());
			//m_wipeGaussianBlur[1].Init(&m_wipeGaussianBlur[0].GetBokeTexture());
			//m_wipeGaussianBlur[2].Init(&m_wipeGaussianBlur[1].GetBokeTexture());
			//m_wipeGaussianBlur[3].Init(&m_wipeGaussianBlur[2].GetBokeTexture());
			////�P�x���o���ꂽ���̂Ƀu���[���������X�v���C�g�B
			//SpriteInitData wipeBokeLuminanceSpriteInitData = bokeLuminanceSpriteInitData;
			//wipeBokeLuminanceSpriteInitData.m_textures[0] = &m_wipeGaussianBlur[0].GetBokeTexture();
			//wipeBokeLuminanceSpriteInitData.m_textures[1] = &m_wipeGaussianBlur[1].GetBokeTexture();
			//wipeBokeLuminanceSpriteInitData.m_textures[2] = &m_wipeGaussianBlur[2].GetBokeTexture();
			//wipeBokeLuminanceSpriteInitData.m_textures[3] = &m_wipeGaussianBlur[3].GetBokeTexture();
			//m_wipeBokeLuminanceSprite.Init(wipeBokeLuminanceSpriteInitData);
		}
		if (m_blurMode)
		{
			
			m_bokeGaussianBlur[0].Init(&m_mainRenderTarget.GetRenderTargetTexture());
			m_bokeGaussianBlur[1].Init(&m_bokeGaussianBlur[0].GetBokeTexture());
			m_bokeGaussianBlur[2].Init(&m_bokeGaussianBlur[1].GetBokeTexture());
			m_bokeGaussianBlur[3].Init(&m_bokeGaussianBlur[2].GetBokeTexture());
			//�u���[���������X�v���C�g�B
			
			m_bokeSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
			m_bokeSpriteInitData.m_vsEntryPointFunc = "VSMain";
			m_bokeSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
			m_bokeSpriteInitData.m_width = 1280;
			m_bokeSpriteInitData.m_height = 720;
			//m_bokeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
			m_bokeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
			m_bokeSpriteInitData.m_textures[0] = &m_bokeGaussianBlur[0].GetBokeTexture();
			m_bokeSpriteInitData.m_textures[1] = &m_bokeGaussianBlur[1].GetBokeTexture();
			m_bokeSpriteInitData.m_textures[2] = &m_bokeGaussianBlur[2].GetBokeTexture();
			m_bokeSpriteInitData.m_textures[3] = &m_bokeGaussianBlur[3].GetBokeTexture();
			m_bokeSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

			m_bokeSprite.Init(m_bokeSpriteInitData);
		}


		if (m_shadowMode)
		{
			//�V���h�E�}�b�v�̍쐬�B
			float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
			m_shadowMap.Create(
				2048,
				2048,
				1,
				1,
				DXGI_FORMAT_R32_FLOAT,
				DXGI_FORMAT_D32_FLOAT,
				clearColor
			);
		}
		
		//�ŏI�I�ȉ�ʂɏo�͂����X�v���C�g�B
		SpriteInitData copyToBufferSpriteInitData;
		copyToBufferSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		copyToBufferSpriteInitData.m_width = 1280;
		copyToBufferSpriteInitData.m_height = 720;
		copyToBufferSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToFrameBufferSprite.Init(copyToBufferSpriteInitData);
		
		
		for (int i = 0; i < nsPostEffectConstant::MAX_WIPE_NUM; i++) {
			m_wipeSpriteInitData[i].m_textures[0] = &nsMuscle::PostEffectManager::GetInstance()->GetWipeRender(i);
			m_wipeSpriteInitData[i].m_fxFilePath = "Assets/shader/sprite.fx";
			m_wipeSpriteInitData[i].m_width = 200.0f;//256
			m_wipeSpriteInitData[i].m_height = 150.0f;//* 9 / 16;//512
			m_wipeCopyToFrameBufferSprite[i].Init(m_wipeSpriteInitData[i]);
		}
		float frame_h = 720.0f;
		m_wipeCopyToFrameBufferSprite[0].Update({ FRAME_BUFFER_W / 2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 1.0f, 1.0f });//�E��
		m_wipeCopyToFrameBufferSprite[1].Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });//����
		m_wipeCopyToFrameBufferSprite[2].Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H/-2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 0.0f });//����
		
}

	void PostEffectManager::ShadowRender(RenderContext& rc)
	{
		if (m_shadowMode)
		{
			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
			rc.SetRenderTargetAndViewport(m_shadowMap);
			rc.ClearRenderTargetView(m_shadowMap.GetRTVCpuDescriptorHandle(), m_shadowMap.GetRTVClearColor());
			rc.ClearDepthStencilView(m_shadowMap.GetDSVCpuDescriptorHandle(), m_shadowMap.GetDSVClearValue());

			D3D12_RECT shadowRect;
			shadowRect.left = 0;
			shadowRect.top = 0;
			shadowRect.right = 2048;
			shadowRect.bottom = 2048;
			rc.SetScissorRect(shadowRect);
		}
	}

	void PostEffectManager::EndShadowRender(RenderContext& rc)
	{
		if (m_shadowMode)
		{
			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

			//m_shadowBlur.ExecuteOnGPU(rc, 2.0f);

			D3D12_RECT normalRect;
			normalRect.left = 0;
			normalRect.top = 0;
			normalRect.right = 1280;
			normalRect.bottom = 720;
			rc.SetScissorRect(normalRect);
		}
	}

	void PostEffectManager::WipeRender(RenderContext& rc, RenderTarget& rt)
	{
		rc.WaitUntilToPossibleSetRenderTarget(rt);
		rc.SetRenderTargetAndViewport(rt);
		rc.ClearRenderTargetView(rt.GetRTVCpuDescriptorHandle(), rt.GetRTVClearColor());
		rc.ClearDepthStencilView(rt.GetDSVCpuDescriptorHandle(), rt.GetDSVClearValue());
	}

	void PostEffectManager::EndWipeRender(RenderContext& rc, RenderTarget& rt)
	{
		rc.WaitUntilFinishDrawingToRenderTarget(rt);

		D3D12_RECT normalRect;
		normalRect.left = 0;
		normalRect.top = 0;
		normalRect.right = 1280;
		normalRect.bottom = 720;
		rc.SetScissorRect(normalRect);
	}


	void PostEffectManager::BeforeRender(RenderContext& rc, RenderTarget& rt)
	{
		//���C�������_�[�^�[�Q�b�g��`���ɃZ�b�g����B
		rc.WaitUntilToPossibleSetRenderTarget(rt);
		rc.SetRenderTargetAndViewport(rt);
		rc.ClearRenderTargetView(rt.GetRTVCpuDescriptorHandle(), rt.GetRTVClearColor());
		rc.ClearDepthStencilView(rt.GetDSVCpuDescriptorHandle(), rt.GetDSVClearValue());
		//���̌�A�e���f���̃h���[�R�[�����Ă΂��(�͂�)�B 		
	}

	void PostEffectManager::AfterRender(RenderContext& rc, RenderTarget& rt)
	{
		//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
		rc.WaitUntilFinishDrawingToRenderTarget(rt);
		
		if (m_bloomMode == true)
		{
			//�P�x���o�p�̃����_�[�^�[�Q�b�g�ɕύX����B
			rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);
			rc.SetRenderTargetAndViewport(m_luminnceRenderTarget);
			rc.ClearRenderTargetView(m_luminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_luminnceRenderTarget.GetRTVClearColor());
			//�P�x���o���s��
			m_luminanceSprite.Draw(rc);
			//�P�x���o�p�̃����_�[�^�[�Q�b�g���`����I����܂ő҂B
			rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

			//�K�E�V�A���u���[�����s����
			m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
			m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
			m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
			m_gaussianBlur[3].ExecuteOnGPU(rc, 10);

			//���C�������_�[�^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
			rc.WaitUntilToPossibleSetRenderTarget(rt);
			rc.SetRenderTargetAndViewport(rt);
			//�P�x���o�������̂��{�J�����������C�������_�[�^�[�Q�b�g�ɉ��Z����
			//���̎��_�Ń��C�������_�[�^�[�Q�b�g�ɂ͌��̉摜�ƃu���[�̉摜�����킳�������̂ɂȂ�B
			m_bokeLuminanceSprite.Draw(rc);
			//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
			rc.WaitUntilFinishDrawingToRenderTarget(rt);
		}
		if (m_blurMode)
		{
			//�P�x���o�p�̃����_�[�^�[�Q�b�g�ɕύX����B
			/*rc.WaitUntilToPossibleSetRenderTarget(rt);
			rc.SetRenderTargetAndViewport(rt);
			rc.ClearRenderTargetView(rt.GetRTVCpuDescriptorHandle(), rt.GetRTVClearColor());
			*/
			//�K�E�V�A���u���[�����s����
			m_bokeGaussianBlur[0].ExecuteOnGPU(rc, 1);
 			m_bokeGaussianBlur[1].ExecuteOnGPU(rc, 1);
			//m_bokeGaussianBlur[2].ExecuteOnGPU(rc, 1);
			//m_bokeGaussianBlur[3].ExecuteOnGPU(rc, 1);

			//���C�������_�[�^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
			rc.WaitUntilToPossibleSetRenderTarget(rt);
			rc.SetRenderTargetAndViewport(rt);

			//�P�x���o�������̂��{�J�����������C�������_�[�^�[�Q�b�g�ɉ��Z����
			//���̎��_�Ń��C�������_�[�^�[�Q�b�g�ɂ͌��̉摜�ƃu���[�̉摜�����킳�������̂ɂȂ�B
			m_bokeSprite.Draw(rc);
			//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
			rc.WaitUntilFinishDrawingToRenderTarget(rt);
		}

		//�`�����t���[���o�b�t�@�ɂ���B
		rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBuffuerDSV());

		//���C�������_�[�^�[�Q�b�g�ɕ`����Ă����ŏI�C���[�W����ʏ�ɕ`���B
		m_copyToFrameBufferSprite.Draw(rc);
	}

	//void PostEffectManager::BeforeWipeRender(RenderContext& rc, RenderTarget& rt)
	//{
	//	//���C�������_�[�^�[�Q�b�g��`���ɃZ�b�g����B
	//	rc.WaitUntilToPossibleSetRenderTarget(rt);
	//	rc.SetRenderTargetAndViewport(rt);
	//	rc.ClearRenderTargetView(rt.GetRTVCpuDescriptorHandle(), rt.GetRTVClearColor());
	//	rc.ClearDepthStencilView(rt.GetDSVCpuDescriptorHandle(), rt.GetDSVClearValue());
	//	//���̌�A�e���f���̃h���[�R�[�����Ă΂��(�͂�)�B 		
	//}

	void PostEffectManager::AfterWipeRender(RenderContext& rc, RenderTarget& rt,int num)
	{
		//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
		rc.WaitUntilFinishDrawingToRenderTarget(rt);

		//if (m_bloomMode == true)
		//{
		//	//�P�x���o�p�̃����_�[�^�[�Q�b�g�ɕύX����B
		//	rc.WaitUntilToPossibleSetRenderTarget(m_wipeLuminnceRenderTarget);
		//	rc.SetRenderTargetAndViewport(m_wipeLuminnceRenderTarget);
		//	rc.ClearRenderTargetView(m_wipeLuminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_wipeLuminnceRenderTarget.GetRTVClearColor());
		//	//�P�x���o���s��
		//	m_wipeLuminanceSprite.Draw(rc);
		//	//�P�x���o�p�̃����_�[�^�[�Q�b�g���`����I����܂ő҂B
		//	rc.WaitUntilFinishDrawingToRenderTarget(m_wipeLuminnceRenderTarget);

		//	//�K�E�V�A���u���[�����s����
		//	m_wipeGaussianBlur[0].ExecuteOnGPU(rc, 10);
		//	m_wipeGaussianBlur[1].ExecuteOnGPU(rc, 10);
		//	m_wipeGaussianBlur[2].ExecuteOnGPU(rc, 10);
		//	m_wipeGaussianBlur[3].ExecuteOnGPU(rc, 10);

		//	//���C�������_�[�^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
		//	rc.WaitUntilToPossibleSetRenderTarget(rt);
		//	rc.SetRenderTargetAndViewport(rt);
		//	//�P�x���o�������̂��{�J�����������C�������_�[�^�[�Q�b�g�ɉ��Z����
		//	//���̎��_�Ń��C�������_�[�^�[�Q�b�g�ɂ͌��̉摜�ƃu���[�̉摜�����킳�������̂ɂȂ�B
		//	m_wipeBokeLuminanceSprite.Draw(rc);
		//	//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
		//	rc.WaitUntilFinishDrawingToRenderTarget(rt);
		//}


		//�`�����t���[���o�b�t�@�ɂ���B
		rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBuffuerDSV());

		//���C�������_�[�^�[�Q�b�g�ɕ`����Ă����ŏI�C���[�W����ʏ�ɕ`���B
		m_wipeCopyToFrameBufferSprite[num].Draw(rc);
	}
	void PostEffectManager::WipeToFull(float interpolate,int wipeNum)
	{
		if (interpolate>=0.0f && 1.0f>=interpolate) {
			m_lerpData = lerp(m_wipe, m_full, interpolate);
			m_wipeSpriteInitData[wipeNum].m_width = m_lerpData.getX();
			m_wipeSpriteInitData[wipeNum].m_height = m_lerpData.getY();
			m_wipeCopyToFrameBufferSprite[wipeNum].Init(m_wipeSpriteInitData[wipeNum]);
			
			switch (wipeNum)
			{
			case 0:
				m_wipeCopyToFrameBufferSprite[wipeNum].Update({ FRAME_BUFFER_W /2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 1.0f, 1.0f });//�E��
				break;
			case 1:
				m_wipeCopyToFrameBufferSprite[wipeNum].Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });//����
				break;
			case 2:
				m_wipeCopyToFrameBufferSprite[wipeNum].Update({ FRAME_BUFFER_W / -2.0f,FRAME_BUFFER_H / -2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 0.0f });//����
				break;			
			default:
				break;
			}

		}
	}
}