/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
	//�����G���W���̃A�b�v�f�[�g���Ăяo���B
	PhysicsWorld::GetInstance()->Update(1.0f/60.0f);
}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	//TODO:�b�菈���A�t���O�܂ߕʂ̌`�ɂ�����
	//�e���ɕ`���Ă��烂�f���ɕ`�����e��`�������̂Ő�ɃV���h�E�}�b�v������B
	nsMuscle::PostEffectManager::GetInstance()->ShadowRender(rc);

	////shadow
	if (nsMuscle::PostEffectManager::GetInstance()->GetShadowFlag())
	{
		rc.SetStep(RenderContext::eStep_RenderShadowMap);
		//ShadowRender�Ńr���[�|�[�g��ݒ肵�Ă���̂ł����Ńr���[�|�[�g�ݒ肵�Ȃ���OK(���Ԃ�)
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, nsMuscle::LightManager::GetInstance()->GetLightCamera());
			}
		}
	}
	nsMuscle::PostEffectManager::GetInstance()->EndShadowRender(rc);

	//�|�X�g�G�t�F�N�g�p�BRender�O�̏���
	nsMuscle::PostEffectManager::GetInstance()->BeforeRender(rc);	

	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B

	rc.SetStep(RenderContext::eStep_Render);

	//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
	nsMuscle::LightManager::GetInstance()->UpdateEyePos(0);

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc, g_camera3D[0]);
		}
	}
	//�|�X�g�G�t�F�N�g�p�BRender�O�̏������C�v�p
	nsMuscle::PostEffectManager::GetInstance()->BeforeWipeRender(rc);

	nsMuscle::PostEffectManager::GetInstance()->WipeRender(rc);

	////wipe

	rc.SetStep(RenderContext::eStep_RenderWipe);
	//ShadowRender�Ńr���[�|�[�g��ݒ肵�Ă���̂ł����Ńr���[�|�[�g�ݒ肵�Ȃ���OK(���Ԃ�)
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc, g_camera3D[1]);
		}
	}

	nsMuscle::PostEffectManager::GetInstance()->EndWipeRender(rc);
	//�|�X�g�G�t�F�N�g�p�BRender��̏���
	nsMuscle::PostEffectManager::GetInstance()->AfterRender(rc);
	//�|�X�g�G�t�F�N�g�p�BRender��̏������C�v�p
	nsMuscle::PostEffectManager::GetInstance()->AfterWipeRender(rc);
}
void GameObjectManager::ExecutePostRender(RenderContext& rc)
{
	if (m_rc == nullptr) {
		m_rc = &rc;
	}

	rc.SetStep(RenderContext::eStep_Render);

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->PostRenderWrapper(rc);
		}
	}
	//Level2D�p�@
	{
		g_camera2D->SetWidth(g_graphicsEngine->GetFrameBufferWidth());

		rc.SetStep(RenderContext::eStep_Render);

		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->PostRenderWrapper(rc);
			}
		}
	}
}