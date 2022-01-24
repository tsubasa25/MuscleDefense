#include "stdafx.h"
#include "SkinModelRender.h"
#include "GameScene.h"
namespace nsMuscle {
	SkinModelRender::~SkinModelRender()
	{
	}
	void SkinModelRender::Update()
	{
		if (m_isActive == false) {
			return;
		}
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed, m_onPostCalcLocalMatrix);
		UpdateModel();
	}
	void SkinModelRender::InitSkeleton(const char* skeletonPath, AnimationClip* animClips, int animClipNum)
	{
		m_skeleton.Init(skeletonPath);
		m_initData[eModel].m_skeleton = &m_skeleton;
		m_initData[eModel_Shadow].m_skeleton = &m_skeleton;
		//�A�j���[�V�����֘A�̏�����
		m_animationClips = animClips;
		m_animationClipNum = animClipNum;
		m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);		
	}
	void SkinModelRender::Init(const char* modelPath, const char* maxModelPath, StructuredBuffer* muscleRateArraySB)
	{
		//���f���̃t�@�C���p�X�̎w��
		m_initData[eModel].m_tkmFilePath = modelPath;		
		//���L���L���f���̃t�@�C���p�X�̎w��
		if (maxModelPath != nullptr) {
			m_initData[eModel].m_maxTkmFilePath = maxModelPath;
		}
		//�V�F�[�_�[�p�X�̎w��
		m_initData[eModel].m_fxFilePath = "Assets/shader/model.fx";		
		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_initData[eModel].m_vsEntryPointFunc = "VSMain";		
		//�V�F�[�_�[�̃X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_initData[eModel].m_vsSkinEntryPointFunc = "VSSkinMain";		
		//�s�N�Z���V�F�[�_�[�B
		m_initData[eModel].m_psEntryPointFunc = "PSMain";		
		
		//�J���[�o�b�t�@�̃t�H�[�}�b�g���w��
		m_initData[eModel].m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;		
		//���f���f�[�^�̏�����̎����w��
		m_initData[eModel].m_modelUpAxis = enModelUpAxisZ;		
		//���f���ɉe�𗎂Ƃ����߂ɉe�̃e�N�X�`����R�t����
		m_initData[eModel].m_expandShaderResoruceView[0]= &PostEffectManager::GetInstance()->GetShadowMap();		
		//�萔�o�b�t�@�����f���ɕR�t����
		m_initData[eModel].m_expandConstantBufferSize[0] = LightManager::GetInstance()->GetLigDataSize();
		
		m_initData[eModel].m_expandConstantBuffer[0] = LightManager::GetInstance()->GetLigDatas();
		
		m_initData[eModel].m_expandConstantBufferSize[1] = LightManager::GetInstance()->GetLigCameraDataSize();
		m_initData[eModel].m_expandConstantBuffer[1] = LightManager::GetInstance()->GetLigCameraDatas();

		m_initData[eModel].m_muscleRateArraySB = muscleRateArraySB;
		if (m_isWipeModel) {
			m_initData[eModel_Wipe0] = m_initData[eModel];//���C�v�p�̃f�[�^�ɃR�s�[
			m_initData[eModel_Wipe1] = m_initData[eModel];//���C�v�p�̃f�[�^�ɃR�s�[
			m_initData[eModel_Wipe2] = m_initData[eModel];//���C�v�p�̃f�[�^�ɃR�s�[
			
		}
		m_initData[eModel_Shadow] = m_initData[eModel];//�e�p�̃f�[�^�ɃR�s�[

		m_initData[eModel_Shadow].m_fxFilePath = "Assets/shader/shadow.fx";
		m_initData[eModel_Shadow].m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		m_initData[eModel_Shadow].m_expandConstantBufferSize[0] = LightManager::GetInstance()->GetLigCameraDataSize();
		m_initData[eModel_Shadow].m_expandConstantBuffer[0] = LightManager::GetInstance()->GetLigCameraDatas();
		
		//���f���̏�����
		if (m_isWipeModel[0]) {
			m_model[eModel_Wipe0].Init(m_initData[eModel_Wipe0]);
		}
		if(m_isWipeModel[1]) {
			m_model[eModel_Wipe1].Init(m_initData[eModel_Wipe1]);
		}
		if (m_isWipeModel[2]) {
			m_model[eModel_Wipe2].Init(m_initData[eModel_Wipe2]);
		}		
		if (m_isMainModel) {
			m_model[eModel].Init(m_initData[eModel]);			
		}
		if (m_isShadowCaster) {
			m_model[eModel_Shadow].Init(m_initData[eModel_Shadow]);
		}
	}
	
	void SkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr,nullptr);
	}
	void SkinModelRender::Render(RenderContext& rc,Camera*camera)
	{	
		if (m_isActive == false) {
			return;
		}
		//�����_�[�R���e�L�X�g�̕`���ŕ���
		switch (rc.GetRenderStep()) {
		case RenderContext::eStep_Render:
			//���1�ɕ`��
			if (m_isMainModel) {
				m_model[eModel].Draw(rc, *camera);
			}
			break;
		case RenderContext::eStep_RenderWipe0:
			//���C�v��ʂɕ`��
			if (m_isWipeModel[0]) {
				m_model[eModel_Wipe0].Draw(rc, *camera);
			}
			break;
		case RenderContext::eStep_RenderWipe1:
			//���C�v��ʂɕ`��
			if (m_isWipeModel[1]) {
				m_model[eModel_Wipe1].Draw(rc, *camera);
			}
			break;
		case RenderContext::eStep_RenderWipe2:
			//���C�v��ʂɕ`��
			if (m_isWipeModel[2]) {
				m_model[eModel_Wipe2].Draw(rc, *camera);
			}
			break;		
		case RenderContext::eStep_RenderShadowMap:
			//�e����郂�f���̎������e��`��
			if (m_isShadowCaster)
			{
				m_model[eModel_Shadow].Draw(rc,*camera);
			}
			break;
		}
	
	}
	void SkinModelRender::UpdateModel()
	{
		//���f�����A�b�v�f�[�g
		for (auto& model : m_model) {
			model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
		}
		m_skeleton.Update(m_model[eModel_Shadow].GetWorldMatrix());
	}

	void SkinModelRender::SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	
	void SkinModelRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;
	}

	void SkinModelRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
	}

	Matrix SkinModelRender::GetWorldMatrixFromBoneName(const wchar_t* boneName)
	{
		int boneNo = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneNo)->GetWorldMatrix();
	}
}