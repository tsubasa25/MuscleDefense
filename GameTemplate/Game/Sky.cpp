#include "stdafx.h"
#include "Sky.h"
namespace nsMuscle {
	void Sky::Init(const wchar_t* filePath)
	{
		//���f���̃t�@�C���p�X�̎w��
		ModelInitData initData;
		initData.m_tkmFilePath = "Assets/modelData/skyCube.tkm";

		//�V�F�[�_�[�p�X�̎w��
		initData.m_fxFilePath = "Assets/shader/Sky.fx";
		//initData.m_fxFilePath = "Assets/shader/Model.fx";

		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		initData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		initData.m_psEntryPointFunc = "PSMain_SkyCube";

		//�J���[�o�b�t�@�̃t�H�[�}�b�g���w��
		initData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//���f���f�[�^�̏�����̎����w��
		initData.m_modelUpAxis = enModelUpAxisZ;

		m_skyTexture.InitFromDDSFile(filePath);//�e�N�X�`���̃t�@�C���p�X

		initData.m_expandShaderResoruceView[0] = &m_skyTexture;

		//initData.m_expandConstantBuffer[0] = &m_skyTexture;
		//initData.m_expandConstantBufferSize[0] = sizeof(m_skyTexture);

		//���f���̏�����
		m_model.Init(initData);
		m_modelWipe.Init(initData);
	}
	void Sky::Render(RenderContext& rc, Camera* camera)
	{
		if (rc.GetRenderStep() == RenderContext::eStep_Render) {
			m_model.Draw(rc, *g_camera3D[0]);
		}
		/*if (rc.GetRenderStep() == RenderContext::eStep_RenderWipe0) {
			m_modelWipe.Draw(rc,*g_camera3D[1]);
		}
		if (rc.GetRenderStep() == RenderContext::eStep_RenderWipe1) {
			m_modelWipe.Draw(rc, *g_camera3D[2]);
		}
		if (rc.GetRenderStep() == RenderContext::eStep_RenderWipe2) {
			m_modelWipe.Draw(rc, *g_camera3D[3]);
		}
		if (rc.GetRenderStep() == RenderContext::eStep_RenderWipe3) {
			m_modelWipe.Draw(rc, *g_camera3D[4]);
		}*/
	}
	void Sky::Update()
	{
		m_model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
		m_modelWipe.UpdateWorldMatrix(m_position, m_qRot, m_scale);
	}
}