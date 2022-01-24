#include "stdafx.h"
#include "SpriteRender.h"
namespace nsMuscle {
	void SpriteRender::Init(const char* spritePath, UINT width, UINT height)
	{
		//�X�v���C�g�̏������f�[�^
		SpriteInitData initData;

		//�X�v���C�g�̃e�N�X�`���p�X
		initData.m_ddsFilePath[0] = spritePath;

		//�X�v���C�g�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		initData.m_vsEntryPointFunc = "VSMain";

		//�X�v���C�g�̃��[�h�ɉ����Ďg�p����s�N�Z���V�F�[�_�[�̃G���g���[�֐���ύX
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

		//�V�F�[�_�[�t�@�C���p�X�̎w��
		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		//�X�v���C�g�̕����w��
		initData.m_width = width;

		//�X�v���C�g�̍������w��
		initData.m_height = height;

		//�X�v���C�g�̃A���t�@�u�����h���[�h�𓧉߂Ɏw��
		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//�X�v���C�g�̃J���[�o�b�t�@�̃t�H�[�}�b�g���w��
		initData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		//�X�v���C�g�̏�����
		m_sprite.Init(initData);

		//�X�v���C�g�T�|�[�^�[�Ɏ����̑��݂�`����
		//m_spriteSupporter.SpriteRenderSetting(this);
	}

	void SpriteRender::Update()
	{
		//�X�v���C�g�T�|�[�^�[�̍X�V
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