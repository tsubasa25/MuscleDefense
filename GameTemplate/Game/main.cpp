#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"
#include "BackGround.h"
namespace
{
	const Vector3 LIGHTCAMERA_POSITION = { 0.0f, 4000.0f,0.0f };
	const Vector3 LIGHTCAMERA_TARGET = { 0.f,1.f,0.f };
	const Vector3 LIGHTCAMERA_UP = { 1.f,0.f,0.f };
	const float LIGHTCAMERA_WIDTH = 4000.0f;//2000
	const float LIGHTCAMERA_HEIGHT = 4000.0f;
	const int CAMERA_FAR = 100000;
}

//////////////////////////////////////
// 関数宣言
//////////////////////////////////////

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	g_camera3D->SetPosition({ 0.0f, 150.0f, 200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	
	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////
	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	nsMuscle::PostEffectManager::CreateInstance();

	//ライトマネージャーのインスタンスを作成
	nsMuscle::LightManager::CreateInstance();
	nsMuscle::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
	nsMuscle::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
	nsMuscle::LightManager::GetInstance()->SetPointLigNum(0);	
	
	//ブルームフラグ、シャドウフラグの順番
	nsMuscle::PostEffectManager::GetInstance()->Init(true,true);

	NewGO<nsMuscle::GameScene>(0);
	NewGO<nsMuscle::BackGround>(0);
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();		

		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		nsMuscle::LightManager::GetInstance()->UpdateEyePos();
		
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

