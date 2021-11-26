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
	nsMuscle::PostEffectManager::GetInstance()->BeforeRender(rc, nsMuscle::PostEffectManager::GetInstance()->GetMainRenderTarget());

	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B

	rc.SetStep(RenderContext::eStep_Render);

	//1P���̉�ʂ̃J������1P�̃J����(g_camera3D[0])
	nsMuscle::LightManager::GetInstance()->UpdateEyePos(0);

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc, g_camera3D[0]);
		}
	}
	//�|�X�g�G�t�F�N�g�p�BRender��̏���
	nsMuscle::PostEffectManager::GetInstance()->AfterRender(rc, nsMuscle::PostEffectManager::GetInstance()->GetMainRenderTarget());


	//�|�X�g�G�t�F�N�g�p�BRender�O�̏������C�v�p
	
	for (int i = 0; i < 4; i++) {
		nsMuscle::PostEffectManager::GetInstance()->BeforeRender(rc, nsMuscle::PostEffectManager::GetInstance()->GetWipeRenderTarget(i));

		////wipe
		switch (i)
		{
		case 0:
			rc.SetStep(RenderContext::eStep_RenderWipe0);
			break;
		case 1:
			rc.SetStep(RenderContext::eStep_RenderWipe1);
			break;
		case 2:
			rc.SetStep(RenderContext::eStep_RenderWipe2);
			break;
		case 3:
			rc.SetStep(RenderContext::eStep_RenderWipe3);
			break;
		default:
			break;
		}
	
		//ShadowRender�Ńr���[�|�[�g��ݒ肵�Ă���̂ł����Ńr���[�|�[�g�ݒ肵�Ȃ���OK(���Ԃ�)
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, g_camera3D[i+1]);
			}
		}
		nsMuscle::PostEffectManager::GetInstance()->EndWipeRender(rc, nsMuscle::PostEffectManager::GetInstance()->GetWipeRenderTarget(i));
		//�|�X�g�G�t�F�N�g�p�BRender��̏������C�v�p
		nsMuscle::PostEffectManager::GetInstance()->AfterWipeRender(rc, nsMuscle::PostEffectManager::GetInstance()->GetWipeRenderTarget(i), i);

	}
	
	/*for (int i = 0; i < 4; i++) {
		
		}*/
	//nsMuscle::PostEffectManager::GetInstance()->AfterWipeRender(rc, nsMuscle::PostEffectManager::GetInstance()->GetWipeRenderTarget(1));

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
}
