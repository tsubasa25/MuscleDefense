/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
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
	//死亡フラグがついているゲームオブジェクトを破棄する。
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
	//物理エンジンのアップデートを呼び出す。
	PhysicsWorld::GetInstance()->Update(1.0f/60.0f);
}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	//TODO:暫定処理、フラグ含め別の形にしたい
	//影を先に描いてからモデルに描いた影を描き足すので先にシャドウマップをつくる。
	nsMuscle::PostEffectManager::GetInstance()->ShadowRender(rc);

	////shadow
	if (nsMuscle::PostEffectManager::GetInstance()->GetShadowFlag())
	{
		rc.SetStep(RenderContext::eStep_RenderShadowMap);
		//ShadowRenderでビューポートを設定しているのでここでビューポート設定しなくてOK(たぶん)
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->RenderWrapper(rc, nsMuscle::LightManager::GetInstance()->GetLightCamera());
			}
		}
	}
	nsMuscle::PostEffectManager::GetInstance()->EndShadowRender(rc);

	//ポストエフェクト用。Render前の処理
	nsMuscle::PostEffectManager::GetInstance()->BeforeRender(rc);

	//レンダラーを変更するならここを改造していくと良い。
	if(m_isWipeScreenMode)
	{ 
	rc.SetStep(RenderContext::eStep_Render);
	
	//1P側の画面のカメラは1Pのカメラ(g_camera3D[0])
	nsMuscle::LightManager::GetInstance()->UpdateEyePos(0);
	
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc,g_camera3D[0]);			
		}
	}
}
	else
	{
		//ワイプ用のカメラg_camera3D[1]
		rc.SetStep(RenderContext::eStep_RenderWipe);

		nsMuscle::LightManager::GetInstance()->UpdateEyePos(1);
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->Render(rc, g_camera3D[1]);
			}
		}
	}
	//ポストエフェクト用。Render後の処理
	nsMuscle::PostEffectManager::GetInstance()->AfterRender(rc);
	
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
	//Level2D用　
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