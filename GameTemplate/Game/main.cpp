#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"
#include "BackGround.h"
#include "TitleScene.h"
#include "DebagWireFrame.h"
#include "WipeCamera.h"
namespace
{	
	const Vector3 LIGHTCAMERA_UP = { -5.0f,-3.0f,0.0f };
	const float LIGHTCAMERA_WIDTH = 2000.0f;
	const float LIGHTCAMERA_HEIGHT = 2000.0f;
	const int CAMERA_FAR = 100000;
}

//////////////////////////////////////
// �֐��錾
//////////////////////////////////////
bool isGameEnd = false;//�Q�[���I����
///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	nsMuscle::PostEffectManager::CreateInstance();
	//���\�[�X�}�l�[�W���̃C���X�^���X���쐬
	nsMuscle::ResourceBankManager::CreateInstance();

	
	//���C�g�}�l�[�W���[�̃C���X�^���X���쐬
	nsMuscle::LightManager::CreateInstance();
	nsMuscle::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
	nsMuscle::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
	nsMuscle::LightManager::GetInstance()->SetPointLigNum(0);
	nsMuscle::LightManager::GetInstance()->SetSpotLigNum(0);
	nsMuscle::LightManager::GetInstance()->SetLightCameraFar(10000.0f);
	//DebugWireframe::Init();
	//�u���[���t���O�A�V���h�E�t���O�̏���
	nsMuscle::PostEffectManager::GetInstance()->Init(true,true);
	NewGO<nsMuscle::WipeCamera>(0);
	NewGO<nsMuscle::TitleScene>(0, "titleScene");
	NewGO<nsMuscle::GameScene>(0, "gameScene");
	//NewGO<nsMuscle::GameScene>(0,"gameScene");
	//NewGO<nsMuscle::BackGround>(0,"backGround");
	
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
		GameObjectManager::GetInstance()->ExecutePostRender(renderContext);
		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		//DebugWireframe::getDebugMode()->RenderContextUpdate(renderContext);
	
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//tkmFileManager���폜
	nsMuscle::ResourceBankManager::DeleteInstance();

	nsMuscle::LightManager::DeleteInstance();
	nsMuscle::PostEffectManager::DeleteInstance();
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

