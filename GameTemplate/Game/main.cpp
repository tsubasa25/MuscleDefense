#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"
#include "BackGround.h"
#include "TitleScene.h"
#include "DebagWireFrame.h"

#include "TestScene.h"
#include "time/Stopwatch.h"


#define OUTPUT_1FRAME_TIME	// 有効で1フレームの経過時間を表示

namespace
{	
	const Vector3 LIGHTCAMERA_UP = { -5.0f,-3.0f,0.0f };
	const float LIGHTCAMERA_WIDTH = 2000.0f;
	const float LIGHTCAMERA_HEIGHT = 2000.0f;
	const int CAMERA_FAR = 100000;
}

//////////////////////////////////////
// 関数宣言
//////////////////////////////////////
bool isGameEnd = false;//ゲーム終了か
///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////
	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	nsMuscle::PostEffectManager::CreateInstance();
	//リソースマネージャのインスタンスを作成
	nsMuscle::ResourceBankManager::CreateInstance();

	
	//ライトマネージャーのインスタンスを作成
	nsMuscle::LightManager::CreateInstance();
	nsMuscle::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
	nsMuscle::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
	nsMuscle::LightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Perspective);
	nsMuscle::LightManager::GetInstance()->SetPointLigNum(0);
	nsMuscle::LightManager::GetInstance()->SetSpotLigNum(0);
	nsMuscle::LightManager::GetInstance()->SetLightCameraFar(10000.0f);
	//DebugWireframe::Init();
	//ブルームフラグ、シャドウフラグの順番
	nsMuscle::PostEffectManager::GetInstance()->Init(true,true);
	NewGO<nsMuscle::TitleScene>(0, "titleScene");//タイトル
	NewGO<nsMuscle::GameScene>(0, "gameScene");//ゲームシーン
	//NewGO<nsMuscle::TestScene>(0);//デバック用
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		Stopwatch sw;
		sw.Start();
		//レンダリング開始。
		g_engine->BeginFrame();		

		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		GameObjectManager::GetInstance()->ExecutePostRender(renderContext);
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		//DebugWireframe::getDebugMode()->RenderContextUpdate(renderContext);
	
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
		sw.Stop();
#ifdef OUTPUT_1FRAME_TIME
		char text[256];
		sprintf(text, "1frame time = %f\n", sw.GetElapsedMillisecond());
		OutputDebugStringA(text);
#endif
	}
	//tkmFileManagerを削除
	nsMuscle::ResourceBankManager::DeleteInstance();

	nsMuscle::LightManager::DeleteInstance();
	nsMuscle::PostEffectManager::DeleteInstance();
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

