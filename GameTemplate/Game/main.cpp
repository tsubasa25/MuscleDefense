#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"
#include "BackGround.h"
namespace
{	
	const Vector3 LIGHTCAMERA_UP = { 0.0f,0.0f,1.0f };
	const float LIGHTCAMERA_WIDTH = 2000.0f;
	const float LIGHTCAMERA_HEIGHT = 2000.0f;
	const int CAMERA_FAR = 100000;
}

//////////////////////////////////////
// �֐��錾
//////////////////////////////////////

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	g_camera3D[0]->SetPosition({ 0.0f, 150.0f, 200.0f });
	g_camera3D[0]->SetTarget({ 0.0f, 50.0f, 0.0f });
	
	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	nsMuscle::PostEffectManager::CreateInstance();

	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	nsMuscle::LightManager::CreateInstance();
	nsMuscle::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
	nsMuscle::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
	nsMuscle::LightManager::GetInstance()->SetPointLigNum(0);
	nsMuscle::LightManager::GetInstance()->SetSpotLigNum(0);

	//�u���[���t���O�A�V���h�E�t���O�̏���
	nsMuscle::PostEffectManager::GetInstance()->Init(true,true);

	NewGO<nsMuscle::GameScene>(0);
	NewGO<nsMuscle::BackGround>(0);
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();		

		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		nsMuscle::LightManager::GetInstance()->UpdateEyePos();
		// �V���h�E�}�b�v��\�����邽�߂̃X�v���C�g������������
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &nsMuscle::PostEffectManager::GetInstance()->GetMainRender();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_width = 256;//256
		spriteInitData.m_height = 256;
		Sprite sprite;

		sprite.Init(spriteInitData);

		sprite.Update({ FRAME_BUFFER_W / 2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 1.0f, 1.0f });
		
		sprite.Draw(renderContext);
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

